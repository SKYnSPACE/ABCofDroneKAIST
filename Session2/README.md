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
&nbsp;&nbsp; **This section describes the preparation of software for the hands-on session.**

#### 1.1. Install the Arduino Software on PCs
&nbsp;&nbsp; [ARDUINO IDE](https://www.arduino.cc/en/Main/Software) is an Integrated Development Environment (IDE) to write a code and upload it to the Arduino board. Please download and install the software depending on the operating system you use.

#### 1.2. (Optional) for Arduino Counterfeit Products.
&nbsp;&nbsp; If you have a [Counterfeit Arduino](https://www.arduino.cc/en/Products/Counterfeit) you may need to install FTDI (CH340/CH341) USB to Serial interface. Please visit the following [Website](http://www.ftdichip.com/Drivers/VCP.htm) for the Virtual COM Port (VCP) drivers.

### 2. Arduino Hardware Verifications
&nbsp;&nbsp; **From this section, we can verify the normal operations of Arduino we'll use throughout the whole sessions.**

#### 2.1. Wiring
![WiringDiagram](https://github.com/SKYnSPACE/ABCofDroneKAIST/blob/master/Session2/Hands-on/Images/ConnectionDiagram.png)

#### 2.2. Testing
&nbsp;&nbsp; Upload the code [ArduinoTest.ino](https://github.com/SKYnSPACE/ABCofDroneKAIST/tree/master/Session2/Hands-on/ARDUINO) to the ARDUINO board to test digital input/output, and serial communication.

### 3. MATLAB Simulation for a Quadcopter UAV
&nbsp;&nbsp; **In this section, you can describe the kinematics of a quadcopter UAV by completing the following quizzes.**

&nbsp;&nbsp; Quizzes are located in the class file [Drone.m](https://github.com/SKYnSPACE/ABCofDroneKAIST/tree/master/Session2/Hands-on/MATLAB/lib).
#### 3.1. Quiz 1
- Complete the velocity part of the state-space equation.

&nbsp;&nbsp; We have built a state-space equation with the following 14 states:

![states](https://latex.codecogs.com/gif.latex?X%20%3D%20%5Cbegin%7Bbmatrix%7Dx%20%5C%5C%20y%5C%5C%20z%5C%5C%20%5Cdot%7Bx%7D%5C%5C%20%5Cdot%7By%7D%5C%5C%20%5Cdot%7Bz%7D%5C%5C%20q_%7B1%7D%5C%5C%20q_%7B2%7D%5C%5C%20q_%7B3%7D%5C%5C%20q_%7B4%7D%5C%5C%20p%5C%5C%20q%5C%5C%20r%5Cend%7Bbmatrix%7D)

&nbsp;&nbsp; Now we want to figure out the first three rows of the following state space equation:\
![ssEquation](https://latex.codecogs.com/gif.latex?%5Cdot%7BX%7D%20%3D%20AX&plus;BU),
where A is a 14 by 14 matrix.

#### 3.2. Quiz 2
- Complete the accleration part of the state-space equation.

&nbsp;&nbsp; Now we want to figure out the row number 4 to 6 of the following state space equation:

#### 3.3. Quiz 3
- Describe a numerical integration method to figure out the next state

&nbsp;&nbsp; You can use the Euler method to update the next state:\
![Euler](https://latex.codecogs.com/gif.latex?X_%7Bt&plus;1%7D%20%3D%20X_%7Bt%7D%20&plus;%20%5Cfrac%7BdX%7D%7Bdt%7Ddt)

#### References
- Michael, N., et al. (2010). "The grasp multiple micro-uav testbed." IEEE Robotics & Automation Magazine 17(3): 56-65.

