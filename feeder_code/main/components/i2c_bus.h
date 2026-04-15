#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

#include "../config.h"

esp_err_t i2c_init(i2c_master_bus_handle_t *bus_handle);
