#include "display.h"

esp_err_t display_dev_init(i2c_master_bus_handle_t *bus_handle)
{
    i2c_device_config_t disp_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x2D,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t disp_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &disp_cfg, &disp_handle));
    ESP_ERROR_CHECK(i2c_master_probe(*bus_handle, disp_cfg.device_address, -1));

    return ESP_OK;
}
