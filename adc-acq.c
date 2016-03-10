#include "contiki.h"
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
#include "dev/adc-sensor.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include <stdio.h>

PROCESS(adc_acq,"ADC Acquisition");
AUTOSTART_PROCESSES(&adc_acq);

#define FILENAME1 "va"
#define FILENAME1 "vb"

#ifndef NEED_FORMATTING
#define NEED_FORMATTING 0
#endif

PROCESS_THREAD(adc_acq,ev,data) {
      //codes without comment are merely for house-keeping 
      static struct etimer et;
      static int val;
      static int fd;
      static int r;
      static uint8_t seq; // sample seq
      static unsigned record[2]; // record[0] is the actual reading, record[1] is zero padding. Coffee system requires 1 byte zero padding between data. We just simply use 2 byte integer type of zero for padding.


      PROCESS_BEGIN();
      seq = 0;
      #if NEED_FORMATTING
	cfs_coffee_format();
      #endif	      
      cfs_remove(FILENAME1);
      cfs_remove(FILENAME2);

      while(seq <= 100000) { // sampling will terminate when reach 200, 000 
	   etimer_set(&et, CLOCK_SECOND * 0.1); // time interval is 0.01, which is 100Hz.
	   SENSORS_ACTIVATE(adc_sensor);
	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	   seq++;

           val = adc_sensor.value(0);
      	   if(val != -1) {
		//write
	      fd = cfs_open(FILENAME1, CFS_WRITE + CFS_APPEND + CFS_READ);
	      if(fd < 0) {
		printf("failed to open\n");
		return 0;
      	      }   
      	      record[0] = val; // 12bit reading
              record[1] = 0; // zero padding
              r = cfs_write(fd, record, sizeof(record));
              if (r != sizeof(record)) {
	        printf("failed to write\n");
	        cfs_close(fd);
	        return 0;
              }
              printf("Va: %u\n", record[0]);
              cfs_close(fd);
           }     
           val = adc_sensor.value(1);
      	   if(val != -1) {
		//write
	      fd = cfs_open(FILENAME2, CFS_WRITE + CFS_APPEND + CFS_READ);
	      if(fd < 0) {
		printf("failed to open\n");
		return 0;
      	      }   
      	      record[0] = val; // 12bit reading
              record[1] = 0; // zero padding
              r = cfs_write(fd, record, sizeof(record));
              if (r != sizeof(record)) {
	        printf("failed to write\n");
	        cfs_close(fd);
	        return 0;
              }
              printf("Vb: %u\n", record[0]);
              cfs_close(fd);
           }                     
	   etimer_reset(&et);
    	   SENSORS_DEACTIVATE(adc_sensor);
      } //end of while
    
      PROCESS_END();

}

