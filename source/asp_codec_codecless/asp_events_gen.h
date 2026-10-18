/* 自动生成，勿手改：asp_events_gen.h
 * 来源：/root/stock163.elf 反汇编（re/gen_events.py）
 * 内容：40 个 widget 的 DAPM 电源事件寄存器序列
 * 语义：PRE_PMU(event=1) 置位 mask；POST_PMD(event=8) 清零 mask。
 * 已剔除实测会让 DSP 抛 SOCHIFI_S_EXCEPTION 的 (0x10,0x20)。
 */
#ifndef _ASP_EVENTS_GEN_H
#define _ASP_EVENTS_GEN_H

struct moli_ev_op {
	u16 reg;		/* codec 块偏移（0x00..0x28C） */
	u32 mask;
};

struct moli_ev_seq {
	const char *widget;
	const struct moli_ev_op *ops;
	int n;
};

static const struct moli_ev_op moli_ops_adc1[] = {
	{ 0x008, 0x00000040 },
	{ 0x004, 0x00000001 },
};
static const struct moli_ev_op moli_ops_adc2[] = {
	{ 0x008, 0x00000080 },
	{ 0x004, 0x00000002 },
};
static const struct moli_ev_op moli_ops_adc3[] = {
	{ 0x008, 0x00000100 },
	{ 0x004, 0x00000004 },
};
static const struct moli_ev_op moli_ops_adc4[] = {
	{ 0x008, 0x00000200 },
	{ 0x004, 0x00000008 },
};
static const struct moli_ev_op moli_ops_adc5[] = {
	{ 0x008, 0x00000400 },
	{ 0x00C, 0x00200000 },
};
static const struct moli_ev_op moli_ops_adc6[] = {
	{ 0x0C8, 0x00000100 },
	{ 0x0C8, 0x00000001 },
};
static const struct moli_ev_op moli_ops_adc7[] = {
	{ 0x0C8, 0x00000002 },
	{ 0x0C8, 0x00000200 },
};
static const struct moli_ev_op moli_ops_adc8[] = {
	{ 0x0C8, 0x00000004 },
	{ 0x0C8, 0x00000400 },
};
static const struct moli_ev_op moli_ops_adc_spai1_filter[] = {
	{ 0x0CC, 0x00000100 },
	{ 0x0CC, 0x00400000 },
};
static const struct moli_ev_op moli_ops_adc_spai2_filter[] = {
	{ 0x0CC, 0x00000020 },
	{ 0x0CC, 0x00004000 },
};
static const struct moli_ev_op moli_ops_adc_spav1_filter[] = {
	{ 0x0CC, 0x00000200 },
};
static const struct moli_ev_op moli_ops_adc_spav2_filter[] = {
	{ 0x0CC, 0x00000040 },
};
static const struct moli_ev_op moli_ops_asp_codec_iv_4pa_switch[] = {
	/* iv_4pa_power_event 反汇编：0x010 |= 0x20，然后 0x010 |= 0x01。
	 * 这两条默认被 moli_dapm_event 跳过（reg==0x010 且 pathev_mask bit0 未开）。 */
	{ 0x010, 0x00000020 },
	{ 0x010, 0x00000001 },
	{ 0x00C, 0x00020000 },
	{ 0x00C, 0x00040000 },
	{ 0x00C, 0x00080000 },
	{ 0x00C, 0x00100000 },
	{ 0x00C, 0x00000040 },
};
static const struct moli_ev_op moli_ops_asp_codec_iv_8pa_switch[] = {
	{ 0x118, 0x00000007 },
	/* MOLI_BLOCK010: { 0x010, 0x00000040 }, 已屏蔽 */
	{ 0x00C, 0x00020000 },
	{ 0x00C, 0x00040000 },
	{ 0x00C, 0x00080000 },
	{ 0x00C, 0x00100000 },
	{ 0x00C, 0x00000004 },
	{ 0x00C, 0x00000008 },
	{ 0x00C, 0x00000010 },
	{ 0x00C, 0x00000020 },
};
static const struct moli_ev_op moli_ops_audio_2pa_dn_switch[] = {
	{ 0x004, 0x00000040 },
	{ 0x004, 0x00000080 },
};
static const struct moli_ev_op moli_ops_audio_4pa_dn_switch[] = {
	{ 0x004, 0x00000040 },
	{ 0x004, 0x00000080 },
	{ 0x004, 0x00000010 },
	{ 0x004, 0x00000020 },
};
static const struct moli_ev_op moli_ops_audio_6mic[] = {
	{ 0x004, 0x08000000 },
	/* MOLI_BLOCK010: { 0x010, 0x00000080 }, 已屏蔽 */
	{ 0x004, 0x10000000 },
	{ 0x004, 0x00000800 },
	{ 0x004, 0x00001000 },
	{ 0x004, 0x00008000 },
	{ 0x004, 0x00010000 },
};
static const struct moli_ev_op moli_ops_audio_8mic[] = {
	{ 0x004, 0x08000000 },
	/* MOLI_BLOCK010: { 0x010, 0x00000080 }, 已屏蔽 */
	{ 0x004, 0x10000000 },
	{ 0x004, 0x00000800 },
	{ 0x004, 0x00001000 },
	{ 0x004, 0x00008000 },
	{ 0x004, 0x00010000 },
};
static const struct moli_ev_op moli_ops_audio_8pa_dn_switch[] = {
	{ 0x004, 0x00000040 },
	{ 0x004, 0x00000080 },
	{ 0x004, 0x00000010 },
	{ 0x004, 0x00000020 },
	{ 0x004, 0x00000100 },
	{ 0x004, 0x00000200 },
	{ 0x004, 0x00080000 },
	{ 0x004, 0x00100000 },
};
static const struct moli_ev_op moli_ops_carkit_48k_switch[] = {
	{ 0x004, 0x00000040 },
	{ 0x004, 0x00000080 },
};
static const struct moli_ev_op moli_ops_codec3_dn_44k1_switch[] = {
	/* MOLI_BLOCK010: { 0x010, 0x00000002 }, 已屏蔽 */
};
static const struct moli_ev_op moli_ops_codec3_dn_r_switch[] = {
	{ 0x004, 0x00000020 },
};
static const struct moli_ev_op moli_ops_dacl_filter[] = {
	{ 0x0C8, 0x00AA0000 },
};
static const struct moli_ev_op moli_ops_dacr_filter[] = {
	{ 0x0C8, 0x00550000 },
};
static const struct moli_ev_op moli_ops_i2s1_tx_i2s_switch[] = {
	{ 0x01C, 0x00020000 },
};
static const struct moli_ev_op moli_ops_i2s1_tx_tdm256_switch[] = {
	{ 0x01C, 0x00020000 },
	{ 0x270, 0x10000000 },
};
static const struct moli_ev_op moli_ops_i2s1_tx_tdm_switch[] = {
	{ 0x01C, 0x00020000 },
	{ 0x018, 0x00000080 },
};
static const struct moli_ev_op moli_ops_iv_2pa_48k_switch[] = {
	{ 0x00C, 0x00020000 },
	{ 0x00C, 0x00080000 },
};
static const struct moli_ev_op moli_ops_mic[] = {
	{ 0x0C8, 0x00000800 },
	{ 0x0C8, 0x00001000 },
	{ 0x0C8, 0x00002000 },
	{ 0x0C8, 0x00004000 },
};
static const struct moli_ev_op moli_ops_mic34[] = {
	{ 0x004, 0x00008000 },
	{ 0x004, 0x00010000 },
};
static const struct moli_ev_op moli_ops_s2_rx_l_src[] = {
	{ 0x008, 0x00020000 },
	{ 0x008, 0x20000000 },
};
static const struct moli_ev_op moli_ops_s2_rx_r_src[] = {
	{ 0x008, 0x00040000 },
	{ 0x008, 0x40000000 },
};
static const struct moli_ev_op moli_ops_s2_tx_l_srcdn[] = {
	{ 0x00C, 0x00000001 },
};
static const struct moli_ev_op moli_ops_s2_tx_r_srcdn[] = {
	{ 0x00C, 0x00000002 },
};
static const struct moli_ev_op moli_ops_sidetone_pga[] = {
	{ 0x008, 0x00000020 },
};
static const struct moli_ev_op moli_ops_spa1_i_up_fifo_switch[] = {
	{ 0x0CC, 0x00002000 },
};
static const struct moli_ev_op moli_ops_spa1_v_up_fifo_switch[] = {
	{ 0x0CC, 0x00001000 },
};
static const struct moli_ev_op moli_ops_spa2_i_up_fifo_switch[] = {
	{ 0x0CC, 0x00000800 },
};
static const struct moli_ev_op moli_ops_spa2_v_up_fifo_switch[] = {
	{ 0x0CC, 0x00000400 },
};
static const struct moli_ev_op moli_ops_voice_8mic[] = {
	{ 0x004, 0x20000000 },
	/* MOLI_BLOCK010: { 0x010, 0x00000080 }, 已屏蔽 */
	{ 0x004, 0x40000000 },
	{ 0x004, 0x00002000 },
	{ 0x004, 0x00004000 },
	{ 0x004, 0x00008000 },
	{ 0x004, 0x00010000 },
};

static const struct moli_ev_seq moli_ev_seqs[] = {
	{ "ADC1", moli_ops_adc1, ARRAY_SIZE(moli_ops_adc1) },
	{ "ADC2", moli_ops_adc2, ARRAY_SIZE(moli_ops_adc2) },
	{ "ADC3", moli_ops_adc3, ARRAY_SIZE(moli_ops_adc3) },
	{ "ADC4", moli_ops_adc4, ARRAY_SIZE(moli_ops_adc4) },
	{ "ADC5", moli_ops_adc5, ARRAY_SIZE(moli_ops_adc5) },
	{ "ADC6", moli_ops_adc6, ARRAY_SIZE(moli_ops_adc6) },
	{ "ADC7", moli_ops_adc7, ARRAY_SIZE(moli_ops_adc7) },
	{ "ADC8", moli_ops_adc8, ARRAY_SIZE(moli_ops_adc8) },
	{ "ADC_SPAI1_FILTER", moli_ops_adc_spai1_filter, ARRAY_SIZE(moli_ops_adc_spai1_filter) },
	{ "ADC_SPAI2_FILTER", moli_ops_adc_spai2_filter, ARRAY_SIZE(moli_ops_adc_spai2_filter) },
	{ "ADC_SPAV1_FILTER", moli_ops_adc_spav1_filter, ARRAY_SIZE(moli_ops_adc_spav1_filter) },
	{ "ADC_SPAV2_FILTER", moli_ops_adc_spav2_filter, ARRAY_SIZE(moli_ops_adc_spav2_filter) },
	{ "ASP_CODEC_IV_4PA_SWITCH", moli_ops_asp_codec_iv_4pa_switch, ARRAY_SIZE(moli_ops_asp_codec_iv_4pa_switch) },
	{ "ASP_CODEC_IV_8PA_SWITCH", moli_ops_asp_codec_iv_8pa_switch, ARRAY_SIZE(moli_ops_asp_codec_iv_8pa_switch) },
	{ "AUDIO_2PA_DN_SWITCH", moli_ops_audio_2pa_dn_switch, ARRAY_SIZE(moli_ops_audio_2pa_dn_switch) },
	{ "AUDIO_4PA_DN_SWITCH", moli_ops_audio_4pa_dn_switch, ARRAY_SIZE(moli_ops_audio_4pa_dn_switch) },
	{ "AUDIO_6MIC", moli_ops_audio_6mic, ARRAY_SIZE(moli_ops_audio_6mic) },
	{ "AUDIO_8MIC", moli_ops_audio_8mic, ARRAY_SIZE(moli_ops_audio_8mic) },
	{ "AUDIO_8PA_DN_SWITCH", moli_ops_audio_8pa_dn_switch, ARRAY_SIZE(moli_ops_audio_8pa_dn_switch) },
	{ "CARKIT_48K_SWITCH", moli_ops_carkit_48k_switch, ARRAY_SIZE(moli_ops_carkit_48k_switch) },
	{ "CODEC3_DN_44K1_SWITCH", moli_ops_codec3_dn_44k1_switch, ARRAY_SIZE(moli_ops_codec3_dn_44k1_switch) },
	{ "CODEC3_DN_R_SWITCH", moli_ops_codec3_dn_r_switch, ARRAY_SIZE(moli_ops_codec3_dn_r_switch) },
	{ "DACL_FILTER", moli_ops_dacl_filter, ARRAY_SIZE(moli_ops_dacl_filter) },
	{ "DACR_FILTER", moli_ops_dacr_filter, ARRAY_SIZE(moli_ops_dacr_filter) },
	{ "I2S1_TX_I2S_SWITCH", moli_ops_i2s1_tx_i2s_switch, ARRAY_SIZE(moli_ops_i2s1_tx_i2s_switch) },
	{ "I2S1_TX_TDM256_SWITCH", moli_ops_i2s1_tx_tdm256_switch, ARRAY_SIZE(moli_ops_i2s1_tx_tdm256_switch) },
	{ "I2S1_TX_TDM_SWITCH", moli_ops_i2s1_tx_tdm_switch, ARRAY_SIZE(moli_ops_i2s1_tx_tdm_switch) },
	{ "IV_2PA_48K_SWITCH", moli_ops_iv_2pa_48k_switch, ARRAY_SIZE(moli_ops_iv_2pa_48k_switch) },
	{ "MIC", moli_ops_mic, ARRAY_SIZE(moli_ops_mic) },
	{ "MIC34", moli_ops_mic34, ARRAY_SIZE(moli_ops_mic34) },
	{ "S2_RX_L_SRC", moli_ops_s2_rx_l_src, ARRAY_SIZE(moli_ops_s2_rx_l_src) },
	{ "S2_RX_R_SRC", moli_ops_s2_rx_r_src, ARRAY_SIZE(moli_ops_s2_rx_r_src) },
	{ "S2_TX_L_SRCDN", moli_ops_s2_tx_l_srcdn, ARRAY_SIZE(moli_ops_s2_tx_l_srcdn) },
	{ "S2_TX_R_SRCDN", moli_ops_s2_tx_r_srcdn, ARRAY_SIZE(moli_ops_s2_tx_r_srcdn) },
	{ "SIDETONE_PGA", moli_ops_sidetone_pga, ARRAY_SIZE(moli_ops_sidetone_pga) },
	{ "SPA1_I_UP_FIFO_SWITCH", moli_ops_spa1_i_up_fifo_switch, ARRAY_SIZE(moli_ops_spa1_i_up_fifo_switch) },
	{ "SPA1_V_UP_FIFO_SWITCH", moli_ops_spa1_v_up_fifo_switch, ARRAY_SIZE(moli_ops_spa1_v_up_fifo_switch) },
	{ "SPA2_I_UP_FIFO_SWITCH", moli_ops_spa2_i_up_fifo_switch, ARRAY_SIZE(moli_ops_spa2_i_up_fifo_switch) },
	{ "SPA2_V_UP_FIFO_SWITCH", moli_ops_spa2_v_up_fifo_switch, ARRAY_SIZE(moli_ops_spa2_v_up_fifo_switch) },
	{ "VOICE_8MIC", moli_ops_voice_8mic, ARRAY_SIZE(moli_ops_voice_8mic) },
};

#endif /* _ASP_EVENTS_GEN_H */
