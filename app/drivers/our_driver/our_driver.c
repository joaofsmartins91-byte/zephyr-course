#include <errno.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config {
    struct gpio_dt_spec led;
};

static const struct our_driver_config config = {
    .led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
};

static int channel_get_my_impl(const struct device *dev,
                   enum sensor_channel chan,
                   struct sensor_value *val)
{
    const struct our_driver_config *cfg = dev->config;

    if (chan != SENSOR_CHAN_ALL) {
        return -ENOTSUP;
    }

    int ret = gpio_pin_set_dt(&cfg->led, 0);
    if (ret < 0) {
        return ret;
    }

    val->val1 = 0;
    val->val2 = 0;
    LOG_INF("LED turned off by sensor_channel_get");
    return 0;
}

static int channel_fetch_my_impl(const struct device *dev, enum sensor_channel chan)
{
    const struct our_driver_config *cfg = dev->config;

    ARG_UNUSED(chan);
    int ret = gpio_pin_set_dt(&cfg->led, 1);
    if (ret < 0) {
        return ret;
    }

    LOG_INF("LED turned on by sensor_sample_fetch");
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
	.sample_fetch = channel_fetch_my_impl,
    .channel_get = channel_get_my_impl,
};

static int init(const struct device *dev)
{
    const struct our_driver_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED GPIO is not ready");
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, &config, POST_KERNEL, 80,
              &api_iomico_lecture);
