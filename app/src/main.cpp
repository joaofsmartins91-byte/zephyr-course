#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "app_led" alias. */
#define LED_NODE DT_ALIAS(led2)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
	void test() {
		const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
		struct sensor_value val;
		int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
		LOG_INF("Channel ret: %d", ret);
	}
}

int main(void)
{
	test();
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
