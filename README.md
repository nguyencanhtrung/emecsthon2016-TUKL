###A balancing system
######TU Kaiserslautern 
#######[_@EMECS-thon_](http://emecs-thon.org/en/)
========
**Achievement**: People Choice Award

**Team Members**:
* [Trung C. Nguyen](mailto:nguyencanhtrung@me.com "Send an email")
* [Heber Cruz Zurian](mailto: "Private")
* [Rogelio Cruz Lopez](mailto: "Private")

A small project was implemented within 48 hours in EMECS-thon 2016 (a competition for Embedded System students 
between 6 universities in Europe - students have 48 hours to learn about a new micro-controller and develop their own projects running on this)
- Platform: Gecko 3700 starter kit
- IDE: Simplicity
- Programming language: C

**Topic description** 

The system receives commands and accelerometer data from an Android smartphone.
Basing on the command, it will perform in according operation modes
Basically, the system has 2 operation modes. 
 - Manual mode, movement of the plate is adjusted based on the movement the smartphone (extract data from accelerometer)
 - Automatic mode, performing self-balancing which means the system be able to hold the ball at the center of the plate in every circumstance

  **_Communication part_**
* The system communicate with Android smartphones by using an extra bluetooth module (HC-05), 
this module is connected with micro-controller through USART.
  
  **_Sensors and actuators_**
* Infrared sensors and LEDs are used to detect location of a ball.
* Actuators are 2 servomotors which control the angles of plate based on PWM signal generated by micro-controller.

---
Initial Setup Tutorial
---
#####Setup git

1. Download and install git for linux/windown
2. Open terminal or git bash
3. Set your name $git config --global user.name "<name>"
4. Set your mail address $git conifg --global user.email me_at_stupid.com

#####Create blink project

1. Open Simplicity Studio and Simplicity IDE
2. File->New->Silicon Labs MCU Project
3. Next->Example->STK3700_blink->Copy contents->Finish

#####Get the current version

1. Open terminal or git bash
2. Go to a location of your choice
3. Clone repository $git clone https://github.com/nguyencanhtrung/emecsthon2016-TUKL.git
4. Copy all files in project folder STK3700_blink. Overwrite all files if necessary. Do not forget the hidden folder .git!
5. Check status $git status
