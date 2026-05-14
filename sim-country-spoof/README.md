# SIM Country Spoof

> Author: 草软

Magisk module to fake SIM operator country for bypassing regional restrictions (e.g., TikTok in China).

## How it works

Telephony service continuously sets SIM operator props based on the physical SIM card. This module runs a loop that fights back — whenever it detects Chinese MCC (460xx), it overwrites with US T-Mobile (310260).

## Installation

1. Copy `sim-country-spoof` folder to `/data/adb/modules/`
2. Reboot
3. Verify: `getprop gsm.sim.operator.numeric` should show `310260`

## Supported countries to spoof

Modify the MCC codes in `service.sh`:

| Country | MCC |
|---------|-----|
| US (T-Mobile) | 310260 |
| Japan | 44010 |
| South Korea | 45005 |
| Singapore | 52501 |

## Note

- Does NOT affect actual phone calls/SMS — only changes what apps see
- Works with Clash/VPN for apps that also check IP location
- May need to clear app data of target app after installation
