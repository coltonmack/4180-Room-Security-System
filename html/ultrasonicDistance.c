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
