# stm32f4-heater

## Purpose
This is a PoC application of a Heating Element System that Controls in a closed
loop a heating component. 

A PID controller is used to determine the heating element control signal 
having first read the current temperature.

In order to control the component a zero crossing approach is used to set the
durations open of a TRIAC.


## Notice
This is a PoC implementation, in a real applications various parts of the hardware/software
should be altered to include a better solution.

## Schematic
![schematic](./doc/stm32l4-heater.png)

## License
Everything that is related to hardware in this repository is licensed under the CERN-OHL-S-2.0 with the added condition that it may not be used for commercial purposes without prior permission from the license holder.

One the other hand, the code is licensed under the MIT license. 
