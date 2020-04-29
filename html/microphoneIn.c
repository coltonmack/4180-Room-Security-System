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
