#pragma once
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "tim.h"

class Encoder {
 private:
  int16_t enc_value = 100;

 public:
  void init();
  void update();

  int16_t get_value() const {
    return enc_value / 4;  // divide by 4 so that one click changes value by 1
  }
};

extern Encoder encoder;