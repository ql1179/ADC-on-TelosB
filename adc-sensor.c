#include "contiki.h"
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
#include "dev/adc-sensor.h"

#define INPUT_CHANNEL      1 << INCH_0
#define INPUT_REFERENCE     SREF_0
#define DATA_MEM  ADC12MEM0

const struct sensors_sensor adc_sensor;

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  return DATA_MEM;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int c)
{
  return sky_sensors_configure(INPUT_CHANNEL, INPUT_REFERENCE, type, c);
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  return sky_sensors_status(INPUT_CHANNEL, type);
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(adc_sensor, "ADC_SENSOR",
               value, configure, status);
