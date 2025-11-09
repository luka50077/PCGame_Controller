#include "led.hpp"

#include <algorithm>

#include "keyboard.hpp"

void LED::init(TIM_HandleTypeDef *htim, uint32_t channel, LED_Side which_side) {
  m_htim = htim;
  m_channel = channel;
  m_which_side = which_side;
}

void LED::setLED(int LEDnum, LED_Color_t data) {
  LED_Data[LEDnum][0] = LEDnum;
  LED_Data[LEDnum][1] = data.Green;
  LED_Data[LEDnum][2] = data.Red;
  LED_Data[LEDnum][3] = data.Blue;
}

void LED::send() {
  uint32_t indx = 0;
  uint32_t color;

  for (int i = 0; i < LED_NUM; i++) {
    color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | (LED_Data[i][3]));
    for (int i = 23; i >= 0; i--) {
      if (color & (1 << i)) {
        pwmData[indx] = (htim3.Init.Period * 2) / 3;
      } else {
        pwmData[indx] = (htim3.Init.Period) / 3;
      }
      indx++;
    }
  }

  for (int i = 0; i < 50; i++) {
    pwmData[indx] = 0;
    indx++;
  }
  HAL_TIM_PWM_Start_DMA(m_htim, m_channel, (uint32_t *)pwmData, indx);
}

void LED::set_color(LED_Color_t *dst, Mode m_mode) {
  LED_Color_t src[10];
  switch (m_mode) {
    case Mode::OFF:
      std::copy(std::begin(LED_COLOR_OFF), std::end(LED_COLOR_OFF), src);
      break;
    case Mode::DIVA_Play:
      std::copy(std::begin(LED_COLOR_DIVA_PLAY), std::end(LED_COLOR_DIVA_PLAY),
                src);
      break;
    case Mode::KALPA_4k:
      std::copy(std::begin(LED_COLOR_KALPA_4K), std::end(LED_COLOR_KALPA_4K),
                src);
      break;
    case Mode::KALPA_5k_RIGHT:
      std::copy(std::begin(LED_COLOR_KALPA_5K_RIGHT),
                std::end(LED_COLOR_KALPA_5K_RIGHT), src);
      break;
    case Mode::KALPA_5k_LEFT:
      std::copy(std::begin(LED_COLOR_KALPA_5K_LEFT),
                std::end(LED_COLOR_KALPA_5K_LEFT), src);
      break;
    case Mode::DIVA_Select:
      std::copy(std::begin(LED_COLOR_DIVA_SELECT),
                std::end(LED_COLOR_DIVA_SELECT), src);
      break;
    case Mode::KALPA_Select:
      std::copy(std::begin(LED_COLOR_KALPA_SELECT),
                std::end(LED_COLOR_KALPA_SELECT), src);
      break;
    default:
      std::copy(std::begin(LED_COLOR_OFF), std::end(LED_COLOR_OFF), src);
      break;
  }
  int offset = m_which_side == LED_Side::LEFT ? 0 : 5;
  std::copy(src + offset, src + offset + 5, dst);
}

void LED::update(Mode m_mode) {
  LED_Color_t send_data[LED_NUM] = {LED_BLUE, LED_YELLOW, LED_RED, LED_GREEN,
                                    LED_CYAN};
  set_color(send_data, m_mode);
  for (int i = 0; i < LED_NUM; i++) {
    setLED(i, send_data[i]);
  }
  send();
}

void LED::test() {
  for (int i = 0; i < LED_NUM; i++) {
    LED_Color_t color;
    color.Red = (i % 3 == 0) ? 48 : 0;
    color.Green = (i % 3 == 1) ? 48 : 0;
    color.Blue = (i % 3 == 2) ? 48 : 0;
    setLED(i, color);
    send();
    HAL_Delay(500);
  }
  send();
}