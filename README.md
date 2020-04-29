# 4180-Room-Security-System
## Our Room Security System for 4180 Final Project

## Team Members:

Colton Mack
Jonathan Wang

## Introduction:

Our group was intrigued by the emerging technologies that have developed in regards to home security (Ring Doorbell, etc.). We wanted to create our own model of one of these systems using a Raspberry Pi. Our model would be used for room security. The idea was to make use of the Raspberry Pi camera functionality along with other I/O devices to create this security system. 

##Video Demo:

https://www.youtube.com/watch?v=3MUbILLNlCg

## Project Description:

### Webpage:
We started this project with the Raspberry Pi as we had already had the camera module and knew we could go from there. We also knew that we could make use of Linux on the Pi to host an Apache Web Server where our project would be accessible from other devices on the LAN (and even globally in the future). We started by installing Apache and setting up the front end for our application. This was done using standard html, css, and javascript. We added some HTML DOM placeholders that would interact with the different I/O devices we would eventually connect.

### Camera Module:
After getting our webpage up, it was time to get the different I/O devices interacting with the PI. We started with the camera module. This one was pretty straightforward, as all that was needed was the raspistill command with the right arguments and the camera module attatched, and the Pi was successfully taking photos. The complicated part came when it was time to actually interact with the raspistill command from the webpage. This meant clicking a button on the webpage to take a photo and then updating the webpage to display this photo. After some research, I decided the way to do this would be with AJAX requests to different PHP files. AJAX allows for the webpage to asynchronously make requests and update the DOM without having to refresh the entire webpage. PHP was used becauses 1) it can handle our AJAX requests and 2) it is able to make shell calls. For the camera, the PHP file called raspistill. So, clicking a button made an AJAX request to a PHP file that would use the camera to take a new picture (along with updating some text files). I was the able to make another function the polled every so often (we decided on 2 seconds). This function would check a text file, and if it had been updated, would load the new image on the webpage. Great! We are able to control the camera from our webpage.

### Ultrasonic Sensor:
The next I/O device we tackled was the ultrasonic sensor. This ultrasonic sensor uses pulses to determine how far away an object is in front of it. To use the ultrasonic on the Pi, we needed to write (and find some help online) code that would properly control these pulses to get accurate outputs. Our code to control the ultrasonic sensor was written in C, and it made use of the pigpio C library, with the functions gpioSetAlertFunc and gpioSetTimerFunc. After enough trial and error, we were able to get accurate outputs and use these to determine if an object came into range of the sensor. To make this work with the webpage, we created another php file that would be called with a different AJAX request that we included in our polling subroutine. So, every 2 seconds, a request would be made to that AJAX file, the shell command would run our C code, and the output would be returned to the javascript where the DOM was updated accordingly. 

### Speaker:
After the ultrasonic sensor came the speaker. The speaker takes in a PWM signal, with the frequency determining its pitch and the duty cycle determining its loudness. Using one of the PWM pins on the Pi and the pigpio library, we were able to create another C program that set the PWM frequency and duty cycle accordingly and sounded the speaker (alarm) when called. Just as before, we compiled the C code and placed in our project. We then added another PHP file to be called via an AJAX request that would run the program with a shell call. 

### Microphone:
Lastly, we added the microphone. However, since the Raspberry Pi does not do native analog input, we needed to use the Mbed to act as an ADC (we didn't have any ADC chips on hand). So, we used the Mbed to read from the speaker, and if noise was detected, would drive an output pin high. The Raspberry Pi would then read from this pin (using gpioRead from the pigpio library), and if the pin was high, the Pi would know that noise was detected. After adding a pulldown resistor, we were able to make this work, and our code could now detect sound. Just as before, this compiled C program was added to our server and called from a PHP file via an AJAX request.

The last portion was tying all these parts together. The main problem was that we were checking the speaker and ultrasonic sesnor every loop (2 seconds), and these requests were coming back blank (the weren't finishing in time). To circumvent this, we made use of the Promises API. Promises are asynchronous, allowing the code to happen in the background without clogging up the main event thread. After adding the Promise functionality, we were able to complete these tasks much quicker making the webpage much more responsive. Lastly, we added some fun features, including sounding the alarm and taking a photo whenever noise or motion was detected. Our home security system is functional!
