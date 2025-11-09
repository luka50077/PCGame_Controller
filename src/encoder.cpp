#include "encoder.hpp"

void Encoder::init() {
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  TIM1->CNT = 0;
}

void Encoder::update() {
  uint16_t enc_buff = TIM1->CNT;
  TIM1->CNT = 0;
  enc_value += static_cast<int16_t>(enc_buff);
}