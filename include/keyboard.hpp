#pragma once
#include "encoder.hpp"
#include "keycode_defs.hpp"
#include "led.hpp"
#include "stm32f4xx_hal.h"
#include "switch.hpp"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

enum class Mode : int {
  OFF,
  DIVA_Select,
  DIVA_Play,
  KALPA_Select,
  KALPA_4k,
  KALPA_5k_RIGHT,
  KALPA_5k_LEFT,
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
  const uint8_t key_map_DIVA_Play[SWITCH_NUM] = {0x0D, 0x0E, 0x0F,
                                                 0x0C, 0x18, 0x12};  // wip

  const uint8_t key_map_KALPA_Select[SWITCH_NUM] = {
      KEYCODE_ESCAPE,  KEYCODE_Down, KEYCODE_Left, KEYCODE_SHIFT_L,
      KEYCODE_R,       KEYCODE_TAB,  KEYCODE_Up,   KEYCODE_Right,
      KEYCODE_SHIFT_R, KEYCODE_ENTER};
  const uint8_t key_map_KALPA_4k[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D,        KEYCODE_S, KEYCODE_RESERVED,
      KEYCODE_RESERVED, KEYCODE_RESERVED, KEYCODE_K, KEYCODE_L,
      KEYCODE_RESERVED, KEYCODE_RESERVED};
  const uint8_t key_map_KALPA_5k_RIGHT[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D,        KEYCODE_S, KEYCODE_RESERVED,
      KEYCODE_RESERVED, KEYCODE_RESERVED, KEYCODE_J, KEYCODE_K,
      KEYCODE_L,        KEYCODE_RESERVED};
  const uint8_t key_map_KALPA_5k_LEFT[SWITCH_NUM] = {
      KEYCODE_ESCAPE,   KEYCODE_D,        KEYCODE_S, KEYCODE_A,
      KEYCODE_RESERVED, KEYCODE_RESERVED, KEYCODE_K, KEYCODE_L,
      KEYCODE_RESERVED, KEYCODE_RESERVED};

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

  const uint8_t* getKeymap(const int16_t enc_value);

  Mode getMode() const {
    return current_mode;
  }

  void sendReport();
};

extern Keyboard keyboard;