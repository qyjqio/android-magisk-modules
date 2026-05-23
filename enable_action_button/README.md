# OPPO Action Button with Virtual Injector

Virtual action button for OPPO devices without physical action button hardware.
**Short press is synced with OPPO Settings UI** — change it in Settings and it updates automatically.

## Architecture

```
Xposed Edge gesture
    ↓
/system/bin/action_button short|long    (shell wrapper)
    ↓
Reads OPPO Settings.System              (linked with Settings UI!)
    ↓
/system/bin/action_button_exec <action> (C executor)
    ↓
Executes action directly
```

## Usage

```bash
action_button short    # Short press — reads from OPPO Settings
action_button long     # Long press — reads from config file
```

## Configuration

Edit `/data/local/tmp/action_button.conf`:

```
long=flash_light      # Long press action only
```

Short press is ALWAYS from OPPO Settings (`oplus_action_button_switch_state`), not configurable in this file.

## Available actions

| Name | Action |
|------|--------|
| voice_assist | Breeno voice assistant |
| camera | Open camera |
| flash_light | Toggle flashlight |
| screen_shot | Take screenshot |
| ring_mode | Toggle ring/silent |
| no_disturb | Toggle DND mode |
| recording | Open sound recorder |
| translate | Open translator |
| flash_memory | Flash notes |
| controlcenter | Expand quick settings |
| nothing | Do nothing |

## Integration with Xposed Edge

Bind gestures to shell commands:
- Short press: `su -c action_button short`
- Long press: `su -c action_button long`

## How it works

1. Sets `ro.oplus.key.actionbutton=1` to unlock OPPO action button settings UI
2. `action_button` wrapper reads current selection from `Settings.System oplus_action_button_switch_state`
3. `action_button_exec` performs the action directly, bypassing OPPO framework

## Build

```bash
arm-linux-gnueabihf-gcc -static -Os -o action_button_exec action_button_v3.c
```

## Credits

- Original module: 抖音：车机研究所_草软
- Virtual injector + Settings linkage: army
