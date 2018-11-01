#include "SPI.h"

const char* SPI::LOG_TAG = "SPI";
const uint8_t SPI::SPI_DEF_PORTS[3][3] = {
    {0, 0, 0},
    {14, 13, 12},
    {18, 23, 19}
};

esp_err_t SPI::init(uint8_t csPin) {
    return init((gpio_num_t)csPin);
}

esp_err_t SPI::init(gpio_num_t csPin) {
    spi_bus_config_t bus_config;
	bus_config.sclk_io_num = SPI_DEF_PORTS[_spi_num][0];
	bus_config.mosi_io_num = SPI_DEF_PORTS[_spi_num][1];
	bus_config.miso_io_num = SPI_DEF_PORTS[_spi_num][2];
	bus_config.quadwp_io_num = -1;
	bus_config.quadhd_io_num = -1;
	bus_config.max_transfer_sz = 0;
    bus_config.flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_SCLK | SPICOMMON_BUSFLAG_MISO | SPICOMMON_BUSFLAG_MOSI;

    spi_device_interface_config_t dev_config;
    dev_config.clock_speed_hz = SPI_MASTER_FREQ_8M;
    dev_config.mode = 0;
    dev_config.spics_io_num = csPin;
    dev_config.queue_size = 1;
    dev_config.pre_cb = NULL;
    dev_config.post_cb = NULL;
    dev_config.flags = 0;
    dev_config.command_bits = 0;
    dev_config.address_bits = 8;
    dev_config.dummy_bits = 0;
    dev_config.duty_cycle_pos = 0;
    dev_config.cs_ena_posttrans = 0;
    dev_config.cs_ena_pretrans = 0;
    dev_config.input_delay_ns = 0;

    return init(bus_config, dev_config);
}

esp_err_t SPI::init(spi_bus_config_t bus_config, spi_device_interface_config_t dev_config) {
    esp_err_t spi_err;

    ESP_LOGI(
        LOG_TAG,
        "Starting SPI cs[%d] clk[%d] mosi[%d] miso[%d]",
        dev_config.spics_io_num, SPI_DEF_PORTS[_spi_num][0], SPI_DEF_PORTS[_spi_num][1], SPI_DEF_PORTS[_spi_num][2]);

    spi_err = spi_bus_initialize(_spi_num, &bus_config, 0);
    if (spi_err != ESP_OK) {
        return spi_err;
    }

    ESP_LOGI(LOG_TAG, "SPI BUS initialized");
    ESP_LOGI(LOG_TAG, "SPI adding device");

    spi_err = spi_bus_add_device(_spi_num, &dev_config, &_spi);
    if (spi_err != ESP_OK) {
        return spi_err;
    }

    ESP_LOGI(LOG_TAG, "SPI device added");

    return spi_err;
}

esp_err_t SPI::readByteByAddress(uint8_t addr, uint8_t *value) {
    spi_transaction_t trans;

    trans.flags = 0;
    trans.addr = addr;
    trans.cmd = 0;
    trans.length = 8;
    trans.rxlength = 8;
    trans.user = NULL;
    trans.tx_buffer = NULL;
    trans.rx_buffer = value;

    return spi_device_transmit(_spi, &trans);
}

esp_err_t SPI::writeByAddress(uint8_t addr, uint8_t *values, uint8_t count) {
    spi_transaction_t trans;

    trans.flags = 0;
    trans.addr = addr;
    trans.cmd = 0;
    trans.length = count * 8;
    trans.user = NULL;
    trans.rxlength = 0;
    trans.tx_buffer = values;
    trans.rx_buffer = NULL;

    return spi_device_transmit(_spi, &trans);
}

esp_err_t SPI::transmitQueue(spi_transaction_t *trans_desc) {
    return spi_device_queue_trans(_spi, trans_desc, 10);
}

esp_err_t SPI::receiveQueue(spi_transaction_t **trans_desc) {
    return spi_device_get_trans_result(_spi, trans_desc, 10);
}

esp_err_t SPI::transmit(spi_transaction_t *trans_desc) {
    return spi_device_transmit(_spi, trans_desc);
}

esp_err_t SPI::close() {
    return spi_bus_free(_spi_num);
}
