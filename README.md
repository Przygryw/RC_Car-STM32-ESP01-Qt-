# RC_Car
### Radio-Controlled car with dedicated Qt Application
### My prototype is based on chasis of Fiat Grande Punto.
### Model has been created by me using Blender 3D and printed on Ender 3.
### Don't be scared of how it looks like in the photos, it's just a prototype!
## Materials
### PETG- body
### PLA- chassis, mechanical elements, battery socket, lamps, wheels/rims

## Overview:
## V0.9
- For now car has functioning steering (SG90), Drivetraing (DC Motor), lights (LEDs). If you need direct info about used pins, check "Schematics.png" file. To details about each element of set, so Qt application and STM Baremetal code, check poprer folder in this repo.
- Communication is monodirectonal, it's only commputer, which sends data and dosesn't receive anything back.
- To communitacte, car uses ESP01 set to AccessPoint, with TCP server set on it
  - Connect turn car on and browse WiFi networks, until you find "Punto" (default), put password "Condon67" (note, "67" was there before trend had shown up!)
  - Start deditated Qt application and log into the car
    - Defaults **IP:** *192.168.5.7*, **Port:** *80*,
  - Have Fun!
