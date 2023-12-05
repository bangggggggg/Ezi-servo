#include "beetle_steering.h"

#include <unistd.h>   // Chứa hàm usleep
#include <esp_log.h>  // Chứa hàm ESP_LOGW
#include "driver/uart.h"  // Chứa các hàm UART
#include "ezi_driver/CommInterface.h"   // Chứa hàm giao tiếp Ezi Driver
#include "ezi_driver/ReturnCodes_Define.h"  // Chứa mã lỗi Ezi Driver

static const char* TAG = "BEETLE STEERING";

// Limits
#define POSITION_LIMIT        (CONFIG_STEERING_POSITION_LIMIT)
#define SPEED_LIMIT           (CONFIG_STEERING_SPEED_LIMIT)
#define DEFAULT_SPEED         (CONFIG_STEERING_DEFAULT_SPEED)

// Ezi SERVO communication configurations
#define SERVO_DRIVER_ADDRESS  0x00
#define BUF_SIZE              (512)
#define COMM_BAUD_RATE        (CONFIG_EZI_UART_BAUD_RATE)
#define COMM_PORT_NUM         (CONFIG_EZI_UART_PORT_NUM)
#define COMM_TXD              (CONFIG_EZI_UART_TXD)
#define COMM_RXD              (CONFIG_EZI_UART_RXD)
#define COMM_RTS              (UART_PIN_NO_CHANGE)
#define COMM_CTS              (UART_PIN_NO_CHANGE)

#define MAX_RETRIES           (1)
#define RETRY_INTERVAL_US     (1000)

#define PRINT_WARNING(errno)  \
  ESP_LOGW(TAG, "errno: %d", errno);

#define RETURN_W_WARNING(errno)   \
  PRINT_WARNING(errno);           \
  if (errno) return ESP_FAIL;

#define RETRY_W_WARNING(cmd, ...) {   \
  int err = FMM_NOT_OPEN;             \
  int retry_cnt = MAX_RETRIES;        \
  while (1) {                         \
    err = cmd(__VA_ARGS__);           \
    if (err == FMM_OK) break;         \
    PRINT_WARNING(err);               \
    if (retry_cnt-- <= 0) break;      \
    usleep(RETRY_INTERVAL_US);        \
  }                                   \
  if (err) return ESP_FAIL;           \
}

static int32_t steering_speed;

/**
 * @brief Initialize steering servo interfacing
 */
esp_err_t str_init()
{
  esp_err_t err;
  /* Initialize module's variables */
  steering_speed = DEFAULT_SPEED;

  /* Initialize UART */
  uart_config_t uart_config = {
    .baud_rate = COMM_BAUD_RATE,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
  };
  int intr_alloc_flags = 0;
#if CONFIG_UART_ISR_IN_IRAM
  intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif
  err = uart_driver_install(
    COMM_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags);
  if (err) return ESP_FAIL;
  err = uart_param_config(COMM_PORT_NUM, &uart_config);
  if (err) return ESP_FAIL;
  err = uart_set_pin(COMM_PORT_NUM, COMM_TXD, COMM_RXD, COMM_RTS, COMM_CTS);
  if (err) return ESP_FAIL;
  err = uart_set_mode(COMM_PORT_NUM, UART_MODE_RS485_HALF_DUPLEX);
  if (err) return ESP_FAIL;

  /* Clear io bits of servo on command */
  err = str_clear_io_servo_on();
  if (err) return ESP_FAIL;

  return ESP_OK;
}

/**
 * @brief Clear IO bits of servo on command
 */
esp_err_t str_clear_io_servo_on()
{
  /* Loop through 12 IOs */
  for (unsigned char io = 0; io < 12; io++)
  {
    unsigned long io_logic_msk;
    unsigned char level;

    RETRY_W_WARNING(FAS_GetIOAssignMap,
      SERVO_DRIVER_ADDRESS, io, &io_logic_msk, &level);
    if (io_logic_msk & 0x00020000)
    {
      RETRY_W_WARNING(FAS_SetIOAssignMap,
        SERVO_DRIVER_ADDRESS, io, (io_logic_msk & ~(0x00020000)), level);
    }
  }
  return ESP_OK;
}

/**
 * @brief Turn on steering servo
 */
esp_err_t str_servo_on()
{
  RETURN_W_WARNING(FAS_ServoEnable(SERVO_DRIVER_ADDRESS, 1));
  return ESP_OK;
}

/**
 * @brief Turn off steering servo
 */
esp_err_t str_servo_off()
{
  RETURN_W_WARNING(FAS_ServoEnable(SERVO_DRIVER_ADDRESS, 0));
  return ESP_OK;
}

/**
 * @brief Get steering servo's actual shaft position
 *
 * @param feedback Servo position in pulses
 */
esp_err_t str_get_position(int32_t *feedback)
{
  RETURN_W_WARNING(FAS_GetActualPos(SERVO_DRIVER_ADDRESS, (long*)feedback));
  return ESP_OK;
}

/**
 * @brief Set target for steering servo's shaft position
 *
 * @param expt Servo's shaft position in pulses
 */
esp_err_t str_set_expt(int32_t expt)
{
  static int32_t prev_expt = -1;

  // limit steering angle
	if (expt > POSITION_LIMIT) expt = POSITION_LIMIT;
	else if (expt < -POSITION_LIMIT) expt = -POSITION_LIMIT;

  // command steering servo
  if (expt != prev_expt) {
    int err;
    err = FAS_MoveSingleAxisAbsPos(SERVO_DRIVER_ADDRESS, expt, steering_speed);
    if (err == FMP_RUNFAIL) {
      err = FAS_PositionAbsOverride(SERVO_DRIVER_ADDRESS, expt);
    }
    RETURN_W_WARNING(err);
    prev_expt = expt;
  }
  return ESP_OK;
}
