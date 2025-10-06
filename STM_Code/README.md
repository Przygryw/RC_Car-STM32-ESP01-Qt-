# This folder contains whole STMCubeIDE project with code and .c file itself.
## Project uses STM32F103C8T6 as main controller and ESP01 to communicate with computer's WiFi. In coding progress, I was using TTL connected to laptop's USB port and RealTerm to read how system responds to operations made on it (such as feedback given by ESP01) .Initial settings of project such as: main timer frequency, clock source, debug, has been set using IDE's GUI. Project has been flashed to "BluePill" using STLink V2.

### How it works?
  1. Code sets peripherials and deffault parameters' values.
  2. Code receives data from Qt application once laptop is connected to RC Car's network and application is connected to ESP01.
  3. Code decodes received communication frame
  4. Code sets each RC car's parameter
  5. Code sends debug info via debug's USART
  6. Goes back to point 2.

### Communication frame: R0L0S000A000/
- R0 - informs in car is going forward or reverse (R0 - forward/R1- reverse NOTE! motor wiring should be connected correctrly for given effect) 
- L0 - inform if lihgts are on of off (L0 - lights off/L1- lights on)
- S000- value of speed, then converted into PWM signal, send to L293D bridge. (**S000** no PWM/**S100**- Full speed/**S050**- 50% of power)
- A00- value of stering angle, then converted into PWM and send to SG90 (in my case: **A000**- maximum right/**A090**- wheels pointing forwards/**A180**- wheels maximum left)
- \ end of transmitted frame

### Fucntions
- **setDebug()**- funtion sets USART2 (pins PA1 PA2) as debug. Helpfull in debuging progess, to receive info via USB port to computer about what's happening inside system.
- **debugSend(char * message)**- function sends message given by the user via USART2. Used as the debug for ex. to show AT repsonses received from ESP01.
- **analyzeString(char * message)**- function analyzes received AT respones from ESP01 - not used yet
- **wifiSend(char * message)**- function sends message constructed as array of characters to ESP01 connected to USART3. -
- **wifisetReceive(char * message, int size)**- function used to receive feedback info from ESP01
- **QtReceive(char * buffer)**- function reads info sent from dedicated Qt application via WiFi and saves it into the buffer.
  - Function waits till received character is ':', that means, that +IPD has passed and now it's time to receive main transmission frame.
  - Function reads characters and saves it into the buffer, until '\' is received (end of transmission)
  - Buffer is displayed in debug and then returned by pointer, to main function
- **setWifi()**- sets USART3 (pins PB10 PB11) to communicate with ESP01. 
  - Function sets clocks, pins and UART peripherial according to reference manual, then using HAL_Delay, forces 1 second break.
  - Function sends commands to ESP01
    - AT+CWMODE=2 -sets ESP01 as Acces Point
    - AT+CWSAP="PUNTO","Condon67",5,3- sets network'parameters to **name:** "Punto", **password:** "Condon67", **channel 5**, **WPA2_PSK protection**
    - AT+CIPAP="192.168.5.7- sets static network's IP to 192.168.5.7
    - AT+CIPMUX=1- Access point permits multi connections
    - AT+CIPSERVER=1,80- starts TCP server on port 80
    - AT+CWDHCP=0,1- sets DHCP
  - *Each time command is send to ESP01, then MCU waits 1 second and waits for response, which is send send to debug bus*
- **decodeFrame(char * frame, char * angle, int * speed, char * lights, char * movement)**- function decodes data received from Qt to each RC Car parameter, then sends it to main() by pointers
- **setControl()**- function sets pins (check Schematics.png file)
- **lightsMode(char mode)**- turns on and off lights according to decoded frame
- **setAngle(char angleValue)**- converts angle value into PWM signal according to SG90 parameters
- **setSpeed(char direction, int speedValue)**- converts speed value to PWM (PWM L293D pin) signal and set if car decides if goes forward or reverse (PA9 PA10 pins).  
