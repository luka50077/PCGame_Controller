#pragma once
#include <stdint.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

#include "main.h"
#include "tim.h"

enum class Mode : int;

constexpr int LED_NUM = 5;

class LED {
 private:
  struct LED_Color_t {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
  };

  TIM_HandleTypeDef *m_htim;
  uint32_t m_channel;
  const int USE_BRIGHTNESS = 1;
  uint8_t LED_Data[LED_NUM][4];
  uint16_t pwmData[24 * LED_NUM + 60];

 protected:
  enum class LED_Side : int { LEFT = 0, RIGHT = 1 };
  LED_Side m_which_side = LED_Side::LEFT;

  const LED_Color_t LED_OFF = {0, 0, 0};
  const LED_Color_t LED_RED = {72, 0, 0};
  const LED_Color_t LED_GREEN = {0, 72, 0};
  const LED_Color_t LED_BLUE = {0, 0, 72};
  const LED_Color_t LED_YELLOW = {48, 48, 0};
  const LED_Color_t LED_CYAN = {0, 48, 48};
  const LED_Color_t LED_MAGENTA = {72, 0, 36};
  const LED_Color_t LED_WHITE = {45, 45, 45};
  const LED_Color_t LED_ORANGE = {72, 24, 0};
  const std::array<LED_Color_t, 10> LED_COLOR_OFF = {
      LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF,
      LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF};
  const std::array<LED_Color_t, 10> LED_COLOR_DIVA_PLAY = {
      LED_CYAN, LED_MAGENTA, LED_GREEN, LED_YELLOW, LED_WHITE,
      LED_OFF,  LED_BLUE,    LED_RED,   LED_YELLOW, LED_WHITE};
  const std::array<LED_Color_t, 10> LED_COLOR_KALPA_4K = {
      LED_CYAN, LED_WHITE, LED_WHITE, LED_OFF, LED_OFF,
      LED_OFF,  LED_WHITE, LED_WHITE, LED_OFF, LED_OFF};
  const std::array<LED_Color_t, 10> LED_COLOR_KALPA_5K_RIGHT = {
      LED_CYAN, LED_WHITE, LED_WHITE, LED_OFF,   LED_OFF,
      LED_OFF,  LED_WHITE, LED_WHITE, LED_WHITE, LED_OFF};
  const std::array<LED_Color_t, 10> LED_COLOR_KALPA_5K_LEFT = {
      LED_CYAN, LED_WHITE, LED_WHITE, LED_WHITE, LED_OFF,
      LED_OFF,  LED_WHITE, LED_WHITE, LED_OFF,   LED_OFF};
  const std::array<LED_Color_t, 10> LED_COLOR_DIVA_SELECT = {
      LED_CYAN, LED_RED,  LED_GREEN,  LED_WHITE, LED_ORANGE,
      LED_OFF,  LED_BLUE, LED_YELLOW, LED_WHITE, LED_CYAN};
  const std::array<LED_Color_t, 10> LED_COLOR_KALPA_SELECT = {
      LED_CYAN,    LED_RED,  LED_GREEN,  LED_WHITE, LED_CYAN,
      LED_MAGENTA, LED_BLUE, LED_YELLOW, LED_WHITE, LED_CYAN};
  const std::array<LED_Color_t, 10> LED_COLOR_BMS = {
      LED_CYAN, LED_WHITE, LED_WHITE, LED_WHITE, LED_BLUE,
      LED_OFF, LED_WHITE, LED_BLUE, LED_RED, LED_BLUE};

 public:
  void init(TIM_HandleTypeDef *htim, uint32_t channel, LED_Side which_side);
  void setLED(int LEDnum, LED_Color_t data);
  void send();
  void update(Mode m_mode);
  void test();
  void set_color(LED_Color_t *dst, Mode m_mode);
};

extern LED led0;
extern LED led1;