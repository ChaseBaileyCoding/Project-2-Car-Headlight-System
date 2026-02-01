CAR ALARM/ IGNITION LOCK + AUTOMATIC HEADLIGHT SYSTEM
--
Group Members:
--
Chase Bailey, Colin Vaughan

System Behavior:
--
This project implements an ignition lock/alarm system and an automatic headlight subsystem.  
The system detects driver seat and passenger seat occupancy in addition to seatbelt inputs to determine whether or not ignition is allowed.
If the ignition button is pressed while any of the required conditions are not met, an alarm in the form of a buzzer will be triggered, the system will print the failed conditions, and ignition will be prevented.
When the required conditions for ignition are met (i.e., both seats are occupied, and both seatbelts are engaged), a green LED will turn on, indicating ignition is possible. 
Pressing the ignition while the green LED is active will "turn on" the engine, indicated by a blue LED. While the engine is active, a potentiometer is used to set the mode of the headlight subsystem, 
either OFF/ON/AUTO. While set OFF, both low beams will be turned off. While set ON, both low beams will be turned on. In the AUTO mode, the previous light mode is held, and a light sensor is read to 
determine whether the light level is dusk or day. If the sensor detects that the light level is below the dusk threshold, the low beams are forced high after 1 second. If the sensor detects a light 
level above the daylight threshold, both low beams are forced low after 2 seconds. 


Starting repository: 
--
https://github.com/ChaseBaileyCoding/Project-2-Car-Headlight-System/commit/42b3994999c4db244ee9ba2708f00df8c580dca4

Summary of testing results:
--
Car alarm/ignition lock

| specification | test process | results |
|-|-|-|
|Enable engine start while both seats are occupied and seatbelts are fastened | 1. all buttons pressed 2. all buttons, but one are pressed  3. no buttons are pressed | 1. the green light is turned on 2. corresponding error message is displayed, the LED stays off 3.  all errors are displayed, LED stays off|
|Turn on the engine when all conditions are met | both seats are occupied, both seatbelts are engaged, and the ignition button is pressed  | the engine LED activates and stays on after the buttons are released | 
|Display welcome message when driver enters| press driver seat button to simulate driver sitting in vehicle, observe output | Message is printed once in the serial output|
|Car alarm sounds when only the driver is seated with their seatbelt on | Press the driver's seat and seatbelt buttons, then press the ignition button | Alarm sounds, engine is not activated, and conditions not met are printed. |

Headlight system

| Behavior to Test | Test Process | Result |
|---|---|---|
|OFF mode selection turns off both low beams | Set the potentiometer to the OFF position | both lowbeams are set low regardless of the previous state|
|ON mode selection turns on both low beams | Set the potentiometer to the on position | both lowbeams are set high regardless of the previous state|
|Auto mode turns off low beams after 2 seconds if the sensor detects a light level above the daylight threshold | Low beams are initially set high, and a flashlight is pointed at the light sensor | The low beams are turned off after a 2-second delay | 
|Auto mode turns on low beams after 1 second if the sensor detects a light level below the dusk threshold | Low beams are initially set low, and the light sensor is covered | The low beams are turned on after a 1-second delay | 
|Headlights cannot be activated while the engine is off| Change potentiometer position while engine is off and system is on | No change in LED level, both low beams are off |
|Turning off ignition will turn off low beams| press the ignition button while ignition is on and headlights are on | Low beams are turned off | 
|Count is reset if mode is switched to auto before 2-second delay| switch headlight mode from auto to ON after the first increment but before the second increment, then turn the mode back to auto | the headlights turn off after 2-second delay, showing that the count is reset, which is the desired functionality   



