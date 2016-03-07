#!/bin/sh

echo Copying files
cp -i adc-sensor.c ~/contiki/platform/sky/dev/
cp -i adc-sensor.h ~/contiki/platform/sky/dev/
echo "CONTIKI_TARGET_SOURCEFILES += adc-sensor.c" >> ~/contiki/platform/sky/Makefile.sky
echo Done
