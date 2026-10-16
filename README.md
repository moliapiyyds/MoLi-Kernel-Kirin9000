# MoLi Kernel · HUAWEI Kirin 9000 · HarmonyOS 3.0

[![Build](https://img.shields.io/badge/build-Clang%2012.0.5%20%2B%20GCC%204.9-blue)]()
[![Kernel](https://img.shields.io/badge/kernel-5.10.43-green)]()
[![SukiSU](https://img.shields.io/badge/SukiSU--Ultra-v4.2.1%20(40994)-orange)]()
[![SUSFS](https://img.shields.io/badge/SUSFS-v2.3.0-red)]()
[![Platform](https://img.shields.io/badge/build%20host-openEuler%2024.03%20LTS--SP3-purple)]()

> **HUAWEI Kirin 9000 通用内核** —— 适配 HarmonyOS 3.0，机型 **NOH-AN00 / NOH-AN01**（Mate 40 Pro）
> 内核侧集成 **SukiSU-Ultra v4.2.1 + SUSFS v2.3.0**，在 **openEuler 24.03 (LTS-SP3)** 上以 **Android Clang 12.0.5 + GCC 4.9** 全量编译

---

## 内核信息

| 项目 | 内容 |
|---|---|
| **内核名称（`uname -r`）** | `5.10.43-MoLi-HarmonyOS3.0-Clang12.0.5-openEuler24.03-LTS-SP3` |
| **编译时间** | 2026-10-16 |
| **编译作者** | 茉莉 　QQ：1265274322 |
| **内核版本** | 5.10.43 |
| **适用设备** | HUAWEI Kirin 9000 · **NOH-AN00 / NOH-AN01**（Mate 40 Pro） |
| **支持系统** | **HarmonyOS 3.0**（主版本是 3 即可，不区分小版本号） |
| **编译系统** | **openEuler 24.03 (LTS-SP3)** |
| **编译工具链版本** | **Android Clang 12.0.5（r416183b）+ GCC 4.9（aarch64 / arm32）** |
| **打包格式** | **ak3 + img** |

---

## 集成组件

- **SukiSU-Ultra `v4.2.1`** — 内核侧 version code `40994`、`uapi 5`，与官方最新管理器完全对齐，不会再出现「需要更新内核」
- **SUSFS `v2.3.0`** — 内核侧文件 + 树侧 **21 个挂点文件全部对齐官方实现**（含 statx / SUS_KSTAT、inotify fdinfo 伪装、srcu open-redirect、clone_mnt 竞态处理等）
- **ksud `4.2.1-2-gdfae9237 (uapi: 5)`** — 与内核、管理器三者同版本
- **内置去更新模块**（`moli_deupdate`）— 通过 hosts 拦截华为系统更新域名

---

## 功能特性

1. **内核态 Root：SukiSU-Ultra v4.2.1** — `KERNEL_SU_UAPI_VERSION = 5`，version code `40994`，与官方管理器版本算法和门限完全对齐，管理器直接进入「工作中」
2. **SUSFS v2.3.0 完整挂载** — 内核侧 + KSU 侧 glue 与官方实现全量对齐；树侧覆盖 `fs/statfs.c`（`statfs_by_dentry_wrapper` / `calculate_f_flags_wrapper`）、`fs/proc/fd.c`、`fs/proc/task_mmu.c`（srcu 版 open-redirect）、`fs/stat.c`（statx 机制）等 21 个挂点文件
3. **内核 ABI 严格对齐** — `ksu_handle_faccessat` / `ksu_handle_stat` 采用 `getname_flags()` + `filename_lookup()` 约定，传递内核态 `struct filename **`，ksud 启动零 Oops
4. **SUS_KSTAT 全字段伪装** — `susfs_sus_kstat_spoof_generic_fillattr()` 携带 `result_mask`，statx 伪装完整生效
5. **授权持久化（重启不丢）** — 授权名单落盘于 `/metadata/ksu/.allowlist`（ext4 未加密分区），并由管理器上下文同步写入，天然绕开 FBE 场景下 `/data` 的 `-ENOKEY` 限制
6. **管理器冷启动即识别** — 开机完成执行全量扫描 + `GET_INFO` 自助加冕，重启后管理器直接显示「工作中」，无需重装、无需再次授权
7. **内核标识统一** — `uname -r`、KSU 全名、AK3 安装脚本统一为 **MoLi Kernel** 标识体系
8. **基带与驱动适配完整** — Balong modem（`CONFIG_HISI_BALONG_MODEM` 等）、华为各驱动与 SELinux 策略按原厂配置保留
9. **SELinux 强制模式**、**root 正常**（`uid=0`，`u:r:ksu:s0`）
10. **兼容各版本管理器** — 内核 `KSU_VERSION=40994`，SukiSU-Ultra（门限 32513）、ReSukiSU（门限 35002）等管理器的版本门限均满足

---

## 下载

见本仓库 **Releases** 页面。每次发布提供：

| 文件 | 用途 |
|---|---|
| `MoLi-Kernel-Kirin9000-HarmonyOS3-v4.2.1-YYYYMMDD.img` | **fastboot 直接刷入**（推荐） |
| `MoLi-Kernel-Kirin9000-HarmonyOS3-v4.2.1-YYYYMMDD-ak3.zip` | **AnyKernel3**，自定义 Recovery 里刷入 |
| `sha256sums.txt` | 两个包的 SHA-256 校验 |

---

## 刷入方法

### 方式 A：fastboot（推荐）

```bash
# 1. 确认 BL 已解锁
fastboot oem get-bootinfo      # 必须显示 unlocked

# 2. 建议先备份当前 boot
adb shell su -c "dd if=/dev/block/by-name/boot of=/sdcard/boot_backup.img"
adb pull /sdcard/boot_backup.img

# 3. 刷入
fastboot flash boot MoLi-Kernel-Kirin9000-HarmonyOS3-v4.2.1-YYYYMMDD.img
fastboot reboot
```

### 方式 B：AnyKernel3

在支持 AK3 的自定义 Recovery 中直接刷入 zip 即可（脚本会自动 dump/repack boot）。

### 刷入后确认

```bash
adb shell uname -r
# 5.10.43-MoLi-HarmonyOS3.0-Clang12.0.5-openEuler24.03-LTS-SP3
adb shell su -c "id"
# uid=0(root) ... context=u:r:ksu:s0
```

---

## 回滚 / 异常恢复

- 刷入前务必**备份 boot 分区**（上面第 2 步）；出问题用备份镜像刷回即可
- 也可用同机型同版本的**原厂 `BOOT.img`** 直接刷回：
  ```bash
  fastboot flash boot BOOT.img
  ```
- 本内核刷入过程**全程不会触碰 BL 回锁**，每次刷写前后都会核对 `fastboot oem get-bootinfo` 为 `unlocked`

---

## 实测验证（本版）

| 项目 | 结果 |
|---|---|
| 启动 | 正常，无 panic / 无 KSU-SUSFS 相关 WARNING |
| root | `uid=0(root) context=u:r:ksu:s0` |
| SUSFS | `ksud susfs status` → `true`；9 项特性全开 |
| 管理器 | SukiSU-Ultra v4.2.1：「工作中」，版本 `40994-5`，无需重新授权 |
| 授权持久化 | 授权后**重启仍保留**（shell / MT 管理器实测） |
| 模块挂载 | `moli_deupdate` 生效，`/system/etc/hosts` 已拦截更新域名 |
| SELinux | Enforcing |

---

## 作者

- **作者：茉莉**
- **QQ：1265274322**
- **官方 Q 群：570387739**

有问题欢迎群里讨论。转发请注明来自本仓库。
