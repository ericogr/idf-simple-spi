#ifndef __SPI_H
#define __SPI_H

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <esp_log.h>

class SPI {
    static const char* LOG_TAG;
    static const uint8_t SPI_DEF_PORTS[3][3];

public:
    SPI(uint8_t spi_num): SPI((spi_host_device_t)spi_num) {
    }
    SPI(spi_host_device_t spi_num): _spi_num(spi_num) {
        _spi = nullptr;
    }
    ~SPI() {
        close();
    }
    esp_err_t init(uint8_t csPin);
    esp_err_t init(gpio_num_t csPin);
    esp_err_t init(spi_bus_config_t bus_config, spi_device_interface_config_t dev_config);

    esp_err_t transmit(spi_transaction_t *trans_desc);
    esp_err_t readByteByAddress(uint8_t addr, uint8_t *value);
    esp_err_t writeByAddress(uint8_t addr, uint8_t *values, uint8_t count);
    esp_err_t transmitQueue(spi_transaction_t *trans_desc);
    esp_err_t receiveQueue(spi_transaction_t **trans_desc);

    esp_err_t close();
private:
    gpio_num_t _csPin;

    spi_host_device_t _spi_num;
    spi_device_handle_t _spi;
};

#endif
