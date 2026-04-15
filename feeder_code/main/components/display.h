#pragma once

#include "esp_err.h"

#include "../config.h"
#include "i2c_bus.h"

esp_err_t display_dev_init(i2c_master_bus_handle_t *bus_handle);
esp_err_t display_test(i2c_master_dev_handle_t disp_handle);
