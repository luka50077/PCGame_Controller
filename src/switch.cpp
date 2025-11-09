#include "switch.hpp"

void Switch::read() {
  for (int i = 0; i < SWITCH_NUM; i++) {
    if (cnt[i] >= SW_DELAY) {
      cnt[i] = SW_DELAY;
      state[i] =
          (HAL_GPIO_ReadPin(gpio_ports[i], gpio_pins[i]) == GPIO_PIN_RESET);
    } else {
      if (HAL_GPIO_ReadPin(gpio_ports[i], gpio_pins[i]) == GPIO_PIN_RESET) {
        cnt[i]++;
      } else {
        cnt[i] = 0;
      }
    }
  }
}
