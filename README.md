# RC_Car
### Radio-Controlled car with dedicated Qt Application
### My prototype is based on chasis of Fiat Grande Punto.
### Model has been created by me using Blender 3D and printed on Ender 3.
### Don't be scared of how it looks like in the photos, it's just a prototype!

## Used manuals:
- [SG90 servo manual](https://www.alldatasheet.pl/datasheet-pdf/download/1572383/ETC/SG90.html)
- [L293D manual](https://www.ti.com/lit/ds/symlink/l293d.pdf?ts=1759741409292&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FL293D)
- [STM32F103C8T6 reference manual](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf&ved=2ahUKEwif4Iuqp5CQAxX0A9sEHY0IHHEQFnoECBcQAQ&usg=AOvVaw0gKEwqSurS9WnLC3KmOqn8)
- [ESP01 Manual](https://academy.cba.mit.edu/classes/networking_communications/ESP8266/esp01.pdf)
- [AT Commands](https://docs.espressif.com/projects/esp-at/en/latest/esp32/AT_Command_Set/Basic_AT_Commands.html)

## Major ToDos!
|Function|Done?|Update|
---------|-----|-------
|Error Checking| | |
|Bidirectional communication| | |
|Code optimizing| | |


## Materials
### PETG- body
### PLA- chassis, mechanical elements, battery socket, lamps, wheels/rims


## Overview:
## V0.9
### Pins:
  |Pin|Function|
  ----|--------
  |PA0|Headlight pin|
  |PA1|Headlight pin|
  |PA2|STM->Debug (USART2) TX pin|
  |PA3|STM->Debug (USART2) RX pin|
  |PA8|Motor PWM (DC motor speed) Pin|
  |PA9|Direction pin of drivetrain|
  |PA10|Direction pin of drivetrain|
  |PB1|SG90 Servo PWM pin|
  |PB10|STM->ESP01 TX Pin|
  |PB11|STM->ESP01 RX Pin|
- For now car has functioning steering (SG90), Drivetraing (DC Motor), lights (LEDs). If you need direct info about used pins, check "Schematics.png" file. To details about each element of set, so Qt application and STM Baremetal code, check poprer folder in this repo.
- Communication is monodirectonal, it's only commputer, which sends data and dosesn't receive anything back.
- To communitacte, car uses ESP01 set to AccessPoint, with TCP server set on it
  - Connect turn car on and browse WiFi networks, until you find "Punto" (default), put password "Condon67" (note, "67" was there before trend had shown up!)
  - Start deditated Qt application and log into the car
    - Defaults **IP:** *192.168.5.7*, **Port:** *80*,
  - Have Fun!
