# Engineering Materials

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2021.

## Content

* `photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
models is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

Team Apollo has designed a 4-wheel rear-wheel driven Arduino-PIXY2 based robotic car which primarily uses a lego chassis with a combination of 3D printed assets for WRO’s FEC. 
We aim to sustainably drive the future in order to efficiently aid the upcoming generation into AI-driven fields. 
For the build of the vehicle, we have used an **Arduino MEGA 2650 Microcontroller** in addition to its compatible sensors, such as the following:
* CMU Pixy Gen2 Camera
* HCSR04 Ultrasonic Sensors (5x)
* TCRT-5000 Analog Infrared Sensor (2x)
* MPU6050 Gyroscope Sensor
* L298n Motor Driver
* TowerPro SG90 9G Micro Servo
* 5:1 DC Mini Gear Motor (2x)
* Arduino Mini Breadboard
* 12V Lithium-ion Batteries (2x)
* 9V Battery, 2x SamBot Wheels (2x)
* 2x Lego Mini Wheels (2x)
* Lego Technic Kit



### Pixy 2 Camera:

A pixy 2 camera is used to identify and differentiate between traffic signs. Using the Pixy2 library, we first initiate the object pixy, and set up the pixy using ‘pixy.init()’ under void setup. Under the void loop, we write an initial ‘if’ statement activating the pixy with the statement ‘( pixy.ccc.getBlocks())’ under which we initialise a variable ‘dist’ which acts as a function for the following if statements.
A second ‘if’ statement is written ‘if (dist < 10)’ which only allows the pixy to detect objects at a distance less than 10cm away. Inside this, 2 if statements for each color of the traffic signs are written. The red block has been initialized as block[0], with the block signature as 1, if this particular block is detected, the servo is commanded to turn to 180 degrees for 1.5 seconds and then return to its original position. Similarly, the green block has been initialized as block[1], with the block signature as 2 and if detected the servo is commanded to turn to 90 degrees for 1.5 seconds and then return to its original position.
In addition to the detection of blocks, the pixy is also used for navigating through corners in the final round. Setting the orange and blue lines as two different signatures, we are able to turn the car. The mechanism starts with turning the servo to 60/160 degrees once the orange/blue line is detected (depending upon the driving direction) and turns until the second line(either orange or blue depending on the direction) is detected.
Motor Driver: We have employed the use of an L298n motor driver for controlling the speed and the direction of our 2 rear wheel drive DC motors. The code starts off by initializing our pins for direction(in1,2,3,4), speed(enA,enB)and a variable ‘sped’ to give a value of speed. Another variable called ‘count’ is initialized with the null value (int count;).Under the void setup all the pins are highlighted as output pins. Furthermore, under the void loop we specify the direction for our directional pins (eg: ‘digitalWrite (in1, HIGH);’) and the speed for our speed control pins as ‘sped’ (eg: ‘analogWrite (enA, sped);’ ). The motors are now functional and running with a speed of 50. To stop the motors at the correct position we specified an if statement under the void loop to check the value of count which if greater than or equal to 3(specifying the three laps) makes the speed controls of the motors 0(analogWrite (enA, 0);).

### Ultrasonic Sensor:

We have used the ultrasonic sensor to map out the boundaries of the game field in order to effectively map the borders and to make Apollo avoid crashing into one. We have initialized the trigger pin as the output and the echo pin as the input for the ultrasonic sensor. The trig pin is connected to pin number 12, the dt(distance) and echo pin are connected to pin number 11. The basic gist of the code turns the servo 90 degrees right/left if the ultrasonic sensor detects the presence of an object less than 10 cm away. The side to which the servo motor turns is determined by the ultrasonic sensor which gives out a signal in the presence of an object less than 10 cm away. For this purpose, we have employed the use of 5 ultrasonic sensors on our model, one on each side, 3 in the front, in which 2 are in the left and right center respectively and one is positioned dead-center. We initialize calc_dis as a variable to calculate the distance that is measured by the ultrasonic sensor. If the distance is less than 10 cm, Apollo instructs the servo to turn 90 degrees for a maximum duration of 1000 milliseconds and return to its original position within the same time frame. All 5 ultrasonic sensors work in harmony to maneuver the track. In addition to this, we also employ a counting mechanism which is used to stop the car in the optimum position after 3 laps have been completed.

### Servo:

We’ve used a TowerPro SG90 9G micro servo motor to help Apollo steer. It is attached to digital pin number 8 on the arduino. To custom fit the Arduino servo onto our lego chassis, we 3D printed a bracket that efficiently connects the servo bracket into the gear of the lego chassis that powers our ackermann steering mechanism. The servo uses 3 pins all connected to the arduino, one which acts as the unique servo data cable and the rest which is used primarily for power. The servo is connected to the lego gear using a 3D printed chassis which in-turn rotates two other gears connected to the axle connecting Apollo’s wheels. The servo turns these gears based on the Arduino's command as mentioned in the aforementioned sections. This is what forms the base of Apollos steering mechanism. The servo rotates at a couple of angles including 140,80,60,160 keeping 110 as the center.

