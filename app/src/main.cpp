#include <errno.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    struct sensor_value val;
    bool led_is_on = false;

    if (!device_is_ready(dev)) {
        LOG_ERR("LED sensor device is not ready");
        return -ENODEV;
    }

    while (1) {
        int ret;

        if (led_is_on) {
            ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
            if (ret == 0) {
                led_is_on = false;
                LOG_INF("LED turned off");
            } else {
                LOG_ERR("sensor_channel_get failed: %d", ret);
            }
        } else {
            ret = sensor_sample_fetch(dev);
            if (ret == 0) {
                led_is_on = true;
                LOG_INF("LED turned on");
            } else {
                LOG_ERR("sensor_sample_fetch failed: %d", ret);
            }
        }

        k_msleep(SLEEP_TIME_MS);
    }
}
