#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "main_functions.h"
#include "wifi.h"

void tf_main(int argc, char* argv[]) {
  setup();
  while (true) {
    loop();
  }
}

extern "C" void app_main() {
	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	esp_err_t status = connect_wifi("nbfc-iot", "nbfcIoTOTA");

	if (WIFI_SUCCESS != status) {
		ESP_LOGI("main", "Failed to associate to AP, dying...");
		return;
	}

	xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
	vTaskDelete(NULL);
}