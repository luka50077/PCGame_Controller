#pragma once
#include "encoder.hpp"
#include "keycode_defs.hpp"
#include "led.hpp"
#include "stm32f4xx_hal.h"
#include "switch.hpp"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

enum class Mode : int {
  DIVA_Select,
  DIVA_Play,
  KALPA_Select,
  KALPA_4k,
  KALPA_5k_RIGHT,
  KALPA_5k_LEFT,
  OFF,
  Count
};

class Keyboard : public LED, public Switch, public Encoder {
 private:
  struct keyboardHID_t {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t key[6];
  };

  const uint8_t key_map_off[SWITCH_NUM] = {0};
  const uint8_t key_map_DIVA_Select[SWITCH_NUM] = {
      KEYCODE_ESCAPE,  KEYCODE_Down, KEYCODE_Left, KEYCODE_SHIFT_L,
      KEYCODE_R,       KEYCODE_TAB,  KEYCODE_Up,   KEYCODE_Right,
      KEYCODE_SHIFT_R, KEYCODE_ENTER};
  uint8_t key_map_DIVA_Play[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_V, KEYCODE_C, KEYCODE_X, KEYCODE_RESERVED,
      KEYCODE_RESERVED, KEYCODE_B, KEYCODE_N, KEYCODE_M, KEYCODE_RESERVED};

  const uint8_t key_map_KALPA_Select[SWITCH_NUM] = {
      KEYCODE_ESCAPE,  KEYCODE_Down, KEYCODE_Left, KEYCODE_SHIFT_L,
      KEYCODE_R,       KEYCODE_TAB,  KEYCODE_Up,   KEYCODE_Right,
      KEYCODE_SHIFT_R, KEYCODE_ENTER};
  const uint8_t key_map_KALPA_4k[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D, KEYCODE_S, KEYCODE_RESERVED, KEYCODE_Down,
      KEYCODE_RESERVED, KEYCODE_K, KEYCODE_L, KEYCODE_RESERVED, KEYCODE_ENTER};
  const uint8_t key_map_KALPA_5k_RIGHT[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D, KEYCODE_S, KEYCODE_RESERVED, KEYCODE_Down,
      KEYCODE_RESERVED, KEYCODE_J, KEYCODE_K, KEYCODE_L,        KEYCODE_ENTER};
  const uint8_t key_map_KALPA_5k_LEFT[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D, KEYCODE_S, KEYCODE_A,        KEYCODE_Down,
      KEYCODE_RESERVED, KEYCODE_K, KEYCODE_L, KEYCODE_RESERVED, KEYCODE_ENTER};

  Mode current_mode = Mode::OFF;
  struct keyboardHID_t keyboard_report = {
      .modifiers = 0,
      .reserved = 0,
  };

 public:
  void init();
  void update();

  void setMode(Mode mode) {
    current_mode = mode;
  }

  void setDIVAReservedKeys(uint8_t left = KEYCODE_RESERVED,
                           uint8_t right = KEYCODE_RESERVED) {
    key_map_DIVA_Play[4] = left;
    key_map_DIVA_Play[9] = right;
  }

  const uint8_t* getKeymap(const int16_t enc_value);

  Mode getMode() const {
    return current_mode;
  }

  void sendReport();
};

extern Keyboard keyboard;