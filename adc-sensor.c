#include "contiki.h"
#include "lib/sensors.h"
#include "dev/adc-sensor.h"

const struct sensors_sensor adc_sensor;

/*---------------------------------------------------------------------------*/
static CC_INLINE void
start(void)
{

  /* Set up the ADC. */
  P6DIR = 0xff;
  P6OUT = 0x00;

  /* Setup ADC12, ref., sampling time */
  /* XXX Note according to the specification a minimum of 17 ms should
     be allowed after turn on of the internal reference generator. */
 // ADC12CTL0 = REF2_5V + SHT0_6 + SHT1_6 + MSC + REFON;
  ADC12CTL0 = SHT0_6 + SHT1_6 + MSC;
  /* Use sampling timer, repeat-sequence-of-channels */
  ADC12CTL1 =  SHP + CONSEQ_2 + CSTARTADD_0;

//  /* Set highest end-of-sequence. */
//  ADC12MCTL0 = EOS;

  ADC12CTL0 |= ADC12ON;
  ADC12CTL0 |= ENC;                   /* enable conversion */
  ADC12CTL0 |= ADC12SC;               /* sample & convert */
}
/*---------------------------------------------------------------------------*/
static CC_INLINE void
stop(void)
{
  /* stop converting immediately, turn off reference voltage, etc. */

  ADC12CTL0 &= ~ENC;
  /* need to remove CONSEQ_3 if not EOS is configured */
  ADC12CTL1 &= ~CONSEQ_2;

  /* wait for conversion to stop */
  while(ADC12CTL1 & ADC12BUSY);

  /* clear any pending interrupts */
  ADC12IFG = 0;
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  if(type == SENSORS_ACTIVE) {
    return 1;
  }
  if(type == SENSORS_READY) {
    return (ADC12IFG & INCH_0) == INCH_0;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int c)
{

  if(type == SENSORS_ACTIVE) {
    stop();

    if(c) {
      P6SEL |= INCH_0 & 0xff;

      /* Set ADC config */
      ADC12MCTL0 = INCH_0 + SREF_0;

    } 
    else {
      P6SEL &= ~(INCH_0 & 0xff);
    }


    start();
    return 1;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  return ADC12MEM0 & 0x0FFF;
}

/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(adc_sensor, "ADC_SENSOR",
               value, configure, status);
