#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static int channel_get_my_impl(const struct device *dev,
                  enum sensor_channel chan,
                  struct sensor_value *val) {
    LOG_INF("Hello,channel_get_my_impl called with channel %d", chan);
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = channel_get_my_impl,
};

// Init fm
static int init() {
    LOG_INF("Device init called");
    return 0;
};

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);
