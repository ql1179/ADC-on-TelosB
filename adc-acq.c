#include "contiki.h"
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include <stdio.h>

PROCESS(adc_acq,"ADC Acquisition");
AUTOSTART_PROCESSES(&adc_acq);

#define FILENAME "data"

#ifndef NEED_FORMATTING
#define NEED_FORMATTING 0
#endif

PROCESS_THREAD(adc_acq,ev,data)
{ 
      static struct etimer et;
      static int val;
      static int fd;
      static int r;
      static uint8_t seq;
      static unsigned record[2];
      static unsigned rread[2];

      PROCESS_BEGIN();
	seq = 0;
      #if NEED_FORMATTING
	cfs_coffee_format();
      #endif	      
      cfs_remove(FILENAME);
//write
      fd = cfs_open(FILENAME, CFS_WRITE + CFS_APPEND + CFS_READ);
      if(fd < 0) {
	printf("failed to open %s\n", FILENAME);
	return 0;
      }   
      record[0] = 52455;
      record[1] = 0;
      r = cfs_write(fd, record, sizeof(record));
      if (r != sizeof(record)) {
	printf("failed to write %d bytes to %s\n", (int)sizeof(record), FILENAME);
	cfs_close(fd);
	return 0;
      }
      printf("write success %u\n", record[0]);
      cfs_close(fd);

//append
      fd = cfs_open(FILENAME, CFS_WRITE + CFS_APPEND + CFS_READ);
      if(fd < 0) {
	printf("failed to open %s\n", FILENAME);
	return 0;
      }   
      record[0] = 524;
      record[1] = 0;
      r = cfs_write(fd, record, sizeof(record));
      if (r != sizeof(record)) {
	printf("failed to write %d bytes to %s\n", (int)sizeof(record), FILENAME);
	cfs_close(fd);
	return 0;
      }
      printf("append success %u\n", record[0]);
      cfs_close(fd);


//read
      fd = cfs_open(FILENAME, CFS_READ);
      if(fd < 0) {
	printf("failed to open %s\n", FILENAME);
	return 0;
      }
      if (cfs_seek(fd, 0, CFS_SEEK_SET) != 0) {
	printf("seek failed\n");
        cfs_close(fd);
	return 0;
      }
	printf("file opened\n");

      for(;;){
        r = cfs_read(fd, rread, sizeof(rread));
	printf("seq %d\n", seq);
	seq++;
        if (r == 0) {
          printf("read finish\n");
          break;
        }
        else if (r != sizeof(rread)) {
	  printf("failed to read %d bytes to %s\n", (int)sizeof(rread), FILENAME);
	  cfs_close(fd);
	  return 0;
        }
        printf("Read success %u\n", rread[0]);
      }
      cfs_close(fd);
      cfs_remove(FILENAME);



      while(1)
      {
	   etimer_set(&et, CLOCK_SECOND * 0.1);
	   SENSORS_ACTIVATE(light_sensor);
	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


           val = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
//      	   if(val != -1) 
//		printf("read value = %d\n", val);                         
	   etimer_reset(&et);
    	   SENSORS_DEACTIVATE(light_sensor);
      } //end of while
    
      PROCESS_END();

}

