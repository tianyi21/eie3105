# EIE3105: Integrated Project
*Autonomous Robot Car with STM32*:car: 

_Ⓒ Copyright Tianyi Liu 2019-2020, All Rights Reserved._

Latest Update: 12:29 AM July 1st, 2020 CST

This repo contains the source code developed for EIE3105, delivered by [Dr. Lawrence Cheung](http://www.eie.polyu.edu.hk/~encccl/) and [Dr. K.H. Loo](https://khloo563545619.wordpress.com), Dept of EIE, HKPU.

:boom:**If you use or partially use this repo, you shall formally - in report or presentation - acknowledge this repo.**

## Introduction
EIE3105 is an integrated hardware-software project that covers from the soldering to MCU programming. It uses [PID controllor](https://en.wikipedia.org/wiki/PID_controller) as the baseline, requiring students to implement control algorithms to accomplish different tasks with ta robot car autonomously. 

### How to Use?
:heavy_check_mark:Simply clone this repo through 

`git clone https://github.com/evanliuty/eie3105`

:heavy_check_mark:Open your IDE, build and upload to you MCU.

### Structure of this repo
`source` contains all the source files

`img`    contains the images

`doc`    contains the documents and deliverables

`test_quiz` contains previous quiz and test **USE AT YOUR OWN RISK!**

## Basic Settings
![alt text](https://github.com/evanliuty/eie3105/blob/master/img/robotcar.jpg "Robot Car")
![alt text](https://github.com/evanliuty/eie3105/blob/master/img/stm32.jpg "STM32")

The main body of the robot car will be distributed in the class. You will need your own [STM32F103RBT6](https://www.st.com/en/microcontrollers-microprocessors/stm32f103rb.html#overview) and corresponding cables.

For IDE selection, I used the [Keil µVision](http://www2.keil.com/mdk5/uvision/) with Windows 10 x64. You can also consider [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) which can be run with macOS.

## Description
⭐️### ARM_USART

This is a quite useful program for you to debug your program. You can send certain character or value through USART and connect the corresponding USART to your STM32 to display the values using TeraTerm. 

:warning: Since the USART2 of STM32F103RBT6 will be overrided by the USB cable, you have to use an intermediate USART (USART1 or USART3) to relay the information to/from USART2 of STM32.

#### Example

*Robot Car Send Information*

Robot Car USART1 *---Ext. Cable--->* STM32 USART1/3 *---Int. Program--->* STM32 USART2 *---USB Cable--->* TeraTerm

*Robot Car Receive Information*

TeraTerm *---USB Cable--->* STM32 USART2 *---Int. Program--->* STM32 USART1/3 *---Ext. Cable--->* Robot Car USART1

### Demo 2: Line Tracking

Perform line tracking and track changes without human intervention.

[Video Link](https://youtu.be/aTSeCGgGjnU)

#### Defination of Error

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo2_error.jpg "Demo 2 Error")

I considered the robot car on the track when the left boundary of the black line lies between the 3rd and 4th photo-resistor, and the right boundary lies between the 5th and 6th photo-resistor.

Therefore, this provides us with two options: Follow the left boundary or follow the right boundary. It also helps the robot car to perform track changing.

#### Control Scheme
P Control

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e" title="{PWM}_L = v_{nominal} - k_P \times e" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e" title="{PWM}_R = v_{nominal} + k_P \times e" /></a>

### Demo 3: Hit 3 Balls

Hitting 3 balls autonomously with the location information provided by WiFi station.

[Video Link](https://youtu.be/oVnxMDGdJqI) 

#### Flowchart

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo3_flow.jpg "Demo 3 Flowchart")

#### Communication with WiFi Station

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/wifi.jpg "WiFi Communication")

The robot car will acquire and update the locations repeatedly from the WiFi station with a timer. After every timer overflow event, it will listen until a **:** is received, which marks the starting point a data frame. Then, it stores all characters except for **\r** and **\n**. When an **M** (from *CMD*) is received, the timer is disabled and the program process the received information to update the corresponding locations. After the processing, the timer is re-enabled and wait for the next overflow event. *However, such setting is not rigorous and also not the **correct** way to use the data frame.* At the WiFi station, there are a few options that can be encoded in the data frame to control the robot car. Due to time limitation, we did not implement such function. A cherry-picked decoding scheme decribed above should work for demos.

#### Defination of Error

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo3_error.jpg "Demo 3 Error")

<a href="https://www.codecogs.com/eqnedit.php?latex=e&space;=&space;deg(\bold&space;v_{velocity})&space;-&space;deg(\bold&space;v_{path})" target="_blank"><img src="https://latex.codecogs.com/gif.latex?e&space;=&space;deg(\bold&space;v_{velocity})&space;-&space;deg(\bold&space;v_{path})" title="e = deg(\bold v_{velocity}) - deg(\bold v_{path})" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex=\bold&space;v_{velocity}&space;=&space;{loc}_{car}&space;-&space;{loc}_{car_{previous}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\bold&space;v_{velocity}&space;=&space;{loc}_{car}&space;-&space;{loc}_{car_{previous}}" title="\bold v_{velocity} = {loc}_{car} - {loc}_{car_{previous}}" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex=\bold&space;v_{path}&space;=&space;{loc}_{ball}&space;-&space;{loc}_{car}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\bold&space;v_{path}&space;=&space;{loc}_{ball}&space;-&space;{loc}_{car}" title="\bold v_{path} = {loc}_{ball} - {loc}_{car}" /></a>

Instead of using the absolute differences in x and y axes as PID error, I defined two vectors, a path vector, and a velocity vector. Moreover, I used a **one-cap design** that requires **only one marker** to locate the car and calculate its heading. The path vector is calculated as the difference between the location of the ball and the location of the car (cap). The velocity vector is calculated using two consecutive measurements of the robot car location.

#### Control Scheme
PI Control

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e&space;-&space;k_I&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e&space;-&space;k_I&space;\times&space;e" title="{PWM}_L = v_{nominal} - k_P \times e - k_I \times e" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e&space;&plus;&space;k_I&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e&space;&plus;&space;k_I&space;\times&space;e" title="{PWM}_R = v_{nominal} + k_P \times e + k_I \times e" /></a>

##### Linear Mapping

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo3_map.jpg "Linear Mapping")

We can see from the above image that when the distance becomes smaller, a same disturbance will create a **larger** error (in terms of degree). Therefore, using the same weighting parameters may not be a wise choice. Instead, the proportional weighting factor is **linearly decreased with the decrement of distance**, while integral remains constant.

<a href="https://www.codecogs.com/eqnedit.php?latex=k_P&space;=&space;\frac{dist(car,&space;ball)}{1250}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.4&space;\times&space;u(dist(car,ball)&space;-&space;500)," target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_P&space;=&space;\frac{dist(car,&space;ball)}{1250}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.4&space;\times&space;u(dist(car,ball)&space;-&space;500)," title="k_P = \frac{dist(car, ball)}{1250} \times u(500 - dist(car,ball)) + 0.4 \times u(dist(car,ball) - 500)," /></a>

### Demo 4: Hit Ball in Turns

Two robot cars hit the ball in turns, from one green region to the opposite green region, again, autonomously.

[Video Link](https://youtu.be/pxqHDLINmTU)

#### Flowchart

![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo4_flow.jpg "Demo 4 Flowchart")

#### Communication with WiFi Station

Same as Demo 3

#### Definition of Error

Same as Demo 3

#### Control Scheme
PI Control

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e&space;-&space;k_I&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_L&space;=&space;v_{nominal}&space;-&space;k_P&space;\times&space;e&space;-&space;k_I&space;\times&space;e" title="{PWM}_L = v_{nominal} - k_P \times e - k_I \times e" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex={PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e&space;&plus;&space;k_I&space;\times&space;e" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{PWM}_R&space;=&space;v_{nominal}&space;&plus;&space;k_P&space;\times&space;e&space;&plus;&space;k_I&space;\times&space;e" title="{PWM}_R = v_{nominal} + k_P \times e + k_I \times e" /></a>

##### Linear Mapping

Similiar to Demo 3, in Demo 4, both proportional and integral weighting factors are linearly mapped.

<a href="https://www.codecogs.com/eqnedit.php?latex=k_P&space;=&space;\frac{dist(car,&space;ball)}{1250}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.4&space;\times&space;u(dist(car,ball)&space;-&space;500)," target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_P&space;=&space;\frac{dist(car,&space;ball)}{1250}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.4&space;\times&space;u(dist(car,ball)&space;-&space;500)," title="k_P = \frac{dist(car, ball)}{1250} \times u(500 - dist(car,ball)) + 0.4 \times u(dist(car,ball) - 500)," /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex=k_I&space;=&space;\frac{dist(car,&space;ball)}{4000}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.1&space;\times&space;u(dist(car,ball)-&space;500)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_I&space;=&space;\frac{dist(car,&space;ball)}{4000}&space;\times&space;u(500&space;-&space;dist(car,ball))&space;&plus;&space;0.1&space;\times&space;u(dist(car,ball)-&space;500)" title="k_I = \frac{dist(car, ball)}{4000} \times u(500 - dist(car,ball)) + 0.1 \times u(dist(car,ball)- 500)" /></a>

#### Compensation of Error/Hitting-Target
![alt text](https://github.com/evanliuty/eie3105/blob/master/img/demo4_comp.jpg "Demo 4 Compensation")

A major difference between Demo 3 and Demo 4 is the locations of the balls are not fixed. Consequently, using the same strategy to hit the ball cannot guarantee the ball entering the opposite green region. A compensation scheme is designed to reduce this effect, or it can be called a hitting-target design, in contrast to hitting-ball.

First, an offset is defined by the distance the ball entering the green region (~the distance the ball needs to travel to the opposite green region). An extra distance is defined by the y-axis difference (~final hitting angle). Then, the car will actually hit the target defined by two previous steps, instead of hitting the ball itself.

Finally, the speed (PWM) is given by: 

<a href="https://www.codecogs.com/eqnedit.php?latex=v&space;=&space;v_{nomimal}&space;&plus;&space;\frac{1}{20}\times&space;L_{diff}&space;&plus;&space;\frac{1}{10}\times&space;{offset}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?v&space;=&space;v_{nomimal}&space;&plus;&space;\frac{1}{20}\times&space;L_{diff}&space;&plus;&space;\frac{1}{10}\times&space;{offset}" title="v = v_{nomimal} + \frac{1}{20}\times L_{diff} + \frac{1}{10}\times {offset}" /></a>

### Lab_Archived

This [folder](https://github.com/evanliuty/eie3105/tree/master/source/Lab_Archived) contains all lab files.

The lab sheet can be found here: [Sem1](https://github.com/evanliuty/eie3105/blob/master/doc/Lab_Sem1.pdf)         [Sem2](https://github.com/evanliuty/eie3105/blob/master/doc/Lab_Sem2.pdf)

## Final Remarks

:thumbsup: Thanks Mr. Bowen YANG for the very helpful discussions and suggestions during my implementation of this project.

:100: Try to implement your own control scheme first. This is a good and interesting class.

:exclamation: This repo is for your reference only, directly copy or modify without proper acknowledgment will result in ***plagiarism***.

:warning: Due to large differences among robot cars, the performance may vary even with the same code.

:memo: A formal version, i.e., the final report, of this README file can be found [here](https://github.com/evanliuty/eie3105/blob/master/doc/report.pdf).

:sunglasses: Most importantly, enjoy the class!
