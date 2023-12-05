#ifndef _STEERING_H_
#define _STEERING_H_

#include "esp_err.h"   // Chứa kiểu esp_err_t và uint32_t

esp_err_t str_init();
esp_err_t str_clear_io_servo_on();
esp_err_t str_servo_on();
esp_err_t str_servo_off();
esp_err_t str_get_position(int32_t *feedback);
esp_err_t str_set_expt(int32_t expt);

#endif // _STEERING_H_
