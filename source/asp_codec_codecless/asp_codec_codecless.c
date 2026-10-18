/*
 * asp_codec_codecless.c - ASoC codec driver for ASP code-less mode
 *
 * Provides the "asp-codec-codecless" component and "asp-codec-codecless-dai"
 * DAI that da_combine_machine (CODECLESS_OUTPUT link) and analog_less_machine
 * expect on Kirin platforms that route audio through the ASP block with no
 * external codec chip.
 *
 * Registers a table of locally-emulated mixer controls
 * (codecless_virt_controls.h, auto-generated): the vendor asp_codec family
 * registered ~430 controls (MIC/ADC PGA gains, ASP path switches, I2S/TDM
 * config) whose driver source is not part of the open-source drop. The audio
 * HAL looks these up during mixer init (audio_route / audio_pop /
 * initStreamVolume) and fails outright when they are absent.
 *
 * Additionally maps the real ASP codec register window (0xFA552000..0xFA55228C)
 * so that:
 *   1) snd_soc_component_read32/write work on this component, and
 *   2) the vendor debug interface (/proc/audio/{rr,rh,dwidget,dpath,dctrl})
 *      registers against this codec, letting us read/compare register state.
 * If the mapping fails the driver still registers (soft fallback): the sound
 * card must keep working regardless.
 *
 * GPL-2.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/io.h>
/* MOLI_CLK: 原厂 asp_codec_codec_probe 依赖的资源（反汇编得到名字） */
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
/* MOLI_PINCTRL */
#include <linux/pinctrl/consumer.h>
/* MOLI_I2C: 读 smartpa 状态 */
#include <linux/i2c.h>

/* MOLI_PROC */
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/regmap.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-component.h>
/* MOLI_INCORDER: 需要 snd_soc_component_read32 等原型 */
#include "asoc_adapter.h"
#include <sound/pcm_params.h>

#include "asp_widgets_gen.h"
#include "asp_events_gen.h"
#include "asp_reg_map.h"
/* MOLI_PATHEV: 三个虚拟开关的真实硬件动作（stock163.elf 反汇编） */
static void moli_path_event(struct snd_soc_component *c,
                            unsigned int idx, unsigned int on);
static void moli_apply_seq(struct snd_soc_component *c, const char *name,
			   unsigned int on, int from_put);

/* MOLI_VIRT_REGMAP: virt 控件直接用自己的 codecless_regmap，
 * 不依赖 snd_soc_kcontrol_component()（实测该路径下所有 virt 控件 get 均为 0）。
 */
extern struct regmap *codecless_regmap;
static unsigned int moli_virt_read(unsigned int reg)
{
	unsigned int v = 0;

	if (codecless_regmap)
		regmap_read(codecless_regmap, reg, &v);
	return v;
}
static int moli_virt_update(unsigned int reg, unsigned int mask, unsigned int val)
{
	if (!codecless_regmap)
		return -ENODEV;
	return regmap_update_bits(codecless_regmap, reg, mask, val);
}

#include "codecless_virt_controls.h"

/* vendor debug helper (sound/soc/codecs/vendor/codec_debug.[ch]) */
#include "codec_debug.h"
/* vendor shim: declares snd_soc_component_read32 for >= 5.10 */

#define ASP_CODEC_CODECLESS_DRV_NAME  "asp-codec-codecless"
#define ASP_CODEC_CODECLESS_DAI_NAME  "asp-codec-codecless-dai"

/* ASP codec register window, from soc_acpu_baseaddr_interface.h */
#define CODECLESS_REG_BASE  0xFA552000UL
#define CODECLESS_REG_BYTES 0x1000UL
/* MOLI_WIDEN: 原来只到 0x28C；MICBIAS 落在 0x284 说明模拟/ADC
 * 相关寄存器在更高处，这里把窗口开到 0x3FF（DT 窗口是 0x1000，够）。
 */
#define CODECLESS_REG_MAX   0x3FF

/* MOLI_SEQ: asp_cfg 区（DT reg[1] = <0xFA54E000 0x400>） */
#define CODECLESS_CFG_BASE   0xFA54E000UL
#define CODECLESS_CFG_BYTES  0x400UL

/* MOLI_DMAREG: ASP DMA 控制器（DT: codec_asp_dmac@fa54b000），
 * 用于客观判断音频数据是否在流向 codec。
 */
#define CODECLESS_DMA_BASE   0xFA54B000UL
#define CODECLESS_DMA_BYTES  0x1000UL
static void __iomem *codecless_base;
struct regmap *codecless_regmap;	/* MOLI_VIRT_REGMAP: 非 static，供 virt 控件直接使用 */

/* MOLI_CLK */
static struct clk *codecless_clk_subsys;
static struct clk *codecless_clk_codec;
static struct regulator *codecless_vdd;

/* MOLI_SEQ */
static const struct regmap_config codecless_cfg_regmap_cfg = {
	.name         = "asp-codec-codecless-cfg",
	.reg_bits     = 32,
	.val_bits     = 32,
	.reg_stride   = 4,
	.max_register = CODECLESS_CFG_BYTES,
	.cache_type   = REGCACHE_NONE,
};

static void __iomem *codecless_cfg_base;
static struct regmap *codecless_cfg_regmap;

/* MOLI_DMAREG */
static void __iomem *codecless_dma_base;

static const struct regmap_config codecless_regmap_cfg = {
	.name         = "asp-codec-codecless",
	.reg_bits     = 32,
	.val_bits     = 32,
	.reg_stride   = 4,
	.max_register = CODECLESS_REG_MAX,
	.cache_type   = REGCACHE_NONE,
};

#ifdef CONFIG_SND_SOC_CODEC_DEBUG
/*
 * Entry addresses must match DBG_CODECLESS_START_ADDR / DBG_CODECLESS_END_ADDR
 * (see codec_debug.h) or hicodec_debug_init() will file this codec under
 * ASP_CODEC instead of ASP_CODECLESS.
 */
static struct hicodec_dump_reg_entry codecless_dump_table[] = {
	{
		.seg_name = "codecless",
		.start    = CODECLESS_REG_BASE,
		.end      = CODECLESS_REG_BASE + CODECLESS_REG_MAX,
		.reg_size = sizeof(unsigned int),
	},
};

static struct hicodec_dump_reg_info codecless_dump_info = {
	.entry = codecless_dump_table,
	.count = ARRAY_SIZE(codecless_dump_table),
};
#endif /* CONFIG_SND_SOC_CODEC_DEBUG */

static int asp_codec_codecless_startup(struct snd_pcm_substream *substream,
				       struct snd_soc_dai *dai)
{
	return 0;
}

static void asp_codec_codecless_shutdown(struct snd_pcm_substream *substream,
					 struct snd_soc_dai *dai)
{
}

static int asp_codec_codecless_hw_params(struct snd_pcm_substream *substream,
					 struct snd_pcm_hw_params *params,
					 struct snd_soc_dai *dai)
{
	return 0;
}

static int asp_codec_codecless_hw_free(struct snd_pcm_substream *substream,
				       struct snd_soc_dai *dai)
{
	return 0;
}

static int asp_codec_codecless_prepare(struct snd_pcm_substream *substream,
				       struct snd_soc_dai *dai)
{
	return 0;
}

/* moli_run_seq 定义在后，先声明 */
static void moli_run_seq(struct snd_soc_component *component);

/* ==================== MOLI_GUARD ====================
 * 实测：DSP 异常会让 ASP codec 块整体复位，寄存器全部归零。
 * 原厂靠 codec_reset_uevent 重新初始化；我们没有那套机制，
 * 所以在每次 PCM 启动时检查一下，发现被清零就重下发初始化序列。
 */
static int moli_codec_was_reset(struct snd_soc_component *c)
{
	unsigned int v = 0;

	if (!c)
		return 0;
	if (snd_soc_component_read32(c, 0x018) == 0 &&
	    snd_soc_component_read32(c, 0x004) == 0) {
		pr_warn("MOLI_GUARD: codec looks wiped, re-init\n");
		return 1;
	}
	return 0;
}

static void moli_reset_guard(struct snd_soc_dai *dai)
{
	struct snd_soc_component *c = dai ? dai->component : NULL;

	if (!c)
		return;
	if (!moli_codec_was_reset(c))
		return;
	moli_run_seq(c);
}

static int asp_codec_codecless_trigger(struct snd_pcm_substream *substream,
				       int cmd, struct snd_soc_dai *dai)
{
	/* MOLI_GUARD: 流启动时确认 codec 没被 DSP 复位清零 */
	if (cmd == SNDRV_PCM_TRIGGER_START ||
	    cmd == SNDRV_PCM_TRIGGER_RESUME)
		moli_reset_guard(dai);
	return 0;
}

static int asp_codec_codecless_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	return 0;
}

static int asp_codec_codecless_set_sysclk(struct snd_soc_dai *dai,
					  int clk_id, unsigned int freq, int dir)
{
	return 0;
}

static const struct snd_soc_dai_ops asp_codec_codecless_dai_ops = {
	.startup    = asp_codec_codecless_startup,
	.shutdown   = asp_codec_codecless_shutdown,
	.hw_params  = asp_codec_codecless_hw_params,
	.hw_free    = asp_codec_codecless_hw_free,
	.prepare    = asp_codec_codecless_prepare,
	.trigger    = asp_codec_codecless_trigger,
	.set_fmt    = asp_codec_codecless_set_fmt,
	.set_sysclk = asp_codec_codecless_set_sysclk,
};

/* 组件 probe：映射真实寄存器窗口并挂上 debug 接口 */
/* MOLI_CLK: 使能供电与两路时钟。
 * 顺序照抄原厂：regulator -> clk_asp_subsys -> clk_asp_codec。
 * 只打日志，不触碰寄存器。
 */
static void codecless_enable_resources(struct device *dev)
{
	int ret;

	codecless_vdd = regulator_get(dev, "asp");
	if (IS_ERR(codecless_vdd)) {
		pr_warn("%s: regulator \"asp\" get failed %ld\n", __func__,
			PTR_ERR(codecless_vdd));
		codecless_vdd = NULL;
	} else {
		ret = regulator_enable(codecless_vdd);
		pr_info("%s: regulator \"asp\" enabled ret=%d\n", __func__, ret);
	}

	codecless_clk_subsys = clk_get(dev, "clk_asp_subsys");
	if (IS_ERR(codecless_clk_subsys)) {
		pr_warn("%s: clk \"clk_asp_subsys\" get failed %ld\n", __func__,
			PTR_ERR(codecless_clk_subsys));
		codecless_clk_subsys = NULL;
	} else {
		ret = clk_prepare_enable(codecless_clk_subsys);
		pr_info("%s: clk_asp_subsys enabled ret=%d\n", __func__, ret);
	}

	codecless_clk_codec = clk_get(dev, "clk_asp_codec");
	if (IS_ERR(codecless_clk_codec)) {
		pr_warn("%s: clk \"clk_asp_codec\" get failed %ld\n", __func__,
			PTR_ERR(codecless_clk_codec));
		codecless_clk_codec = NULL;
	} else {
		ret = clk_prepare_enable(codecless_clk_codec);
		pr_info("%s: clk_asp_codec enabled ret=%d\n", __func__, ret);
	}
}

/* MOLI_PROC: 暴露驱动状态到 /proc/moli_audio（printk 会被环形缓冲冲掉） */
static char moli_status[2048];
static int moli_status_len;

static void moli_log(const char *fmt, ...)
{
	va_list ap;
	int n;

	if (moli_status_len >= (int)sizeof(moli_status) - 2)
		return;
	va_start(ap, fmt);
	n = vsnprintf(moli_status + moli_status_len,
		      sizeof(moli_status) - moli_status_len, fmt, ap);
	va_end(ap);
	if (n > 0)
		moli_status_len += n;
}

static char moli_live[8192];

/* MOLI_LIVEREG: 实时读 codec 寄存器，输出到 /proc/moli_audio。
 * 用于判断 HAL 的 mixer 写入是否真的落到硬件。
 */
static int moli_build_live(void)
{
	char *p = moli_live;
	int n = 0, i;
	unsigned int v;

	n += scnprintf(p + n, sizeof(moli_live) - n,
		       "=== status ===\n%s\n", moli_status);
	/* MOLI_WRLOG: HAL 写控件记录 */
	n += scnprintf(p + n, sizeof(moli_live) - n,
		       "=== ctl writes (total=%u) ===\n", virt_wrlog_total);
	{
		unsigned int i, start;
		unsigned int cnt = virt_wrlog_total < VIRT_WRLOG_N ? virt_wrlog_total : VIRT_WRLOG_N;
		start = virt_wrlog_total > VIRT_WRLOG_N ? virt_wrlog_total - VIRT_WRLOG_N : 0;
		for (i = 0; i < cnt && n < (int)sizeof(moli_live) - 48; i++) {
			unsigned int k = (start + i) % VIRT_WRLOG_N;
			n += scnprintf(p + n, sizeof(moli_live) - n,
				       "  idx=%u val=%u mapped=%u\n",
				       virt_wrlog[k].idx, virt_wrlog[k].val,
				       virt_wrlog[k].mapped);
		}
	}

	/* MOLI_DMAREG */
	n += scnprintf(p + n, sizeof(moli_live) - n,
		       "=== asp dma regs 0xFA54B000 (nonzero only) ===\n");
	if (codecless_dma_base) {
		unsigned int off;
		for (off = 0; off < 0x200 && n < (int)sizeof(moli_live) - 32; off += 4) {
			unsigned int dv = readl(codecless_dma_base + off);
			if (dv)
				n += scnprintf(p + n, sizeof(moli_live) - n,
					       "  0x%03X = 0x%08X\n", off, dv);
		}
	} else {
		n += scnprintf(p + n, sizeof(moli_live) - n, "  (not mapped)\n");
	}

	n += scnprintf(p + n, sizeof(moli_live) - n,
		       "=== asp cfg regs 0x00-0x3FF (0xFA54E000, nonzero only) ===\n");
	if (codecless_cfg_regmap) {
		unsigned int co;

		for (co = 0; co < 0x400 && n < (int)sizeof(moli_live) - 32; co += 4) {
			unsigned int cv = 0;

			if (regmap_read(codecless_cfg_regmap, co, &cv) == 0 && cv)
				n += scnprintf(p + n, sizeof(moli_live) - n,
					       "  0x%03X = 0x%08X\n", co, cv);
		}
	}

	n += scnprintf(p + n, sizeof(moli_live) - n,
		       "=== codec regs 0x00-0x28C (offset from 0xFA552000) ===\n");
	if (codecless_regmap) {
		for (i = 0; i <= CODECLESS_REG_MAX && n < (int)sizeof(moli_live) - 32; i += 4) {
			if (regmap_read(codecless_regmap, i, &v) == 0) {
				if (v)
					n += scnprintf(p + n, sizeof(moli_live) - n,
						       "  0x%03X = 0x%08X\n", i, v);
			}
		}
	} else {
		n += scnprintf(p + n, sizeof(moli_live) - n, "  (no regmap)\n");
	}
	return n;
}

/* ==================== MOLI_CTL ====================
 * 运行时调试口 /proc/moli_ctl，避免每次试寄存器都要重新编译刷机。
 *   mask <n>              pathev 组掩码：bit0=4PA  bit1=2PA_DN  bit2=I2S1_TX
 *   w  <reg> <val>        codec 区(0x00..0x28C) 直写
 *   u  <reg> <mask> <val> codec 区 update_bits
 *   r  <reg>              codec 区读
 *   cw <reg> <val>        cfg 区(0xFA54E000, 0x00..0x3FF) 直写
 *   cu <reg> <mask> <val> cfg 区 update_bits
 * 结果追加到 /proc/moli_audio 的 status 段。
 */
/* 默认关闭：0x1=4PA(不含 0x010 bit5) 0x2=2PA_DN 0x4=I2S1_TX_TDM
 * 0x010 bit5 实测会让 HiFi DSP 抛 SOCHIFI_S_EXCEPTION，已从序列剔除。
 */
static unsigned int moli_pathev_mask;	/* 默认 0：配置完全交给 DAPM 事件 */

/* MOLI_DAPMEV: bit0=1 允许 DAPM widget 事件写寄存器 */
static unsigned int moli_dapm_ev_mask = 0x1;

/* MOLI_SETCTL: 卡指针，用于按名字找控件 */
static struct snd_soc_card *moli_card;

/* MOLI_I2C: 一次简单的读事务，只读不写（安全）。
 * 目的：读 smartpa 的状态/ID 寄存器，判断 I2S 时钟与信号是否到位。
 */
static int moli_i2c_read(int bus, u16 addr, u8 reg, u8 *buf, int len)
{
	struct i2c_adapter *adap;
	struct i2c_msg msgs[2];
	u8 r = reg;
	int ret;

	adap = i2c_get_adapter(bus);
	if (!adap) {
		pr_warn("MOLI_I2C: bus %d not found\n", bus);
		return -ENODEV;
	}

	msgs[0].addr = addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &r;

	msgs[1].addr = addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = buf;

	ret = i2c_transfer(adap, msgs, 2);
	i2c_put_adapter(adap);

	if (ret == 2)
		return 0;
	return ret < 0 ? ret : -EIO;
}

/* MOLI_SETCTL: 按名字设置卡上任意控件（含其他组件的控件）。
 * 目的：验证 da_combine 的 MICBIAS / 模拟通路是否因为没被上电导致麦克风静音。
 */
static int moli_setctl(unsigned int val, const char *name)
{
	struct snd_kcontrol *kctl;
	struct snd_ctl_elem_value *u;
	int ret;

	if (!moli_card) {
		pr_warn("MOLI_SETCTL: no card\n");
		return -ENODEV;
	}

	kctl = snd_soc_card_get_kcontrol(moli_card, name);
	if (!kctl) {
		pr_warn("MOLI_SETCTL: control '%s' not found\n", name);
		return -ENOENT;
	}

	u = kzalloc(sizeof(*u), GFP_KERNEL);
	if (!u)
		return -ENOMEM;
	u->id = kctl->id;
	u->value.integer.value[0] = val;
	u->value.integer.value[1] = val;

	ret = kctl->put(kctl, u);
	pr_info("MOLI_SETCTL: '%s' <- %u ret=%d\n", name, val, ret);
	kfree(u);
	return ret < 0 ? ret : 0;
}

/* ==================== MOLI_CTLLIST ====================
 * /proc/moli_ctls：枚举卡上全部控件（名字 + 值）。
 * dctrl 只输出前 187 行（共 597），我们组件的控件看不到。
 */
static char moli_ctls_buf[65536];

static int moli_ctls_build(void)
{
	struct snd_kcontrol *k;
	int n = 0, cnt = 0;

	if (!moli_card || !moli_card->snd_card)
		return 0;

	n += scnprintf(moli_ctls_buf + n, sizeof(moli_ctls_buf) - n,
		       "BEGIN ctls card=%s\n", moli_card->name);

	list_for_each_entry(k, &moli_card->snd_card->controls, list) {
		struct snd_ctl_elem_value u;
		long v0 = -1, v1 = -1;

		if (n > (int)sizeof(moli_ctls_buf) - 160)
			break;
		memset(&u, 0, sizeof(u));
		u.id = k->id;
		if (k->get && k->get(k, &u) == 0) {
			v0 = u.value.integer.value[0];
			v1 = u.value.integer.value[1];
		}
		n += scnprintf(moli_ctls_buf + n, sizeof(moli_ctls_buf) - n,
			       "%d\t%s\t%ld\t%ld\n", cnt++, k->id.name, v0, v1);
	}
	n += scnprintf(moli_ctls_buf + n, sizeof(moli_ctls_buf) - n,
		       "END total=%d\n", cnt);
	return n;
}

static ssize_t moli_ctls_read(struct file *f, char __user *buf,
			      size_t count, loff_t *ppos)
{
	int len = moli_ctls_build();

	return simple_read_from_buffer(buf, count, ppos, moli_ctls_buf, len);
}

static const struct proc_ops moli_ctls_pops = {
	.proc_read = moli_ctls_read,
};

/* MOLI_SYNCALL: 对卡上所有组件各做一次 DAPM power 评估。
 * da_combine_v5 的 240 个 widget 在播放/录音时都是 0 上电，
 * 用它验证是否只是缺了一次 sync。
 */
static int moli_sync_all(void)
{
	struct snd_soc_component *c;
	int n = 0;

	if (!moli_card)
		return -ENODEV;

	for_each_card_components(moli_card, c) {
		int r = snd_soc_dapm_sync(&c->dapm);

		pr_info("MOLI_SYNCALL: %s sync=%d\n", c->name, r);
		n++;
	}
	snd_soc_dapm_sync(&moli_card->dapm);
	pr_info("MOLI_SYNCALL: %d components\n", n);
	return n;
}

/*
 * MOLI_PCM: 把卡上所有 substream 的 runtime 关键字段打到 dmesg。
 * 目的：判断 ALSA 侧是否真的在搬数据（hw_ptr 前进），
 * 以及拿到 dma_addr，好去采样真正的 PCM 缓冲。
 */
/* MOLI_SEQCMD: 按名字找到 codec 组件，供 seq 命令使用 */
static struct snd_soc_component *moli_comp_by_name(const char *name)
{
	struct snd_soc_component *c;

	if (!moli_card || !name)
		return NULL;
	for_each_card_components(moli_card, c) {
		if (c->name && !strcmp(c->name, name))
			return c;
	}
	return NULL;
}

static struct snd_soc_component *moli_codec_comp(void)
{
	return moli_comp_by_name(ASP_CODEC_CODECLESS_DRV_NAME);
}

/* MOLI_DCW: 原厂播放/录音路径真正写的是 da_combine 的寄存器
 * （audio_play_drv_power_event -> play_config_power_event）。
 * 我们之前只写 ASP codec 块，方向不对，这里补一条通道。
 */
#define MOLI_DCV5_NAME "da_combine_v5"

static void moli_dump_pcm(void)
{
	struct snd_soc_pcm_runtime *rtd;
	int s;

	if (!moli_card) {
		pr_info("MOLI_PCM: no card\n");
		return;
	}

	for_each_card_rtds(moli_card, rtd) {
		struct snd_pcm *pcm = rtd->pcm;

		if (!pcm)
			continue;
		for (s = 0; s < 2; s++) {
			struct snd_pcm_substream *sub = pcm->streams[s].substream;
			struct snd_pcm_runtime *rt;

			if (!sub || !sub->runtime)
				continue;
			rt = sub->runtime;
			pr_info("MOLI_PCM: %s dev=%d str=%d dma=%pa bytes=%zu buf=%lu per=%lu st=%d hw=%lu appl=%lu\n",
				rtd->dai_link->name, pcm->device, s,
				&rt->dma_addr, rt->dma_bytes,
				(unsigned long)rt->buffer_size,
				(unsigned long)rt->period_size,
				(int)rt->status->state,
				(unsigned long)(rt->status ? rt->status->hw_ptr : 0),
				(unsigned long)(rt->control ? rt->control->appl_ptr : 0));
		}
	}
}

static ssize_t moli_ctl_write(struct file *f, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	char kbuf[128];
	char cmd[16];
	unsigned int a, b, c;
	struct regmap *m;

	if (count >= sizeof(kbuf))
		count = sizeof(kbuf) - 1;
	if (copy_from_user(kbuf, buf, count))
		return -EFAULT;
	kbuf[count] = '\0';

	if (sscanf(kbuf, "%15s", cmd) != 1)
		return -EINVAL;

	if (!strcmp(cmd, "mask")) {
		if (sscanf(kbuf, "%*s %x", &a) == 1) {
			moli_pathev_mask = a;
			moli_log("\n[ctl] pathev mask = 0x%X\n", a);
		}
	} else if (!strcmp(cmd, "w") || !strcmp(cmd, "cw")) {
		if (sscanf(kbuf, "%*s %x %x", &a, &b) == 2) {
			m = (cmd[0] == 'c') ? codecless_cfg_regmap : codecless_regmap;
			if (m)
				moli_log("\n[ctl] w 0x%X <- 0x%X ret=%d\n", a, b,
					 regmap_write(m, a, b));
		}
	} else if (!strcmp(cmd, "u") || !strcmp(cmd, "cu")) {
		if (sscanf(kbuf, "%*s %x %x %x", &a, &b, &c) == 3) {
			m = (cmd[0] == 'c') ? codecless_cfg_regmap : codecless_regmap;
			if (m)
				moli_log("\n[ctl] u 0x%X m 0x%X v 0x%X ret=%d\n",
					 a, b, c, regmap_update_bits(m, a, b, c));
		}
	} else if (!strcmp(cmd, "r")) {
		if (sscanf(kbuf, "%*s %x", &a) == 1) {
			unsigned int v = 0xdeadbeef;

			if (codecless_regmap)
				regmap_read(codecless_regmap, a, &v);
			moli_log("\n[ctl] r 0x%X = 0x%X\n", a, v);
		}
	} else if (!strcmp(cmd, "dw")) {
		struct snd_soc_component *dc = moli_comp_by_name(MOLI_DCV5_NAME);
		unsigned int dm = 0, dv = 0;
		int drc;

		if (!dc) {
			pr_info("MOLI_DCW: component %s not found\n", MOLI_DCV5_NAME);
		} else if (sscanf(kbuf, "%*s %x %x %x", &a, &dm, &dv) == 3) {
			drc = snd_soc_component_update_bits(dc, a, dm, dv);
			pr_info("MOLI_DCW: dw 0x%X m 0x%X v 0x%X ret=%d\n",
				a, dm, dv, drc);
		}
	} else if (!strcmp(cmd, "dr")) {
		struct snd_soc_component *dc = moli_comp_by_name(MOLI_DCV5_NAME);

		if (!dc) {
			pr_info("MOLI_DCW: component %s not found\n", MOLI_DCV5_NAME);
		} else if (sscanf(kbuf, "%*s %x", &a) == 1) {
			pr_info("MOLI_DCW: dr 0x%X = 0x%X\n", a,
				snd_soc_component_read(dc, a));
		}
	} else if (!strcmp(cmd, "seq")) {
		char onoff[8];
		char nm[40];
		struct snd_soc_component *c;

		if (sscanf(kbuf, "%*s %7s %39s", onoff, nm) == 2) {
			c = moli_codec_comp();
			if (!c) {
				pr_info("MOLI_SEQCMD: no component\n");
			} else {
				moli_apply_seq(c, nm, !strcmp(onoff, "on") ? 1 : 0, 2);
				pr_info("MOLI_SEQCMD: %s %s\n", nm, onoff);
			}
		}
	} else if (!strcmp(cmd, "pcm")) {
		moli_dump_pcm();
	} else if (!strcmp(cmd, "dm")) {
		/* 读 ASP DMAC (0xFA54B000) 的一个寄存器：看有没有通道在搬数据 */
		if (sscanf(kbuf, "%*s %x", &a) == 1 && codecless_dma_base) {
			unsigned int v = readl(codecless_dma_base + (a & 0xFFFU));

			moli_log("\n[ctl] dm 0x%X = 0x%X\n", a, v);
		}
	} else if (!strcmp(cmd, "dmd")) {
		/* 批量 dump DMAC，走 dmesg（moli_log 只有 2KB，装不下） */
		if (sscanf(kbuf, "%*s %x %x", &a, &b) == 2 && codecless_dma_base &&
		    b > a && (b - a) <= 0x1000U) {
			unsigned int o;

			for (o = a & ~3U; o < b; o += 16)
				pr_info("MOLI_DM: %03X: %08X %08X %08X %08X\n", o,
					readl(codecless_dma_base + o),
					readl(codecless_dma_base + o + 4),
					readl(codecless_dma_base + o + 8),
					readl(codecless_dma_base + o + 12));
		}
	} else if (!strcmp(cmd, "pm")) {
		/*
		 * 读 DSP 保留 DDR 的物理内存（只放行两个 DSP 区）：
		 *   RESERVED_HIFI_PHYMEM_BASE      0x2DA00000 + 0x0F80000
		 *   RESERVED_HIFI_DATA_PHYMEM_BASE 0x2F380000 + 0x0580000
		 * 用来判断 DSP 有没有真的往它自己的播放缓冲写东西。
		 */
		unsigned int len = 0;

		if (sscanf(kbuf, "%*s %x %x", &a, &len) == 2 && len > 0 &&
		    len <= 0x1000U &&
		    (((a >= 0x2DA00000U) && (a + len) <= 0x2E980000U) ||
		     ((a >= 0x2F380000U) && (a + len) <= 0x2F900000U))) {
			void __iomem *p = memremap(a, len, MEMREMAP_WB);

			if (p) {
				unsigned int o;

				for (o = 0; o < len; o += 16)
					pr_info("MOLI_PM: %08X: %08X %08X %08X %08X\n", a + o,
						readl(p + o), readl(p + o + 4),
						readl(p + o + 8), readl(p + o + 12));
				memunmap(p);
			} else {
				pr_info("MOLI_PM: memremap 0x%X failed\n", a);
			}
		}
	} else if (!strcmp(cmd, "i2c")) {
		unsigned int bus, addr, reg, len = 1;
		u8 buf[8];
		int k, rc;

		if (sscanf(kbuf, "%*s %x %x %x %x", &bus, &addr, &reg, &len) >= 3) {
			if (len == 0 || len > sizeof(buf))
				len = 1;
			rc = moli_i2c_read((int)bus, (u16)addr, (u8)reg, buf, (int)len);
			/* dmesg 通道（缓冲更大，适合批量探测） */
			pr_info("MOLI_I2C: bus=%u addr=0x%02X reg=0x%02X rc=%d %02X %02X %02X %02X\n",
				bus, addr, reg, rc,
				len > 0 ? buf[0] : 0, len > 1 ? buf[1] : 0,
				len > 2 ? buf[2] : 0, len > 3 ? buf[3] : 0);
			moli_log("\n[i2c] bus=%u addr=0x%02X reg=0x%02X rc=%d :",
				 bus, addr, reg, rc);
			for (k = 0; k < (int)len; k++)
				moli_log(" %02X", buf[k]);
			moli_log("\n");
		}
	} else if (!strcmp(cmd, "setctl")) {
		unsigned int val;
		char name[96];

		if (sscanf(kbuf, "%*s %x %95[^\n]", &val, name) == 2) {
			moli_setctl(val, name);
		}
	} else if (!strcmp(cmd, "syncall")) {
		moli_log("\n[ctl] syncall=%d\n", moli_sync_all());
	} else if (!strcmp(cmd, "dmask")) {
		if (sscanf(kbuf, "%*s %x", &a) == 1) {
			moli_dapm_ev_mask = a;
			moli_log("\n[ctl] dapm ev mask = 0x%X\n", a);
		}
	} else if (!strcmp(cmd, "cr")) {
		if (sscanf(kbuf, "%*s %x", &a) == 1) {
			unsigned int v = 0xdeadbeef;

			if (codecless_cfg_regmap)
				regmap_read(codecless_cfg_regmap, a, &v);
			moli_log("\n[ctl] cr 0x%X = 0x%X\n", a, v);
		}
	} else if (!strcmp(cmd, "hs")) {
		unsigned int ms = 300, i;

		sscanf(kbuf, "%*s %u", &ms);
		m = codecless_cfg_regmap;
		if (m) {
			unsigned int v = 0xdeadbeef;

			regmap_write(m, 0x90, 1);
			moli_log("\n[hs] start cfg[0x90]=1 ms=%u\n", ms);
			for (i = 0; i < ms; i++) {
				usleep_range(1000, 1100);
				regmap_read(m, 0x90, &v);
				if (i < 4 || v == 0)
					moli_log("[hs] t=%ums v=0x%X\n", i, v);
				if (v == 0)
					break;
			}
			moli_log("[hs] end i=%u v=0x%X\n", i, v);
		}
	} else if (!strcmp(cmd, "clear")) {
		moli_status_len = 0;
		moli_status[0] = 0;
	} else if (!strcmp(cmd, "log")) {
		moli_log("\n[ctl] ---- marker ----\n");
	}
	return count;
}

static const struct proc_ops moli_ctl_pops = {
	.proc_write = moli_ctl_write,
};

static ssize_t moli_audio_read(struct file *f, char __user *buf,
			       size_t count, loff_t *ppos)
{
	moli_build_live();
	return simple_read_from_buffer(buf, count, ppos,
				       moli_live, strlen(moli_live));
}

static const struct proc_ops moli_audio_pops = {
	.proc_read = moli_audio_read,
};

/* MOLI_SEQ: 原厂 0.163 内核反汇编得到的初始化序列
 *   asp_codec_codec_probe (cfg 区)
 *   asp_codec_v1_reg_init  (codec 区)
 * 相对偏移 = 原厂寄存器号 - 0x52000（codec）/ - 0x4E000（cfg）
 */
static const u32 moli_codec_seq[][2] = {
	{ 0x94,  0x400 },
	{ 0x98,  0x400 },
	{ 0x9C,  0x400 },
	{ 0xA0,  0x400 },
	{ 0xA4,  0x400 },
	{ 0x180, 0x400 },
	{ 0x17C, 0x400 },
	{ 0x178, 0x400 },
};

static void moli_run_seq(struct snd_soc_component *component)
{
	unsigned int v = 0;
	int i, ret;

	if (!codecless_cfg_regmap) {
		pr_warn("MOLI_SEQ: cfg regmap missing\n");
		return;
	}

	pr_info("MOLI_SEQ: cfg write 0x00\n");
	regmap_write(codecless_cfg_regmap, 0x00, 0x200);
	pr_info("MOLI_SEQ: cfg write 0x04\n");
	regmap_write(codecless_cfg_regmap, 0x04, 0x200);
	pr_info("MOLI_SEQ: cfg write 0x0C\n");
	regmap_write(codecless_cfg_regmap, 0x0C, 0x8);

	ret = regmap_read(codecless_cfg_regmap, 0x00, &v);
	pr_info("MOLI_SEQ: cfg readback 0x00=0x%x ret=%d\n", v, ret);

	pr_info("MOLI_SEQ: codec seq start\n");
	for (i = 0; i < ARRAY_SIZE(moli_codec_seq); i++) {
		snd_soc_component_update_bits(component, moli_codec_seq[i][0],
					      0x400, moli_codec_seq[i][1]);
		pr_info("MOLI_SEQ: codec 0x%x done\n", moli_codec_seq[i][0]);
	}
	pr_info("MOLI_SEQ: codec 0x20 / 0x14\n");
	snd_soc_component_update_bits(component, 0x20, 0xE000, 0x400);
	snd_soc_component_update_bits(component, 0x14, 0x40000000, 0x40000000);

	v = 0;
	ret = regmap_read(codecless_regmap, 0x94, &v);
	pr_info("MOLI_SEQ: codec readback 0x94=0x%x ret=%d\n", v, ret);
	v = 0;
	ret = regmap_read(codecless_regmap, 0x14, &v);
	pr_info("MOLI_SEQ: codec readback 0x14=0x%x ret=%d\n", v, ret);
	moli_log("seq done\n");
	pr_info("MOLI_SEQ: done\n");
}

/* MOLI_PINCTRL: DT 声明了 pinctrl-0/pinctrl-1（default/idle），
 * 原厂 asp_codec_v1_fpga_pinctrl_init 会切到 default。
 * 不切的话 I2S/ASP 引脚停在复位态，数字音频出不去。
 */
static struct pinctrl *codecless_pinctrl;
static struct pinctrl_state *codecless_pin_default;
static struct pinctrl_state *codecless_pin_idle;

static void codecless_pinctrl_init(struct device *dev)
{
	int ret;

	codecless_pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR(codecless_pinctrl)) {
		pr_warn("MOLI_PINCTRL: devm_pinctrl_get failed %ld\n",
			PTR_ERR(codecless_pinctrl));
		codecless_pinctrl = NULL;
		return;
	}

	codecless_pin_default = pinctrl_lookup_state(codecless_pinctrl, "default");
	if (IS_ERR(codecless_pin_default)) {
		pr_warn("MOLI_PINCTRL: no default state %ld\n",
			PTR_ERR(codecless_pin_default));
		codecless_pin_default = NULL;
	}

	codecless_pin_idle = pinctrl_lookup_state(codecless_pinctrl, "idle");
	if (IS_ERR(codecless_pin_idle))
		codecless_pin_idle = NULL;

	if (codecless_pin_default) {
		ret = pinctrl_select_state(codecless_pinctrl,
					   codecless_pin_default);
		pr_info("MOLI_PINCTRL: select default ret=%d idle=%d\n",
			ret, codecless_pin_idle ? 1 : 0);
	}
}

/* ==================== MOLI_DAPMEV ====================
 * 把 asp_events_gen.h 里的 (reg,mask) 序列挂成 widget 的 DAPM 电源事件。
 * 语义来自原厂：PRE_PMU(1) 置位，POST_PMD(8) 清零。
 * 原厂这些事件是这棵 DAPM 树唯一会写 codec 寄存器的路径，
 * 我们之前只手工复刻了 3 个开关，其余 37 个 widget 的事件是缺的。
 */
static struct snd_soc_dapm_widget *moli_widgets_ev;

static const struct moli_ev_seq *moli_find_seq(const char *name)
{
	int i;

	for (i = 0; i < (int)ARRAY_SIZE(moli_ev_seqs); i++) {
		if (!strcmp(moli_ev_seqs[i].widget, name))
			return &moli_ev_seqs[i];
	}
	return NULL;
}

/* MOLI_SEQFN: 把某个 widget 的 (reg,mask) 序列按 on/off 应用一遍。
 * 被 DAPM 事件和 /proc/moli_ctl 的 seq 命令共用。
 */
static void moli_apply_seq(struct snd_soc_component *c, const char *name,
			   unsigned int on, int from_put)
{
	const struct moli_ev_seq *seq = moli_find_seq(name);
	int i, last = 0;

	if (!seq || !c || !codecless_regmap)
		return;

	for (i = 0; i < seq->n; i++) {
		/* 0x010 bit5 打开 IV 4PA，实测会让 HiFi DSP 抛 SOCHIFI_S_EXCEPTION，
		 * 所以默认跳过；运行时用 `echo mask 1 > /proc/moli_ctl` 才放行。 */
		if (seq->ops[i].reg == 0x010 && !(moli_pathev_mask & 0x1))
			continue;
		last = snd_soc_component_update_bits(c, seq->ops[i].reg,
						     seq->ops[i].mask,
						     on ? seq->ops[i].mask : 0);
	}

	pr_info("MOLI_SEQ: %s %s ops=%d last=%d src=%d\n", name,
		on ? "on" : "off", seq->n, last, from_put);
}

static int moli_dapm_event(struct snd_soc_dapm_widget *w,
			   struct snd_kcontrol *kcontrol, int event)
{
	const struct moli_ev_seq *seq = moli_find_seq(w->name);
	struct snd_soc_component *c;
	unsigned int on;
	int i, last = 0;

	if (!seq || !(moli_dapm_ev_mask & 0x1))
		return 0;

	if (event == SND_SOC_DAPM_PRE_PMU)
		on = 1;
	else if (event == SND_SOC_DAPM_POST_PMD)
		on = 0;
	else
		return 0;

	c = w->dapm ? w->dapm->component : NULL;
	if (!c)
		return 0;

	for (i = 0; i < seq->n; i++) {
		/* 0x010 bit5 打开 IV 4PA，实测会让 HiFi DSP 抛 SOCHIFI_S_EXCEPTION，
		 * 所以默认跳过；运行时用 `echo mask 1 > /proc/moli_ctl` 才放行。 */
		if (seq->ops[i].reg == 0x010 && !(moli_pathev_mask & 0x1))
			continue;
		last = snd_soc_component_update_bits(c, seq->ops[i].reg,
						     seq->ops[i].mask,
						     on ? seq->ops[i].mask : 0);
	}

	pr_info("MOLI_DAPMEV: %s %s ops=%d last=%d\n", w->name,
		on ? "on" : "off", seq->n, last);
	return 0;
}

static struct snd_soc_dapm_widget *moli_prepare_widgets(void)
{
	struct snd_soc_dapm_widget *w;
	int i, n = 0;

	w = kmemdup(asp_widgets_gen, sizeof(asp_widgets_gen), GFP_KERNEL);
	if (!w)
		return NULL;

	for (i = 0; i < (int)ARRAY_SIZE(asp_widgets_gen); i++) {
		if (!moli_find_seq(w[i].name))
			continue;
		w[i].event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD;
		w[i].event = moli_dapm_event;
		n++;
	}
	pr_info("MOLI_DAPMEV: attached %d/%d widgets\n", n,
		(int)ARRAY_SIZE(asp_widgets_gen));
	return w;
}

static int asp_codec_codecless_component_probe(struct snd_soc_component *component)
{
	int ret;

	pr_info("%s: component probe\n", __func__);

	/* MOLI_PROC */
	moli_status_len = 0;
	proc_create("moli_audio", 0644, NULL, &moli_audio_pops);
	proc_create("moli_ctl", 0222, NULL, &moli_ctl_pops);
	proc_create("moli_ctls", 0444, NULL, &moli_ctls_pops);

	/* MOLI_CLK: 先上电/开时钟 */
	moli_card = component->card;

	codecless_enable_resources(component->dev);

	/* MOLI_PINCTRL: 把 I2S/ASP 引脚切到工作态 */
	codecless_pinctrl_init(component->dev);

	/* MOLI_SEQ: 映射 asp_cfg 区后执行初始化序列 */
	/* MOLI_DMAREG: 映射 ASP DMA 控制器 */
	codecless_dma_base = devm_ioremap(component->dev, CODECLESS_DMA_BASE,
					  CODECLESS_DMA_BYTES);
	pr_info("%s: MOLI_DMAREG dma_base=%pK\n", __func__, codecless_dma_base);

	codecless_cfg_base = devm_ioremap(component->dev, CODECLESS_CFG_BASE,
					  CODECLESS_CFG_BYTES);
	if (codecless_cfg_base) {
		codecless_cfg_regmap = devm_regmap_init_mmio(component->dev,
						codecless_cfg_base,
						&codecless_cfg_regmap_cfg);
		if (IS_ERR(codecless_cfg_regmap)) {
			pr_warn("MOLI_SEQ: cfg regmap err %ld\n",
				PTR_ERR(codecless_cfg_regmap));
			codecless_cfg_regmap = NULL;
		}
	} else {
		pr_warn("MOLI_SEQ: cfg ioremap failed\n");
	}

	codecless_base = devm_ioremap(component->dev, CODECLESS_REG_BASE,
				      CODECLESS_REG_BYTES);
	if (!codecless_base) {
		pr_warn("%s: ioremap 0x%lx failed, run without regmap\n",
			__func__, CODECLESS_REG_BASE);
		return 0;   /* 软失败：不影响声卡注册 */
	}
	pr_info("%s: ioremap ok, base=%pK\n", __func__, codecless_base);

	codecless_regmap = devm_regmap_init_mmio(component->dev, codecless_base,
						 &codecless_regmap_cfg);
	if (IS_ERR(codecless_regmap)) {
		pr_warn("%s: regmap init failed %ld\n", __func__,
			PTR_ERR(codecless_regmap));
		codecless_regmap = NULL;
		return 0;
	}

	snd_soc_component_init_regmap(component, codecless_regmap);
	pr_info("%s: regmap ready 0x%lx-0x%lx\n", __func__,
		CODECLESS_REG_BASE, CODECLESS_REG_BASE + CODECLESS_REG_MAX);

	/* 读几个寄存器证明可访问 */
	pr_info("%s: reg[0x00]=0x%08x reg[0x04]=0x%08x reg[0x28c]=0x%08x\n",
		__func__,
		snd_soc_component_read32(component, 0x00),
		snd_soc_component_read32(component, 0x04),
		snd_soc_component_read32(component, CODECLESS_REG_MAX));

#ifdef CONFIG_SND_SOC_CODEC_DEBUG
	moli_log("clk_subsys=%s clk_codec=%s vdd=%s\n",
		 codecless_clk_subsys ? "ok" : "fail",
		 codecless_clk_codec ? "ok" : "fail",
		 codecless_vdd ? "ok" : "fail");

	/* MOLI_DAPM: 注册原厂提取的 widget 与 route，补上 DAPM 连接图 */
	{
		int r1, r2;

		moli_widgets_ev = moli_prepare_widgets();
		r1 = snd_soc_dapm_new_controls(&component->dapm,
				moli_widgets_ev ? moli_widgets_ev : asp_widgets_gen,
				ARRAY_SIZE(asp_widgets_gen));
		/* MOLI_ROUTES_LOADBEARING: 这里的注册是承重的。
		 * 实测（r9）：只保留机器驱动的 card->dapm_routes、
		 * 去掉这一句后，播放完全起不来（PCM 一直 closed、
		 * 0 个 DAPM 事件、HAL 日志为空）。恢复后立刻恢复正常。
		 * 因此保留双注册：卡片级 + 组件级。
		 */
		if (component->card)
			r2 = snd_soc_dapm_add_routes(&component->card->dapm,
						     asp_routes_gen,
						     ARRAY_SIZE(asp_routes_gen));
		else
			r2 = snd_soc_dapm_add_routes(&component->dapm,
						     asp_routes_gen,
						     ARRAY_SIZE(asp_routes_gen));
		/* MOLI_DAPMSYNC: 原厂 probe 里会 dapm_power_widgets()，
		 * 公开等价的入口是 snd_soc_dapm_sync()。
		 */
		if (r1 == 0 && r2 == 0) {
			int rs = snd_soc_dapm_sync(&component->dapm);

			moli_log("dapm: sync=%d\n", rs);
			pr_info("MOLI_DAPMSYNC: sync ret=%d\n", rs);
		}
		moli_log("dapm: add_controls=%d add_routes=%d (w=%d r=%d)\n",
			 r1, r2, (int)ARRAY_SIZE(asp_widgets_gen),
			 (int)ARRAY_SIZE(asp_routes_gen));
		pr_info("%s: MOLI_DAPM add_controls=%d add_routes=%d\n",
			__func__, r1, r2);
	}

	moli_run_seq(component);

	ret = hicodec_debug_init(component, &codecless_dump_info);
	pr_info("%s: hicodec_debug_init ret=%d (/proc/audio)\n", __func__, ret);
#endif
	return 0;
}

static void asp_codec_codecless_component_remove(struct snd_soc_component *component)
{
#ifdef CONFIG_SND_SOC_CODEC_DEBUG
	hicodec_debug_uninit(component);
#endif
	pr_info("%s: component remove\n", __func__);
}

static const struct snd_soc_component_driver asp_codec_codecless_component = {
	.name           = ASP_CODEC_CODECLESS_DRV_NAME,
	.probe          = asp_codec_codecless_component_probe,
	.remove         = asp_codec_codecless_component_remove,
	.controls       = codecless_virt_controls,
	.num_controls   = ARRAY_SIZE(codecless_virt_controls),
};

static int asp_codec_codecless_dai_probe(struct snd_soc_dai *dai)
{
	pr_info("%s: dai %s probed\n", __func__, dai->name);
	return 0;
}

/* MOLI_DAI_MATCH: 与原厂 asp_codec_dai 逐字段一致（反汇编得到）
 *   capture.stream_name  = "Capture",  formats 0x0000000100000444, rates 0x7FFE, ch 1..8
 *   playback.stream_name = "Playback", formats 0x0000000100000444, rates 0x7FFE, ch 2..2
 * 没有 stream_name 时 ASoC 不会为 codec 组件创建流端 widget，
 * snd_soc_dapm_stream_event 就无法把 DAC/ADC widget 标成 active。
 */
#define MOLI_DAI_FORMATS	0x0000000100000444ULL	/* S16_LE|S24_LE|S24_3LE|S32_LE */
#define MOLI_DAI_RATES		0x00007FFE		/* 8000..384000 */

static struct snd_soc_dai_driver asp_codec_codecless_dais[] = {
	{
		.name = ASP_CODEC_CODECLESS_DAI_NAME,
		.playback = {
			.stream_name = "Playback",
			.formats = MOLI_DAI_FORMATS,
			.rates = MOLI_DAI_RATES,
			.channels_min = 2,
			.channels_max = 2,
		},
		.capture = {
			.stream_name = "Capture",
			.formats = MOLI_DAI_FORMATS,
			.rates = MOLI_DAI_RATES,
			.channels_min = 1,
			.channels_max = 8,
		},
		.ops = &asp_codec_codecless_dai_ops,
		.probe = asp_codec_codecless_dai_probe,
	},
};

static int asp_codec_codecless_probe(struct platform_device *pdev)
{
	int ret;

	pr_info("%s: probe\n", __func__);

	dev_set_name(&pdev->dev, ASP_CODEC_CODECLESS_DRV_NAME);

	ret = devm_snd_soc_register_component(&pdev->dev,
					      &asp_codec_codecless_component,
					      asp_codec_codecless_dais,
					      ARRAY_SIZE(asp_codec_codecless_dais));
	if (ret != 0) {
		pr_err("%s: register component failed %d\n", __func__, ret);
		return ret;
	}

	pr_info("%s: codec %s registered with %d controls (%d int + %d enum)\n",
		__func__, ASP_CODEC_CODECLESS_DRV_NAME, VIRT_CTL_TOTAL,
		VIRT_INT_CNT, VIRT_ENUM_CNT);
	return 0;
}

static int asp_codec_codecless_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id asp_codec_codecless_dt_match[] = {
	{ .compatible = "hisilicon,asp-codec-codecless" },
	{}
};
MODULE_DEVICE_TABLE(of, asp_codec_codecless_dt_match);

static struct platform_driver asp_codec_codecless_driver = {
	.probe  = asp_codec_codecless_probe,
	.remove = asp_codec_codecless_remove,
	.driver = {
		.name = ASP_CODEC_CODECLESS_DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = asp_codec_codecless_dt_match,
	},
};

static int __init asp_codec_codecless_init(void)
{
	return platform_driver_register(&asp_codec_codecless_driver);
}
module_init(asp_codec_codecless_init);

static void __exit asp_codec_codecless_exit(void)
{
	platform_driver_unregister(&asp_codec_codecless_driver);
}
module_exit(asp_codec_codecless_exit);

MODULE_DESCRIPTION("ASoC ASP codec driver for code-less mode");
MODULE_LICENSE("GPL");

/* ==================== MOLI_PATHEV ====================
 * 数据来源：/root/stock163.elf 反汇编
 *   iv_4pa_power_event          @0xffffffc011cc4850
 *   audio_2pa_dn_power_event    @0xffffffc011cc3ac8
 *   audio_4pa_dn_power_event    @0xffffffc011cc3db8
 *   codec3_dn_r_power_event     @0xffffffc011cc4310
 *   s1_tx_tdm_power_event       @0xffffffc011cc36e8
 *   s1_tx_tdm256_power_event    @0xffffffc011cc3a00
 *   dacl_filter_pga_event       @0xffffffc011cc8024
 *   dacr_filter_pga_event       @0xffffffc011cc80fc
 * 已验证的语义：event==1 (PRE_PMU) 置位；event==8 (POST_PMD) 清零；
 * 两个分支写的是同一组 (reg,mask)，只有 val 不同。
 * 寄存器号 0x52xxx = codec 块（reg&0xFFF，窗口 0xFA552000，<=0x28C）。
 */
struct moli_bitop {
	u16 reg;
	u32 mask;
};

/* ASP_CODEC_IV_4PA_SWITCH：扬声器 4PA 主路径 */
static const struct moli_bitop moli_seq_iv4pa[] = {
	{ 0x00C, 0x00000040 },
	{ 0x00C, 0x00020000 },
	{ 0x00C, 0x00040000 },
	{ 0x00C, 0x00080000 },
	{ 0x00C, 0x00100000 },
	{ 0x0C8, 0x00AA0000 },
	{ 0x0C8, 0x00550000 },
	{ 0x004, 0x00000010 },
	{ 0x004, 0x00000020 },
};

/* AUDIO_2PA_DN_SWITCH：下行 smartpa 两路 */
static const struct moli_bitop moli_seq_2pa_dn[] = {
	{ 0x004, 0x00000040 },
	{ 0x004, 0x00000080 },
};

/* I2S1_TX_TDM_SWITCH：I2S1 TX TDM 输出 */
static const struct moli_bitop moli_seq_i2s1_tdm[] = {
	{ 0x01C, 0x00020000 },
	{ 0x018, 0x00000080 },
	{ 0x270, 0x80000000 },
};

static void moli_run_bits(struct snd_soc_component *c,
			  const struct moli_bitop *t, int n, unsigned int on)
{
	int i, r;

	for (i = 0; i < n; i++) {
		r = snd_soc_component_update_bits(c, t[i].reg, t[i].mask,
						  on ? t[i].mask : 0);
		pr_info("MOLI_PATHEV: reg 0x%03X mask 0x%08X on=%u ret=%d\n",
			t[i].reg, t[i].mask, on, r);
	}
}

static void moli_path_event(struct snd_soc_component *c,
			    unsigned int idx, unsigned int on)
{
	if (!c)
		return;

	pr_info("MOLI_PATHEV: idx=%u on=%u\n", idx, on);

	switch (idx) {
	case 25:	/* ASP_CODEC_IV_4PA_SWITCH */
		if (moli_pathev_mask & 0x1)
			moli_run_bits(c, moli_seq_iv4pa,
				      ARRAY_SIZE(moli_seq_iv4pa), on);
		break;
	case 74:	/* AUDIO_2PA_DN_SWITCH */
		if (moli_pathev_mask & 0x2)
			moli_run_bits(c, moli_seq_2pa_dn,
				      ARRAY_SIZE(moli_seq_2pa_dn), on);
		break;
	case 256:	/* I2S1_TX_TDM_SWITCH */
		if (moli_pathev_mask & 0x4)
			moli_run_bits(c, moli_seq_i2s1_tdm,
				      ARRAY_SIZE(moli_seq_i2s1_tdm), on);
		break;
	default:
		break;
	}
}
