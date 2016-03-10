 
#include "contiki.h"
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
      static int fd;
      static int r;
      static unsigned rread[2];

      PROCESS_BEGIN();
      #if NEED_FORMATTING
	cfs_coffee_format();
      #endif	    

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
	printf("Reading commences:\n");

      for(;;){
        r = cfs_read(fd, rread, sizeof(rread));
        if (r == 0) {
          printf("Reading finishes.\n");
          break;
        }
        else if (r != sizeof(rread)) { // If we don't finish all 200,000 samples, it will likely end here.
	  printf("Reading ends abnormally.\n");
	  break;
        }
        printf("%u\n", rread[0]);
      }
      cfs_close(fd);
      cfs_remove(FILENAME);
      PROCESS_END();

}

