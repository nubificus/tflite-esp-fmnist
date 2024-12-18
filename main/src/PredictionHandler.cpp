#include "PredictionHandler.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <iostream>
#include <iomanip>

static const char *TAG = "[esp_cli]";

void PredictionHandler::Update(const std::vector<float>& predictions, int sock) {
	if (resp(sock, (void*) predictions.data(), predictions.size() * sizeof(float)) < 0) {
		ESP_LOGE(TAG, "Failed to send response to server");
		close(sock);
		vTaskDelete(NULL);
	}
}