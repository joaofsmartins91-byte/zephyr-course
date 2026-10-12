#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <errno.h>
#include "our_driver.h"

static int cmd_channel_sample_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret != 0) {
        shell_error(sh, "Failed to sample fetch: %d", ret);
        return -EFAULT;
    }

    shell_info(sh, "Sample fetched from %s", dev->name);

    return 0;
}

static int cmd_channel_get_handler(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

    if (ret != 0) {
        shell_error(sh, "Failed to get channel value: %d", ret);
        return -EFAULT;
    }

    shell_info(sh, "Channel value: %d", val.val1);

    return 0;
}

static int cmd_channel_info(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    shell_info(sh, "Device: %s", dev->name);
    shell_info(sh, "Ready: %s", device_is_ready(dev) ? "Yes" : "No");

    return 0;
}

static int cmd_channel_set_handler(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 3) {
        shell_error(sh, "Missing value; expected 0 (off) or 1 (on)");
        return -EINVAL;
    }
    
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    bool on;

    if (strcmp(argv[2], "0") == 0) {
        on = false;
    } else if (strcmp(argv[2], "1") == 0) {
        on = true;
    } else {
        shell_error(sh, "Invalid value '%s'; expected 0 or 1", argv[2]);
        return -EINVAL;
    }

    int ret = our_driver_set_led_state(dev, on);
    if (ret < 0) {
        shell_error(sh, "Failed to set LED state: %s", argv[2]);
        return -EFAULT;
    }

    shell_info(sh, "LED turned %s", on ? "on" : "off");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "Fetch a sensor sample", cmd_channel_sample_fetch, 2, 0),
    SHELL_CMD_ARG(read, NULL, "Get channel value", cmd_channel_get_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Get device information", cmd_channel_info, 2, 0),
    SHELL_CMD_ARG(set, NULL, "Set LED state: 0=off, 1=on", cmd_channel_set_handler, 2, 1),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &our_driver_subcmd, "Our driver commands", NULL);
