# Android Magisk Modules

> Author: **草软**

自用 Magisk 模块合集，解决国内 Android 设备的常见限制问题。

## 模块列表

### 🔵 SIM Country Spoof `v1.0`

**绕过 TikTok / 区域限制应用**

Telephony 服务会持续根据物理 SIM 卡设置运营商属性。本模块每 10 秒检测一次，当检测到中国 MCC（460xx）时自动覆写为美国 T-Mobile（310260）。

| 检测项 | 伪装值 |
|--------|--------|
| `gsm.sim.operator.numeric` | 310260 |
| `gsm.operator.numeric` | 310260 |
| `gsm.sim.operator.iso-country` | us |

**使用方法**：
1. 下载 [sim-country-spoof-v1.0.zip](release/sim-country-spoof-v1.0.zip)
2. Magisk 中刷入，重启
3. 验证：`getprop gsm.sim.operator.numeric` → `310260`
4. 清除目标应用数据后打开

**注意**：不影响实际通话/短信，仅改变应用读取到的值。需配合代理（Clash/VPN）使用。

## 设备兼容

- ✅ Android 14+ (Magisk 27+)
- ✅ ColorOS / OxygenOS / OnePlus
- ✅ 其他 AOSP 系 ROM

## 许可

仅供个人学习研究使用。
