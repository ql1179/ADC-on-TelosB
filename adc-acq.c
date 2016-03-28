#include "contiki.h"
#include "lib/sensors.h"
#include "dev/adc-sensor.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(adc_acq,"ADC Acquisition");
AUTOSTART_PROCESSES(&adc_acq);

#define FILENAME "data"

#ifndef NEED_FORMATTING
#define NEED_FORMATTING 0
#endif

PROCESS_THREAD(adc_acq,ev,data) {
      //codes without comment are merely for house-keeping 
      static struct etimer et;
      static int val;
      static int fd;
      static int r;
      static unsigned long seq; // sample seq
      static unsigned record[2]; // record[0] is the actual reading, record[1] is zero padding. Coffee system requires 1 byte zero padding between data. We just simply use 2 byte integer type of zero for padding.


      PROCESS_BEGIN();
      seq = 0;
      ADC12IE = 0;
      #if NEED_FORMATTING
	cfs_coffee_format();
      #endif	      
      cfs_remove(FILENAME);
      fd = cfs_open(FILENAME, CFS_WRITE + CFS_APPEND + CFS_READ);
      if(fd < 0) {
	printf("failed to open %s\n", FILENAME);
	return 0;
      }   
      while(seq <= 30000) { // sampling will terminate when reach 200, 000 
	   etimer_set(&et, CLOCK_SECOND * 0.01); // time interval is 0.01, which is 100Hz.
	   SENSORS_ACTIVATE(adc_sensor);
 	   if (seq % 50 == 0)
		leds_toggle(LEDS_GREEN);
	   seq++;
	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

           val = adc_sensor.value(0);
      	   if(val != -1) {
		//write
	      
      	      record[0] = val; // 12bit reading
              record[1] = 0; // zero padding
              r = cfs_write(fd, record, sizeof(record));
              if (r != sizeof(record)) {
	        printf("failed to write at seq %lu\n", seq);
	        cfs_close(fd);
	        return 0;
              }
              printf("write success %u\n", record[0]);
              
           }  
	   SENSORS_DEACTIVATE(adc_sensor);                    
	   etimer_reset(&et);

      } //end of while
      cfs_close(fd);  
      leds_on(LEDS_RED);
      etimer_set(&et, CLOCK_SECOND * 3);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
      leds_toggle(LEDS_GREEN);
      leds_off(LEDS_RED);
      PROCESS_END();

}

