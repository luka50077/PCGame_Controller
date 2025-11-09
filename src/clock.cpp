#include "keyboard.hpp"
extern "C" {
#include "stm32f4xx_it.h"
#include "tim.h"
}

Keyboard keyboard;

void cycle_callback_start() {
  HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim6) {  // 500us
    keyboard.update();
  }
}
