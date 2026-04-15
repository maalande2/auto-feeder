#include "i2c_bus.h"

esp_err_t i2c_init(i2c_master_bus_handle_t *bus_handle)
{
    // config ESP as i2c master
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = DISPLAY_SCL,
        .sda_io_num = DISPLAY_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    // add ESP master to new master bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));
    return ESP_OK;
}
