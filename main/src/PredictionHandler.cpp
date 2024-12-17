#include "PredictionHandler.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <iostream>
#include <iomanip>

static const char *TAG = "[esp_cli]";

PredictionHandler::PredictionHandler(const std::vector<std::string>& labels) : labels(labels) {}

void PredictionHandler::Update(const std::vector<float>& predictions, int sock) {
	if (resp(sock, (void*) predictions.data(), predictions.size() * sizeof(float)) < 0) {
		ESP_LOGE(TAG, "Failed to send response to server");
		close(sock);
		vTaskDelete(NULL);
	}
}

std::string PredictionHandler::GetLabel(int label) {
	if (label >= 0 && label < labels.size())
		return labels[label];
	
	return std::to_string(label);
}