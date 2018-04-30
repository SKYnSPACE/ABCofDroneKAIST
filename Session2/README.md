# Session2 (30.APR.2018)

## Lecture Materials (by Taemin Shim)
- **Please refer to the slides in the Lecture folder.**

## Hands-on Materials (by Seongheon Lee)

```
###### Things to prepare ######
1. Personal computer (Laptop)
2. Arduino Kit composed with
  - Arduion UNO
  - Breadboard
  - Jumper wires
  - LED Red x1
  - LED Green x1
  - Push button x1
###############################
```

### 1. Arduino Software Installations
**This section describes the preparation of software for the hands-on session.**

#### 1.1. Install the Arduino Software on PCs
[ARDUINO IDE](https://www.arduino.cc/en/Main/Software) is an Integrated Development Environment (IDE) to write a code and upload it to the Arduino board. 
Please download and install the software depending on the operating system you use.

#### 1.2. (Optional) for Arduino Counterfeit Products.
If you have a [Counterfeit Arduino](https://www.arduino.cc/en/Products/Counterfeit) you may need to install FTDI (CH340/CH341) USB to Serial interface.
Please visit the following [Website](http://www.ftdichip.com/Drivers/VCP.htm) for the Virtual COM Port (VCP) drivers.

### 2. Arduino Hardware Verifications
**From this section, we can verify the normal operations of Arduino we'll use throughout the whole sessions.**

#### 2.1. Wiring
![WiringDiagram](https://github.com/SKYnSPACE/ABCofDroneKAIST/blob/master/Session2/Hands-on/Images/ConnectionDiagram.png)

#### 2.2. Testing
Upload the code [ArduinoTest.ino](https://github.com/SKYnSPACE/ABCofDroneKAIST/tree/master/Session2/Hands-on/ARDUINO) to the ARDUINO board to test digital input/output, and serial communication.

### 3. MATLAB Simulation for a Quadcopter UAV
**In this section, you can describe the kinematics of a quadcopter UAV by completing the following quizzes.**
#### 3.1. Quiz 1
- Complete the velocity part of the state-space equation.
${dx\dt}= 
#### 3.2. Quiz 2
- Complete the accleration part of the state-space equation.
#### 3.3. Quiz 3
- Describe a numerical integration method to figure out the next state
#### References
- Michael, N., et al. (2010). "The grasp multiple micro-uav testbed." IEEE Robotics & Automation Magazine 17(3): 56-65.

