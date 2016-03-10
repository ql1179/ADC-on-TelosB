# ADC-on-TelosB
The code is to configure ADC on TelosB in Contiki so that it can measure and store the energy amount of a motion energy harvester Ampy Move.

The data can be later collected from TelosB via USB connection.

Run `sh install.sh` to import the ADC driver.

The voltage input should be connected to PIN3 (channel 0) and PIN9 (ground) on the 10-pin input block.

Run `make TARGET=sky MOTES=/dev/ttyUSB0 adc-acq.upload login` to burn the sensor reading program. Sensor data will be stored in the EPPROM on the board. Later, we can run `make TARGET=sky MOTES=/dev/ttyUSB0 read-file.upload login > readings.txt` to get the data and store them into a file called `readings.txt`.

The sensor reading is currently set to 100Hz, and the reading will terminate when finshing 200,000 samples due to the memory limit, where one sample is 4 bytes and we have total 1MB memory. Therefore, the recording will last for 2000 seconds, a bit longer than half an hour.
