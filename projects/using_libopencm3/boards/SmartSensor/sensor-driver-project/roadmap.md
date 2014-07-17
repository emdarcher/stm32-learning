Roadmap for this project:
========================

1. Learn about I2C/TWI and how it works

2. Learn the way that the sensor communicates via I2C.
  - possibly experiment using the BusPirate's I2C on the sensor

3. Figure out how to implement I2C on the STM32F103C8T6 mcu

4. Test communication between the STM32 and sensor
  - probably use a logic analyser to watch the signals

5. create a plan for a modular approach for the code

6. start writing prototype code

7. test/debug

8. improve/rewrite code for a possibly better implementation

9. test/debug

10. Hopefully have a fully functional driver.

Other things:
------------

* Figure out why only 2 out of the 4 SmartSensor V1 boards give correct IDs to the ST-Link v1/v2.

* Maybe try putting a modified LeafLabs Maple USB DFU bootloader on the SmartSensor so it can be programmed through the carrier board's USB port. The Maple board uses a STM32F103RBT6 with is almost the same as the STM32F103C8T6, just with 128K of flash vs. 64K and a larger pin count.


