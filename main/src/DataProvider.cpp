#include "DataProvider.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <vector>
#include <string>
#include <iostream>

static constexpr int kNumCols = 28;
static constexpr int kNumRows = 28;
static constexpr int kMaxImageSize = kNumCols * kNumRows;
static float image_buf[kMaxImageSize];

static const char *TAG = "[esp_cli]";
static const char *host_ip = "192.168.11.56";
static uint16_t port = 1234;

int DataProvider::Read(TfLiteTensor* modelInput) {
	// Get the next image from the server
	if (next_image(sock, (void*) image_buf, sizeof(image_buf)) < 0) {
		ESP_LOGE(TAG, "Failed to receive image");
		close(sock);
		vTaskDelete(NULL);
	}

	// Copy the data to the model input tensor
	std::copy (
		image_buf,
		image_buf + kMaxImageSize,
		modelInput->data.f
	);

	return 0;
}

bool DataProvider::Init() {
	sock = connect_to_server(host_ip, port);

	if (sock  == -1) {
		ESP_LOGE(TAG, "Failed to Connect");
		vTaskDelete(NULL);
	}
	
	return true;
}