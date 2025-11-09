#include "keyboard.hpp"

#include <cstring>

LED led0;
LED led1;
Switch sw;
Encoder encoder;

void Keyboard::init() {
  while (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) {
    current_mode = Mode::OFF;
  }
  current_mode = Mode::KALPA_Select;

  led0.LED::init(&htim3, TIM_CHANNEL_1, LED::LED_Side::LEFT);
  led1.LED::init(&htim4, TIM_CHANNEL_3, LED::LED_Side::RIGHT);
  encoder.init();
}

const uint8_t* Keyboard::getKeymap(const int16_t enc_value) {
  Mode mode = static_cast<Mode>(enc_value % static_cast<int>(Mode::Count));
  switch (mode) {
    case Mode::OFF:
      return key_map_off;
    case Mode::DIVA_Select:
      return key_map_DIVA_Select;
    case Mode::DIVA_Play:
      return key_map_DIVA_Play;
    case Mode::KALPA_Select:
      return key_map_KALPA_Select;
    case Mode::KALPA_4k:
      return key_map_KALPA_4k;
    case Mode::KALPA_5k_RIGHT:
      return key_map_KALPA_5k_RIGHT;
    case Mode::KALPA_5k_LEFT:
      return key_map_KALPA_5k_LEFT;
    default:
      return key_map_off;
  }
}

void Keyboard::update() {
  static long cnt = 0;
  cnt++;
  sw.Switch::read();
  if (cnt % 50 == 0) {  // every 25ms
    led0.LED::update(keyboard.getMode());
    encoder.update();
    setMode(static_cast<Mode>((encoder.get_value()) %
                              static_cast<int>(Mode::Count)));
  } else if (cnt % 50 == 25) {
    led1.LED::update(keyboard.getMode());
  }
}

void Keyboard::sendReport() {
  if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return;

  const uint8_t* key_map = getKeymap(encoder.get_value());

  static keyboardHID_t prev_report = {0};
  keyboardHID_t new_report = {0};

  int key_reg_cnt = 0;
  for (int i = 0; i < SWITCH_NUM; i++) {
    if (sw.state[i]) {
      uint8_t keycode = key_map[i];
      if (keycode == KEYCODE_SHIFT_L) {
        new_report.modifiers |= HID_MOD_LSHIFT;
        continue;
      } else if (keycode == KEYCODE_SHIFT_R) {
        new_report.modifiers |= HID_MOD_RSHIFT;
        continue;
      }
      new_report.key[key_reg_cnt] = key_map[i];
      key_reg_cnt++;
      if (key_reg_cnt >= 6) break;  // max 6 keys
    }
  }

  // send report only when there is a change
  if (std::memcmp(&prev_report, &new_report, sizeof(keyboardHID_t)) != 0) {
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&new_report,
                        sizeof(keyboardHID_t));
    prev_report = new_report;
  }
}
