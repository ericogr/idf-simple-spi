# idf-spi-simple

SPI interface library in C++ for working with idf (Espressif ESP32 IoT Development Framework), that supports master mode.

This SPIbus library is designed just as a high-level access of the ESP32's SPI peripheral, that actually, incorporates some of the _basic functionality_ of `spi_master` driver from _esp-idf_. Its purpose is to simplify the usage of the native spi-master driver, and it is most for communication with **8-bit register sensors**.

## Install

You can clone it right into your project components directory or in your specific library path.

```git
 git clone https://github.com/ericogr/idf-simple-spi.git idf-simple-spi
```

## Usage

The ESP32 has some user avaible SPI peripheral devices, called HSPI and VSPI.

SPI0 is entirely dedicated to the flash cache, the ESP32 uses to map the SPI flash device it is connected to into memory. SPI1 is connected to the same hardware lines as SPI0 and is used to write to the flash chip. HSPI and VSPI are free to use and are the currently implemented ones in the esp-idf.

So we can start off initializing the `vspi` as follows:

```C++
SPI _SPI(VSPI_HOST);
_SPI.init(5); //5==csPin
```

HSPI and VSPI all have three chip select lines, allowing them to drive up to three SPI devices each as a master.
