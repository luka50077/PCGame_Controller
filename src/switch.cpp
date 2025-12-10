#include "switch.hpp"

void Switch::read() {
  for (int i = 0; i < SWITCH_NUM; i++) {
    bool pressed = (HAL_GPIO_ReadPin(gpio_ports[i], gpio_pins[i]) == GPIO_PIN_RESET);

    if (pressed && !state[i]) {
      oneshot_state[i] = true;
      oneshot_cnt[i] = ONE_SHOT_TIME;
    }
    state[i] = pressed;
    if (oneshot_cnt[i] > 0) {
      oneshot_cnt[i]--;
      if (oneshot_cnt[i] == 0) {
        oneshot_state[i] = false;
      }
    }
  }
}