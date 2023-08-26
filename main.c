#include "uart.h"
#include "analogRead.h"
#include "bitmacros.h"
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "dht.h"


// https://controllerstech.com/temperature-measurement-using-dht22-in-stm32/
/* https://wokwi.com/projects/374031193915817985
*/




int main(){
	init_serial();

  uint16_t temperature_int = 0;
  uint16_t humidity_int = 0;

  _delay_ms(2000);
	while(true){  

    DHT_Status status = DHT_Get(&temperature_int, &humidity_int) ;
    if (status == DHT_Status_Ok) {
			
			printf("%d\n", humidity_int);
				printf("%d\n", temperature_int);
	}
    else printf("ERROR %d\n",status);
	_delay_ms(2000); 
  }

	return 0;
}