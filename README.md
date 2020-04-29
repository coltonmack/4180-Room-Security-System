# 4180-Room-Security-System
Our Room Security System for 4180 Final Project

Team Members:

Colton Mack
Jonathan Wang

Introduction:

Our group was intrigued by the emerging technologies that have developed in regards to home security (Ring Doorbell, etc.). We wanted to create our own model of one of these systems using a Raspberry Pi. Our model would be used for room security. The idea was to make use of the Raspberry Pi camera functionality along with other I/O devices to create this security system. We added an ultrasonic sensor to detect motion, a microphone for sound detection, and a speaker to sound an alarm in addition to the camera module. We were able to run C programs to control the ultrasonic sensor and speaker directly using the pigpio library. However, we needed to make use of the mbed to detect noise as the Raspberry Pi does not natively support analog input. We used an apache web server and standard html/css/javascript to make the information accessable on a webpage. We also used ajax to calls to PHP functions that used shell commands to run the C programs that controlled the different I/O devices.
