#include <linux/uinput.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    const char *name;
    void (*exec)(void);
} Action;

// === Action implementations ===

static void act_voice_assist() {
    system("am start -a android.intent.action.VOICE_COMMAND </dev/null >/dev/null 2>&1");
}

static void act_camera() {
    system("am start -a android.media.action.IMAGE_CAPTURE </dev/null >/dev/null 2>&1");
}

static void act_flashlight() {
    // Toggle flashlight via system properties
    system("su -c 'echo 1 > /sys/class/leds/flashlight/brightness 2>/dev/null; "
           "input keyevent 265 2>/dev/null' </dev/null >/dev/null 2>&1");
    // Fallback: use Android's flashlight tile via statusbar command
    system("su -c 'cmd statusbar click-tile com.android.systemui/com.oplus.systemui.statusbar.notification.keymagicservice.KeyFlashlightService 2>/dev/null' </dev/null >/dev/null 2>&1");
}

static void act_screenshot() {
    system("screencap -p /sdcard/Pictures/Screenshots/sc_$(date +%s).png </dev/null >/dev/null 2>&1");
}

static void act_ring_mode() {
    // Toggle between normal and vibrate
    system("input keyevent 164 </dev/null >/dev/null 2>&1"); // VOLUME_MUTE
}

static void act_no_disturb() {
    system("su -c 'settings put global zen_mode 1' </dev/null >/dev/null 2>&1");
}

static void act_recording() {
    system("am start -n com.coloros.soundrecorder/com.soundrecorder.browsefile.BrowseFile </dev/null >/dev/null 2>&1");
}

static void act_translate() {
    system("am start -n com.coloros.translate/com.coloros.translate.ui.TranslateActivity </dev/null >/dev/null 2>&1");
}

static void act_flash_memory() {
    system("am start -n com.coloros.colordirectservice/com.oplus.directservice.flashnotes.business.service.DataCollectService </dev/null >/dev/null 2>&1");
}

static void act_control_center() {
    // Expand notification panel
    system("su -c 'cmd statusbar expand-panel' </dev/null >/dev/null 2>&1");
}

static void act_nothing() {
    // Do nothing
}

static const Action actions[] = {
    {"voice_assist",  act_voice_assist},
    {"camera",        act_camera},
    {"flash_light",   act_flashlight},
    {"screen_shot",   act_screenshot},
    {"ring_mode",     act_ring_mode},
    {"no_disturb",    act_no_disturb},
    {"recording",     act_recording},
    {"translate",     act_translate},
    {"flash_memory",  act_flash_memory},
    {"controlcenter", act_control_center},
    {"nothing",       act_nothing},
    {NULL, NULL}
};

static const Action* find_action(const char *name) {
    for (int i = 0; actions[i].name; i++)
        if (strcmp(actions[i].name, name) == 0) return &actions[i];
    return NULL;
}

// === Minimal uinput injector for haptic feedback ===
static void inject_key(int keycode, int duration_ms) {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) return;
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    for (int i = 0; i < 768; i++) ioctl(fd, UI_SET_KEYBIT, i);
    struct uinput_setup us;
    memset(&us, 0, sizeof(us));
    us.id.bustype = BUS_VIRTUAL;
    us.id.vendor = 0x1234;
    us.id.product = 0x5678;
    strcpy(us.name, "action-btn");
    ioctl(fd, UI_DEV_SETUP, &us);
    ioctl(fd, UI_DEV_CREATE);
    usleep(50000);
    struct input_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = EV_KEY; ev.code = keycode; ev.value = 1;
    write(fd, &ev, sizeof(ev));
    ev.type = EV_SYN; ev.code = SYN_REPORT; ev.value = 0;
    write(fd, &ev, sizeof(ev));
    usleep(duration_ms * 1000);
    ev.type = EV_KEY; ev.code = keycode; ev.value = 0;
    write(fd, &ev, sizeof(ev));
    ev.type = EV_SYN; ev.code = SYN_REPORT; ev.value = 0;
    write(fd, &ev, sizeof(ev));
    usleep(50000);
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

static void print_usage() {
    fprintf(stderr, "Usage: action_button [short|long] [inject] [k=KEYCODE]\n");
    fprintf(stderr, "Available actions:\n");
    for (int i = 0; actions[i].name; i++)
        fprintf(stderr, "  %s\n", actions[i].name);
}

int main(int argc, char **argv) {
    const char *press = "short";
    int also_inject = 0;
    int keycode = 582;
    int duration = 100;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return 0;
        }
        if (strcmp(argv[i], "short") == 0) { press = "short"; duration = 100; }
        else if (strcmp(argv[i], "long") == 0) { press = "long"; duration = 800; }
        else if (strcmp(argv[i], "inject") == 0) also_inject = 1;
        else if (strncmp(argv[i], "k=", 2) == 0) keycode = atoi(argv[i] + 2);
    }

    // Read config
    char short_action[64] = "voice_assist";
    char long_action[64] = "camera";
    FILE *cfg = fopen("/data/local/tmp/action_button.conf", "r");
    if (cfg) {
        char line[128];
        while (fgets(line, sizeof(line), cfg)) {
            char k[32], v[64];
            if (sscanf(line, "%31[^=]=%63s", k, v) == 2) {
                if (strcmp(k, "short") == 0) strncpy(short_action, v, 63);
                if (strcmp(k, "long") == 0) strncpy(long_action, v, 63);
            }
        }
        fclose(cfg);
    }

    // Optional: inject uinput key for haptic
    if (also_inject) inject_key(keycode, duration);

    // Execute
    const char *action_name = (strcmp(press, "long") == 0) ? long_action : short_action;
    const Action *a = find_action(action_name);
    if (!a) {
        fprintf(stderr, "Unknown action: %s\n", action_name);
        print_usage();
        return 1;
    }
    a->exec();
    return 0;
}
