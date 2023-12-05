#include "delayed_reset.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delayed_reset()
{
  for (int i = 10; i >= 0; i--) {
      printf("Restarting in %d seconds...\n", i);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}
