#!/bin/bash
# ASP codec driver - code-less mode
#
# The upstream open-source drop removed this driver's sources while leaving
# CONFIG_SND_SOC_ASP_CODEC_CODECLESS=y in the defconfig. Reconstructed here.

GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
EXTRA_CFLAGS += -I$(srctree)/sound/soc/codecs/vendor/
EXTRA_CFLAGS += -I$(srctree)/sound/soc/codecs/vendor/adapter/
EXTRA_CFLAGS += -I$(srctree)/sound/soc/platforms/common_image/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/

snd-soc-asp-codec-codecless-objs := asp_codec_codecless.o

obj-$(CONFIG_SND_SOC_ASP_CODEC_CODECLESS) += snd-soc-asp-codec-codecless.o
