#include "uart.h"
#include "analogRead.h"
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#define PINNE_A0 0


#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

const float BETA = 3950; // should match the Beta Coefficient of the thermistor


// https://wokwi.com/projects/373853359976045569
/*
NTC thermistors are resistors with a negative temperature coefficient, which means that the resistance decreases with increasing temperature. They are primarily used as resistive temperature sensors and current-limiting devices.
*/


unsigned long Pow2(long V, unsigned char shift)
{
  unsigned long Val = 1;
  while (shift-- != 0)
    Val *= V;

  return Val;
}

char * ftos(char * buf, unsigned char W, unsigned char D, float data)
{
  unsigned char shf = 0;
  if (data < 0)
  {
    data *= -1;
    shf = 1;
    buf[0] = '-';
  }
  long Wdata = data, tmp;
  float dec = data - (long)data;

  // get the first whole number and convert it
  buf[0 + shf] = Wdata / Pow2(10, W - 1) + '0';
  tmp = Wdata % Pow2(10, W - 1);

  //now get the rest of the whole numbers
  for (unsigned char i = 1; i < W; i++)
  {
    long factor = Pow2(10, W - 1 - i);
    buf[i + shf] = (tmp / factor) + '0';
    tmp %= factor;
  }

  buf[W + shf] = '.'; // add the decimal point

  // now do the decimal numbers
  for (unsigned char i = 0; i < D; i++)
  {
    dec *= 10;
	int d = (int)dec;
	int a = (int)(d % 10);
    buf[W + i + 1 + shf] = a + '0';
  }
  // don't forget the NULL terminator
  buf[W + D + 1 + shf] = 0;
  return buf;
}





int main(){
	init_serial();

	BIT_CLEAR(DDRC,PINNE_A0);


	while(true){
		uint16_t value = analogRead(PINNE_A0); // 0 - 1023
		float celsius = 1 / (log(1 / (1023. / value - 1)) / BETA + 1.0 / 298.15) - 273.15;

//The AVR port does not link any float support by default 
//(BTW many ARM implementations also) because of the size of the functions.
// Remember that float math support + printf floats may take up to 15k of your 32k 
//program memory!!
//		printf("%f\n",celsius);


		char txt[30];
		printf("%s\n", ftos(txt,3,2,celsius));
    //
    // printf("%.2f\n", celsius);
		_delay_ms(1000);
	}
	return 0;
}