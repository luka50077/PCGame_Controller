# PCGame Controller
This is the firmware for a custom PC game controller designed for rhythm games". The controller features programmable LED lighting and can emulate keyboard inputs to interact with the games.

## Article
- [Part1: Design and Circuits](https://xenon01.hatenablog.com/entry/2025/11/02/163902)
- [Part2: Soldering and Firmware](https://xenon01.hatenablog.com/entry/2025/11/13/143734)
- [Part3: Under Construnction]

## Compile
```bash
$ mkdir build && cd build
$ cmake ..
$ make
```
To flash the compiled firmware onto the STM32 microcontroller, use STM32CubeProgrammer or you can use the following command:
```bash
$ make write
```
Note that STM32CubeProgrammer should be installed in the default path (Not tested on macOS).

## Customization
You can customize...
- the key mappings for different modes by editing the `keyboard.hpp` file.
- the LED lighting patterns by editing the `led.hpp` file.

## Note
To change the MCU settings, use STM32CubeMX to open the `controller.ioc` file.

## References
- LED
    - ControllersTech, [Interface WS2812 with STM32](https://controllerstech.com/interface-ws2812-with-stm32/)
- Keyboard
    - ControllersTech, [How to use STM32 as a KEYBOARD](https://controllerstech.com/how-to-use-stm32-as-a-keyboard/)
    - 東京工業大学 ロボット技術研究会公式ブログ, [STM32でマウスとキーボードを作る](https://blog.rogiken.org/blog/2018/12/20/stm32%E3%81%A7%E3%83%9E%E3%82%A6%E3%82%B9%E3%81%A8%E3%82%AD%E3%83%BC%E3%83%9C%E3%83%BC%E3%83%89%E3%82%92%E4%BD%9C%E3%82%8B/)