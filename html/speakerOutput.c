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
