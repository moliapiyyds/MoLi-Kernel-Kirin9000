/* 自动生成，勿手改：asp_tables_gen.h
 * 来源：/root/stock163.elf（原厂 0.163 内核）反汇编提取
 * 内容：137 controls / 87 widgets / 46 routes
 */
#ifndef _ASP_TABLES_GEN_H
#define _ASP_TABLES_GEN_H

/* ---------- controls: offset = reg - 0x52000 ---------- */
static const struct snd_kcontrol_new asp_controls_gen[] = {
    SOC_SINGLE_EXT("ADC1_CIC_6SEL", 0x28C, 0, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC1_PGA_GAIN", 0x94, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC1_UP_PGA_BYPASS", 0x94, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC2_CIC_6SEL", 0x28C, 1, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC2_PGA_GAIN", 0x98, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC2_UP_PGA_BYPASS", 0x98, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC3_CIC_6SEL", 0x28C, 2, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC3_PGA_GAIN", 0x9C, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC3_UP_PGA_BYPASS", 0x9C, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC4_CIC_6SEL", 0x28C, 3, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC4_PGA_GAIN", 0xA0, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC4_UP_PGA_BYPASS", 0xA0, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC5_CIC_6SEL", 0x28C, 4, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC5_PGA_GAIN", 0xA4, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC5_UP_PGA_BYPASS", 0xA4, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC6_CIC_6SEL", 0x28C, 5, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC6_PGA_GAIN", 0x180, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC6_UP_PGA_BYPASS", 0x180, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC7_CIC_6SEL", 0x28C, 6, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC7_PGA_GAIN", 0x17C, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC7_UP_PGA_BYPASS", 0x17C, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC8_CIC_6SEL", 0x28C, 7, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC8_PGA_GAIN", 0x178, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ADC8_UP_PGA_BYPASS", 0x178, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ASP_CODEC_SIDETONE_PGA_BYPASS", 0x60, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ASP_CODEC_SIDETONE_PGA_GAIN", 0x60, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("AU_DIRECT_LOOP", 0x110, 2, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_BYPASS", 0x1C0, 31, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_DN_CFG", 0x1C0, 0, 15, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_DO_CFG", 0x1C0, 8, 15, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_DYM_EN", 0x1C8, 9, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_EN", 0x1C8, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_FALL_DELAY", 0x1C0, 16, 32767, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_FALL_DELAY_EN", 0x1C0, 4, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_FALL_STEP", 0x1C8, 24, 63, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_PGA_GAIN", 0x1C8, 0, 511, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_L_REG_VCTRL1", 0x1C8, 16, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_BYPASS", 0x1D0, 31, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_DN_CFG", 0x1D0, 0, 15, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_DO_CFG", 0x1D0, 8, 15, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_DYM_EN", 0x1D4, 9, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_EN", 0x1D4, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_FALL_DELAY", 0x1D0, 16, 32767, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_FALL_DELAY_EN", 0x1D0, 4, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_FALL_STEP", 0x1D4, 24, 63, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_PGA_GAIN", 0x1D4, 0, 511, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("CLASSH_R_REG_VCTRL1", 0x1D4, 16, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("DACL_SPA_SRCUP_CLKEN", 0xC8, 17, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("DACL_SPA_SRCUP_MODE", 0x288, 24, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("DACR_SPA_SRCUP_CLKEN", 0xC8, 16, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("DACR_SPA_SRCUP_MODE", 0x288, 28, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_I2S2_RX_L_PGA", 0x124, 0, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_I2S2_RX_R_PGA", 0x124, 3, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_I2S2_TX_MIXER2", 0x134, 0, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_MDM5G_DN_L_PGA", 0x190, 18, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_MDM5G_DN_R_PGA", 0x190, 21, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_MDM_5G_L_DN_AFIFO", 0x11C, 12, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_MDM_5G_R_DN_AFIFO", 0x11C, 15, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("FS_S2_IF_I2S", 0x20, 13, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_CODEC_IO_WORDLENGTH", 0x20, 28, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_FRAME_MODE", 0x20, 24, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_FUNC_MODE", 0x20, 21, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_I2S_DIRECT_MODE", 0x20, 19, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_L_RX_PGA_BYPASS", 0x84, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_CLK_SEL", 0x20, 31, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_PGA_GAIN", 0x84, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCDN_CLKEN", 0x8, 29, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCDN_DIN", 0x12C, 24, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCDN_DOUT", 0x12C, 27, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCDN_MODE", 0xB0, 0, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCUP_CLKEN", 0x8, 17, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCUP_DIN", 0x124, 26, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCUP_DOUT", 0x124, 29, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_L_SRCUP_MODE", 0xA8, 16, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_PGA_GAIN", 0x88, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCDN_CLKEN", 0x8, 30, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCDN_DIN", 0x130, 0, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCDN_DOUT", 0x130, 3, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCDN_MODE", 0xB0, 4, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCUP_CLKEN", 0x8, 18, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCUP_DIN", 0x128, 0, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCUP_DOUT", 0x128, 3, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_RX_R_SRCUP_MODE", 0xA8, 20, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_R_RX_PGA_BYPASS", 0x88, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_CLK_SEL", 0x20, 30, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_L_SRCDN_CLKEN", 0xC, 0, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_L_SRCDN_DIN", 0x130, 6, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_L_SRCDN_DOUT", 0x130, 8, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_L_SRCDN_MODE", 0xB0, 8, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_MIXER2_S1_GAIN", 0xC4, 22, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_MIXER2_S2_GAIN", 0xC4, 24, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_R_SRCDN_CLKEN", 0xC, 1, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_R_SRCDN_DIN", 0x130, 11, 3, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_R_SRCDN_DOUT", 0x130, 13, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("I2S2_TX_R_SRCDN_MODE", 0xB0, 12, 7, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG0_3", 0x1D8, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG12_15", 0x1F0, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG16_19", 0x1F8, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG20_23", 0x200, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG24_27", 0x208, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG28_31", 0x210, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG32_35", 0x218, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG36_39", 0x220, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG40_43", 0x228, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG44_47", 0x230, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG48_51", 0x238, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG4_7", 0x1E0, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG52_55", 0x240, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG56_59", 0x248, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG60_63", 0x250, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG64_67", 0x258, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_L_REG8_11", 0x1E8, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG0_3", 0x1DC, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG12_15", 0x1F4, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG16_19", 0x1FC, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG20_23", 0x204, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG24_27", 0x20C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG28_31", 0x214, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG32_35", 0x21C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG36_39", 0x224, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG40_43", 0x22C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG44_47", 0x234, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG48_51", 0x23C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG4_7", 0x1E4, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG52_55", 0x244, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG56_59", 0x24C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG60_63", 0x254, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG64_67", 0x25C, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("LUT_R_REG8_11", 0x1EC, 0, 2147483647, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("MDM5G_DN_L_PGA_BYPASS", 0x1A4, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("MDM5G_DN_L_PGA_GAIN", 0x1A8, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("MDM5G_DN_R_PGA_BYPASS", 0x1A8, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("MDM5G_DN_R_PGA_GAIN", 0x1A4, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ULTR_DN_L_PGA_BYPASS", 0x280, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ULTR_DN_L_PGA_GAIN", 0x280, 24, 255, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ULTR_DN_R_PGA_BYPASS", 0x284, 10, 1, 0, asp_gen_get, asp_gen_put),
    SOC_SINGLE_EXT("ULTR_DN_R_PGA_GAIN", 0x284, 24, 255, 0, asp_gen_get, asp_gen_put),
};
/* controls 跳过 0 条（寄存器不在 codec 窗口内）*/

/* ---------- widgets ---------- */
static const struct snd_soc_dapm_widget asp_widgets_gen[] = {
    SND_SOC_DAPM_PGA("ADC1", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC1_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("ADC2", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC2_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("ADC3", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC3_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("ADC4", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC4_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("ADC5", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC6", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC7", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC8", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_INPUT("ADC_SPA1_INPUT"),
    SND_SOC_DAPM_INPUT("ADC_SPA2_INPUT"),
    SND_SOC_DAPM_PGA("ADC_SPAI1_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC_SPAI2_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC_SPAV1_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ADC_SPAV2_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("ASP_CODEC_IV_8PA_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("ASP_CODEC_MIC3_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("ASP_CODEC_MIC4_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("AUDIO_8PA_DN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("AUDIO_UP_L_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("AUDIO_UP_R_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("AU_L_EN_MIXER", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=4 未映射 */
    SND_SOC_DAPM_PGA("AU_R_EN_MIXER", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=4 未映射 */
    SND_SOC_DAPM_PGA("BT_TX_SRCDN_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC1_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC2_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC3_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC4_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC5_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC6_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC7_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("CIC_ADC8_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("DACL_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("DACL_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("DACR_FILTER", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("DACR_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_OUTPUT("DAC_SPA1_OUTPUT"),
    SND_SOC_DAPM_OUTPUT("DAC_SPA2_OUTPUT"),
    SND_SOC_DAPM_PGA("DMIC1_IN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("DMIC2_IN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("DMIC3_IN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("DMIC4_IN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_INPUT("I2S2_RX_INPUT"),
    SND_SOC_DAPM_PGA("I2S2_RX_L_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("I2S2_RX_L_SRC_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_RX_L_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_RX_R_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("I2S2_RX_R_SRC_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_RX_R_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_RX_SUPPLY", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=19 未映射 */
    SND_SOC_DAPM_PGA("I2S2_SUPPLY", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=19 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_DRV", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=7 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_L_SRCDN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_L_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_MIXER2", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=4 未映射 */
    SND_SOC_DAPM_OUTPUT("I2S2_TX_OUTPUT"),
    SND_SOC_DAPM_PGA("I2S2_TX_R_SEL_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_R_SRCDN_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("I2S2_TX_R_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_INPUT("I2S2_ULTRA_PLL_TRK_INPUT"),
    SND_SOC_DAPM_OUTPUT("I2S2_ULTRA_PLL_TRK_OUTPUT"),
    SND_SOC_DAPM_PGA("I2S2_ULTRA_PLL_TRK_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_INPUT("MDM5G_DN_FIFO_INPUT"),
    SND_SOC_DAPM_PGA("MDM5G_DN_L_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("MDM5G_DN_R_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_INPUT("PDM_MIC12_INPUT"),
    SND_SOC_DAPM_INPUT("PDM_MIC34_INPUT"),
    SND_SOC_DAPM_INPUT("PDM_MIC56_INPUT"),
    SND_SOC_DAPM_INPUT("PDM_MIC78_INPUT"),
    SND_SOC_DAPM_INPUT("SIDETONE_INPUT"),
    SND_SOC_DAPM_OUTPUT("SIDETONE_OUTPUT"),
    SND_SOC_DAPM_PGA("SIDETONE_PAG_DIN_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("SIDETONE_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("SPA1_I_UP_FIFO_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_OUTPUT("SPA1_UP_FIFO_OUTPUT"),
    SND_SOC_DAPM_PGA("SPA1_V_UP_FIFO_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_PGA("SPA2_I_UP_FIFO_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_OUTPUT("SPA2_UP_FIFO_OUTPUT"),
    SND_SOC_DAPM_PGA("SPA2_V_UP_FIFO_SWITCH", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=15 未映射 */
    SND_SOC_DAPM_INPUT("ULTR_DN_FIFO_INPUT"),
    SND_SOC_DAPM_PGA("UTLR_DN_L_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("UTLR_DN_R_PGA", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("VOICE_UP_L_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
    SND_SOC_DAPM_PGA("VOICE_UP_R_MUX", SND_SOC_NOPM, 0, 0, NULL, 0), /* id=2 未映射 */
};
/* widgets 未映射类型 49 个：ADC1_MUX(2), ADC2_MUX(2), ADC3_MUX(2), ADC4_MUX(2), ASP_CODEC_IV_8PA_SWITCH(15), ASP_CODEC_MIC3_MUX(2), ASP_CODEC_MIC4_MUX(2), AUDIO_8PA_DN_SWITCH(15), AUDIO_UP_L_MUX(2), AUDIO_UP_R_MUX(2) */

/* ---------- routes ---------- */
static const struct snd_soc_dapm_route asp_routes_gen[] = {
    { "AU_L_EN_MIXER", "AU_ADC_SPA1_EN", "ADC_SPA1_INPUT" },
    { "AU_R_EN_MIXER", "AU_ADC_SPA2_EN", "ADC_SPA2_INPUT" },
    { "ADC_SPAI1_FILTER", NULL, "AU_L_EN_MIXER" },
    { "ADC_SPAV1_FILTER", NULL, "AU_L_EN_MIXER" },
    { "ADC_SPAI2_FILTER", NULL, "AU_R_EN_MIXER" },
    { "ADC_SPAV2_FILTER", NULL, "AU_R_EN_MIXER" },
    { "AUDIO_8PA_DN_SWITCH", "ENABLE", "AUDIO_DN_FIFO_INPUT" },
    { "AUDIO_8PA_DN_SWITCH", "ENABLE", "CODEC3_DN_FIFO_INPUT" },
    { "AUDIO_8PA_DN_SWITCH", "ENABLE", "ULTR_DN_FIFO_INPUT" },
    { "AUDIO_8PA_DN_SWITCH", "ENABLE", "MDM5G_DN_FIFO_INPUT" },
    { "ASP_CODEC_IV_8PA_SWITCH", "ENABLE", "I2S1_RX_INPUT" },
    { "AUDIO_DN_L_PGA", NULL, "AUDIO_8PA_DN_SWITCH" },
    { "DMIC1_IN_SWITCH", "ENABLE", "PDM_MIC12_INPUT" },
    { "CIC_ADC1_SWITCH", "ENABLE", "DMIC1_IN_SWITCH" },
    { "CIC_ADC2_SWITCH", "ENABLE", "DMIC1_IN_SWITCH" },
    { "ADC1_MUX", "DMIC1", "CIC_ADC1_SWITCH" },
    { "ADC2_MUX", "DMIC1", "CIC_ADC2_SWITCH" },
    { "ADC1", NULL, "ADC1_MUX" },
    { "ADC2", NULL, "ADC2_MUX" },
    { "AUDIO_UP_L_MUX", "ASP_CODEC_MIC1_ADC", "ADC1" },
    { "AUDIO_UP_R_MUX", "ASP_CODEC_MIC1_ADC", "ADC1" },
    { "VOICE_UP_L_MUX", "ASP_CODEC_MIC1_ADC", "ADC1" },
    { "VOICE_UP_R_MUX", "ASP_CODEC_MIC1_ADC", "ADC1" },
    { "AUDIO_UP_L_MUX", "ASP_CODEC_MIC2_ADC", "ADC2" },
    { "AUDIO_UP_R_MUX", "ASP_CODEC_MIC2_ADC", "ADC2" },
    { "VOICE_UP_L_MUX", "ASP_CODEC_MIC2_ADC", "ADC2" },
    { "VOICE_UP_R_MUX", "ASP_CODEC_MIC2_ADC", "ADC2" },
    { "AUDIO_UP_L_O_SWITCH", "ENABLE", "AUDIO_UP_L_MUX" },
    { "AUDIO_UP_R_O_SWITCH", "ENABLE", "AUDIO_UP_R_MUX" },
    { "VOICE_UP_L_O_SWITCH", "ENABLE", "VOICE_UP_L_MUX" },
    { "VOICE_UP_R_O_SWITCH", "ENABLE", "VOICE_UP_R_MUX" },
    { "AUDIO_FIFO_OUTPUT", NULL, "AUDIO_UP_L_O_SWITCH" },
    { "AUDIO_FIFO_OUTPUT", NULL, "AUDIO_UP_R_O_SWITCH" },
    { "VOICE_FIFO_OUTPUT", NULL, "VOICE_UP_L_O_SWITCH" },
    { "I2S2_TX_MIXER2", "MIXER4_L", "DAC_L_MIXER4" },
    { "I2S2_TX_MIXER2", "MIXER4_R", "DAC_R_MIXER4" },
    { "BT_TX_SRCDN_MUX", "MIXER4_L", "DAC_L_MIXER4" },
    { "I2S2_TX_R_SRCDN_SWITCH", "ENABLE", "DAC_R_MIXER4" },
    { "BT_TX_SRCDN_MUX", "I2S2_MIXER2", "I2S2_TX_MIXER2" },
    { "I2S2_TX_L_SRCDN_SWITCH", "ENABLE", "BT_TX_SRCDN_MUX" },
    { "I2S2_TX_L_SWITCH", "ENABLE", "I2S2_TX_L_SRCDN_SWITCH" },
    { "I2S2_TX_R_SEL_MUX", "I2S2_TX_L_BTMATCH", "I2S2_TX_L_SRCDN_SWITCH" },
    { "SIDETONE_PAG_DIN_MUX", "MIC1_ADC", "ADC1" },
    { "SIDETONE_PAG_DIN_MUX", "MIC2_ADC", "ADC2" },
    { "SIDETONE_PAG_DIN_MUX", "MIC3_ADC", "ADC3" },
    { "SIDETONE_PAG_DIN_MUX", "MIC4_ADC", "ADC4" },
};

#endif /* _ASP_TABLES_GEN_H */
