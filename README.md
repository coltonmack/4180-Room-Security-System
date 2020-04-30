# 4180-Room-Security-System
## Our Room Security System for 4180 Final Project

## Team Members:

Colton Mack,
Jonathan Wang

## Introduction:

Our group was intrigued by the emerging technologies that have developed in regards to home security (Ring Doorbell, etc.). We wanted to create our own model of one of these systems using a Raspberry Pi. Our model would be used for room security. The idea was to make use of the Raspberry Pi camera functionality along with other I/O devices to create this security system. 

## Video Demo:

[![Video](https://img.youtube.com/vi/3MUbILLNlCg/maxresdefault.jpg)](https://www.youtube.com/watch?v=3MUbILLNlCg)

## Powerpoint Presentation:

[![Powerpoint](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Presentation.png)](https://drive.google.com/file/d/1PKbyUkijrtj7KbJfEo9iLL-Ew3n2jb8j/view?fbclid=IwAR0kjawb1E4BlpZjiKy2dBnQndjmz35e45wym4jfjU1FKdgFd8ano6Gxn88)

## Hardware Description:

### Finished Build:

![Build](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Build.png)

### Hardware Pinout:

![Hardware](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Pinout.png)

## Parts List:

### Raspberry Pi Model B3+

![Pi](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Raspberry_Pi.png)

### Raspberry Pi Camera Module V2

![Camera](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Camera.png)

### Mbed LPC1768

![Mbed](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Mbed.png)

### SPW2430 MEMS Microphone

![Mic](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Microphone.png)

### HRC-SR04 Ultrasonic Distance Sensor

![Ultrasonic](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Ultrasonic.png)

### Sparkfun PCB or Breadboard Mount Speaker 8ohm .1W

![Speaker](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Speaker.png)

### Class D High-Efficiency Audio Amp

![Amp](https://github.com/coltonmack/4180-Room-Security-System/blob/master/images/Amp.png)

## Software Description:

### Webpage:
We started this project with the Raspberry Pi as we had already had the camera module. We also knew that we could make use of Linux on the Pi to host an Apache Web Server where our project would be easily accessible from other devices on the LAN (and globally in the future). We started by installing Apache and setting up the front end for our application. This was done using standard HTML, CSS, and Javascript. We added some HTML DOM placeholders that would interact with the different I/O devices we would eventually connect.

### Camera Module:
After getting our webpage up, it was time to get the different I/O devices interacting with the PI. We started with the camera module. This one was pretty straightforward, as all that was needed was the raspistill command with the right arguments and the camera module attatched, and the Pi was successfully taking photos. 
```
raspistill -n -w 640 -h 480 -t 1000 -o /var/www/html/img/image0.jpg
```
The complicated part came when it was time to actually interact with the raspistill command from the webpage. This meant clicking a button on the webpage to take a photo on the Pi and then updating the webpage to display this photo. After some research, we decided the way to do this would be with AJAX requests to different PHP files. AJAX allows for the webpage to asynchronously make requests and update the DOM without having to refresh the entire webpage. This is what our AJAX request to take a photo looks like.
```
function updateWebcam(){
	var xmlhttp=new XMLHttpRequest();
	xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState==4 && xmlhttp.status==200){
		}
	}
	//Take a still image
	xmlhttp.open("GET","updateWebcam.php?_=" + new Date().getTime(), true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
}
```
PHP was used becauses 1) it can handle our AJAX requests and 2) it is able to make shell calls. For the camera, the PHP file called raspistill. So, clicking a button made an AJAX request to a PHP file that would use the camera to take a new picture (along with updating some text files). 
```
<?php
//retrieve last image number
$img_number = (int) file_get_contents('./img_number.txt', true);

//increment it
$img_number += 1;

//write new image number
file_put_contents('./img_number.txt', $img_number);

//take still image with PiCamera and store it in '/var/www/img' directory
shell_exec("raspistill -n -w 640 -h 480 -t 1000 -o /var/www/html/img/image".$img_number.".jpg");

//save new image name
$img_name = './img/image'.$img_number.'.jpg';

//update ajax_info with most recent image name
file_put_contents('./ajax_info.txt', $img_name);
?>
```
We were able to make another function that polled every so often (we decided on 2 seconds). This function would make an AJAX request to load one of the text files from above, and if it had been updated, display the new image on the webpage. Great! We are able to control the camera from our webpage.

### Ultrasonic Sensor:
The next I/O device we tackled was the ultrasonic sensor. This ultrasonic sensor uses pulses to determine how far away an object is in front of it. To use the ultrasonic on the Pi, we needed to write (using some help from online) code that would properly control these pulses to get accurate reads. Our code to control the ultrasonic sensor was written in C, and it made use of the pigpio C library functions including gpioWrite, gpioSetAlertFunc, and gpioSetTimerFunc. After enough trial and error, we were able to get accurate outputs from the sensor, and we could use these to determine if an object came into its range. 
```
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define TRIG 17
#define ECHO 4

void sonarTrigger(void);
void sonarEcho(int gpio, int level, uint32_t tick);

int values[5] = {0};
int i = 0;
int validCount = 0;

int main(int argc, char *argv[])
{
   if (gpioInitialise()<0) return 1;

   gpioSetMode(TRIG, PI_OUTPUT);
   gpioWrite(TRIG, PI_OFF);
   gpioSetMode(ECHO,PI_INPUT);

   /* update sonar 20 times a second, timer #0 */
   gpioSetTimerFunc(0, 50, sonarTrigger);

   /* monitor sonar echos */
   gpioSetAlertFunc(ECHO, sonarEcho);

   usleep(250000);
   gpioTerminate();

   for (int j = 0; j < 5; ++j) {
	   if (values[j] > 0 && values[j] < 150) {
		   validCount += 1;
	   } 
   }
   
   if (validCount >= 2) {
	printf("1\n");
   } else {
   	printf("0\n");
   }
   return 0;
}

void sonarTrigger(void)
{
   /* trigger a sonar reading */

   gpioWrite(TRIG, PI_ON);
   gpioDelay(10); /* 10us trigger pulse */
   gpioWrite(TRIG, PI_OFF);
}

void sonarEcho(int gpio, int level, uint32_t tick)
{
  
   static uint32_t startTick, firstTick=0;

   int diffTick;
   int output;
   if (!firstTick) firstTick = tick;

   if (level == PI_ON)
   {
      startTick = tick;
   }
   else if (level == PI_OFF)
   {
      diffTick = tick - startTick; 
	  output = diffTick / 58;
      values[i] = output;
      i += 1;
   }
}
```
To make this work with the webpage, we created another php file that would be called with a different AJAX request. This function was also included in our polling subroutine. So, every 2 seconds, a request would be made to that AJAX file, the shell command would run our C code, and the output would be returned to the javascript where the DOM was updated accordingly. 

### Speaker:
After the ultrasonic sensor came the speaker. The speaker (powered by the Class D Audio Amp) takes in a PWM signal, with the frequency setting its pitch and the duty cycle determining its loudness. Using one of the PWM pins on the Pi and the pigpio C library functions gpioSetPWMfreqeuncy and gpioPWM, we were able to create another C program that set the PWM frequency and duty cycle accordingly, souding the speaker (alarm) when called. 
```
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define SPEAKEROUT 18

int main(int argc, char *argv[])
{
	if (gpioInitialise()<0) return 1;
	
	gpioSetPWMfrequency(SPEAKEROUT, 500);
	gpioPWM(18, 128);
	
	sleep(1);
	gpioTerminate();
	
	return 0;
}
```
Just as before, we compiled the C code and placed in our project. We then added another PHP file to be called via an AJAX request that would run the program with a shell call. 

### Microphone:
Lastly, we added the microphone. This microphone was differnt from our other I/O devices, as we couldn't hook it up directly to the Pi (Raspberry Pi does not natively support analog input). Seeing as we didn't have any ADC chips on hand, we needed to use the Mbed to act as an "ADC." In reality, the Mbed ran code to read input from the microphone, and when it detected sound, drove an output pin to the Raspberry Pi high. 
```
#include "mbed.h"
//Adafruit MEMs SPW2430 microphone demo
//BusOut myleds(LED1,LED2,LED3,LED4);
 DigitalOut myled(LED1);    //p14
 DigitalOut outpin(p14);
class microphone
{
public :
    microphone(PinName pin);
    float read();
    operator float ();
private :
    AnalogIn _pin;
};
microphone::microphone (PinName pin):
    _pin(pin)
{
}
float microphone::read()
{
    return _pin.read();
}
inline microphone::operator float ()
{
    return _pin.read();
}
 
microphone mymicrophone(p16);
 
int main()
{
    while(1) {
//read in, subtract 0.67 DC bias, take absolute value, and scale up .1Vpp to 15 for builtin LED display
    if(int(abs((mymicrophone - (0.67/3.3)))*500.0) > 13){
        myled = 1;
        outpin = 1;
        wait(5);
    }
        
//Use an 8kHz audio sample rate (phone quality audio);
        wait(1.0/8000.0);
        myled = 0;
        outpin = 0;
    }
}
```
The Raspberry Pi would then read from this pin (using gpioRead from the pigpio library), and if it was high, would know noise was detected and update the DOM accordingly. 
```
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define MICIN 3

int values[5]={0};
int i = 0;
int validCount = 0;

int main(int argc, char *argv[])
{
	if (gpioInitialise()<0) return 1;
	
	gpioSetMode(MICIN, PI_INPUT);
	
	for (int j = 0; j < 5; ++j) {
	   if (gpioRead(MICIN) == 1) {
		   validCount += 1;
	   } 
	   usleep(10000);
    }
    
    if (validCount >= 2) {
		printf("%d\n", 1);
	} else {
		printf("%d\n", 0);
	}
	
	return 0;
}
```
After adding a pulldown resistor, we were able to make this work, and our code could now detect sound. Just as before, this compiled C program was added to our server and called from a PHP file via an AJAX request. This subroutine was also added to our polling function.

### Putting it together:

The last portion was tying all these parts together. The main problem was that we were checking the speaker and ultrasonic sesnor every loop (2 seconds), and these requests were coming back blank (they weren't finishing in time). To circumvent this, we made use of the Promises API. Promises allow us to better manage asynchronous operations (in this case our AJAX requests), and by chaining the ultrasonic and microphone calls via promises we were able to speed up their execution. 
```
function refresh(timeoutPeriod){
	checkWebcam();
	count = 0;
	checkMicrophone()
	.then(function (result) {
		console.log(result);
		if (result == "Sound detected")
		count += 1;
		checkSensor()
			.then(function (result) {
			console.log(result);
			if (result == "Object detected")
			count += 1;
			if (count > 0) {
				updateWebcam();
				soundAlarm();			
			}
		});
	});
	setTimeout("refresh(2000);",timeoutPeriod);
}
```
After adding this functionality, the tasks completed significantly faster making the webpage much more responsive. Lastly, we added some fun features, including sounding the alarm and taking a photo whenever noise or motion was detected. Our home security system is functional!

## Conclusion:

Overall we are satisfied with the outcome of our project. We were able to do the I/O we wanted while keeping the webpage pretty responsive. In retrospect, we should have combined the three AJAX functions that were called in our polling loop (check the text file, read ultrasonic, and read microphone) into one multithreaded C program. This would have reduced the number of AJAX calls made in the loop from 3 to 1, and since C code runs much faster than javascript/php, would have certainly improved performance. We will remember this for the next embedded system we develop. We also would have liked to make our server open to the web with ngrok or some other software as well as adding secure login functionality. All in all, we learned a lot from this project and hope to continue redefining the next generation of room security.
