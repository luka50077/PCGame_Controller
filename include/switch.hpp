#pragma once
#include <algorithm>
#include <array>

#include "gpio.h"
#include "main.h"

constexpr int SWITCH_NUM = 10;

class Switch {
 private:
  const int SW_DELAY = 10;
  std::array<int, SWITCH_NUM> cnt = {};
  const std::array<uint16_t, SWITCH_NUM> gpio_pins = {
      Escape_Pin,     SW2_Pin, SW1_Pin, SW0_Pin, SpaceL_Pin,
      Encoder_SW_Pin, SW3_Pin, SW4_Pin, SW5_Pin, SpaceR_Pin};
  const std::array<GPIO_TypeDef*, SWITCH_NUM> gpio_ports = {
      Escape_GPIO_Port, SW2_GPIO_Port,        SW1_GPIO_Port, SW0_GPIO_Port,
      SpaceL_GPIO_Port, Encoder_SW_GPIO_Port, SW3_GPIO_Port, SW4_GPIO_Port,
      SW5_GPIO_Port,    SpaceR_GPIO_Port};

 public:
  std::array<bool, SWITCH_NUM> state = {};
  void read();

  void getState(bool out_state[]) {
    std::copy(state.begin(), state.end(), out_state);
  }
};

extern Switch sw;