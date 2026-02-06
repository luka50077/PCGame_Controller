#include "switch.hpp"

void Switch::read() {
  for (int i = 0; i < SWITCH_NUM; i++) {
    state[i] = (HAL_GPIO_ReadPin(gpio_ports[i], gpio_pins[i]) == GPIO_PIN_RESET);
  }
}