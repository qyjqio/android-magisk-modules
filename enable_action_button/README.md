# OPPO Action Button with Virtual Injector

Unlock OPPO action button settings UI and provide a virtual key injector with short/long press support.

## What it does

1. Sets `ro.oplus.key.actionbutton=1` to unlock the action button settings UI
2. Installs `action_button` binary to `/system/bin/`

## Usage

```bash
action_button short    # Execute short press action
action_button long     # Execute long press action
```

## Configuration

Edit `/data/local/tmp/action_button.conf`:

```
short=voice_assist    # Short press action
long=camera           # Long press action
```

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

## Build

```bash
arm-linux-gnueabihf-gcc -static -Os -o action_button action_button_v3.c
```

## Credits

- Original module: 抖音：车机研究所_草软
- Virtual injector: army
