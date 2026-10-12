#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_led_state(const struct device *dev, bool on);

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H */