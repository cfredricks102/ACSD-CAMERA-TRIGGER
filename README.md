# Project: Autonomous Crop Surveillance Drone (ACSD)

***Project Description***

The Autonomous Crop Surveillance Drone (ACSD) is an unmanned aerial vehicle (UAV) that'll fly at an optimized altitude of 10ft above the field under test (FUT). After the user gives the dimensions of the FUT, the drone will have an automated distance interval between each capture. This will optimize the amount of space surveyed relative to the resolution of the ZED camera. Once one image is captured, the image will then be processed by the YoloV7 algorithm processed on Jetson Nano. The real time processing will be occurring simultaneously as the UAV moves to the next capture location. Once the entire field has been successfully surveyed, the UAV will land and the user can extract the data via a USB SSD. Inserting the SSD into the PC,  the User Interface will generate an interactive map that will indicate which capture location has diseased foliage within a degree of accuracy. 

### **Module: ACSD-CAMERA-TRIGGER**
*This repository includes the GPIO Camera Triggering software that is a piece of the ACSD System*

***Software Description***

The ACSD-CAMERA-TRIGGER program consists of three modules. KConfig, CMakeList, gpio_trigger

- Kconfig file configures Linux build time dependencies for the PX4-Autopilot software. 
- CMakeList file adds the gpio_trigger module to the default built in app list. This is a cmake definition file that uses px4_add_module command to add the gpio_trigger function to the prexisting software. 
- The gpio_trigger.c enables General Purpose Input Output (GPIO) communication between the Nvidia Jetson Nano and the PX4 Holybro 6C flight controller. After the user sets waypoints for the Aerial Vehicle to hover, the gpio_trigger software will automatically trigger the Jetson to take an image at the set locations. This program  offers dynamic execution time to change the hovering capture time. The delay for the pulse is gnerated by px4_sleep functions that pend on user input. It also has built in protection for excess hover time. The output from pixhawk is configured to pin FMU AUX 5 & 6 for potential for multi imaging. The pulse of the trigger is set at 3.3V and rising edge. 

***This specific release is meant for version 6 of the pixhawk autopilot software and not compatible for sitl simulation files** 
