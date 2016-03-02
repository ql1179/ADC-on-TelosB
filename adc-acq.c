#include "contiki.h"
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
// #include "dev/temperature-sensor.h"
#include <stdio.h>

PROCESS(adc_acq,"ADC Acquisition");
AUTOSTART_PROCESSES(&adc_acq);

PROCESS_THREAD(adc_acq,ev,data)
{ 
      static struct etimer et;
      static int val;
//      static struct sensors_sensor *sensor;

      PROCESS_BEGIN();
      
      while(1)
      {
	   etimer_set(&et, CLOCK_SECOND * 2);
	   SENSORS_ACTIVATE(light_sensor);
  //         SENSORS_ACTIVATE(temperature_sensor); 
	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


           val = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
      	   if(val != -1) 
      	  	printf("Total Solar = %d\n", val);                
	
           val = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
      	   if(val != -1) 
      	  	printf("Photosynthetic = %d\n", val); 
               
  //         val = temperature_sensor.value(1);
  //    	   if(val != -1) 
  //    	  	printf("Temperature = %d\n", val); 
               
	   etimer_reset(&et);
    	   SENSORS_DEACTIVATE(light_sensor);
 //          SENSORS_DEACTIVATE(temperature_sensor);

      } //end of while
    
      PROCESS_END();

}

