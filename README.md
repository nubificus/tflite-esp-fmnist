# ESP TFLITE deployment 

**Table of Contents:**

1. [General information](#general-information)
2. [main](#main)
3. [main_functions](#main_functions)
4. [DataProvider](#dataprovider)
5. [PredictionInterpreter](#predictioninterpreter)
6. [PredictionHandler](#predictionhandler)
7. [TCP_Server](#tcp_server)
8. [TCP_Client](#tcp_client)

---

## General information

This project is meant to be used as a template for deploying tflite models on ESP32 devices. This repository uses a FashionMNIST model as an example for demonstrating the project's basic utilities. Once you clone this repository ensure to use the `update_components.sh` script as follows:
```bash
chmod +x scripts/update_components.sh
```
```bash
./scripts/update_components.sh
```

## main

The ESP32 application's main file connects to the wifi and then calls the `tf_main`, which setups the device for inference and then uses `loop` to invoke the model for various input data.


## main_functions

This file implements the `setup` and `loop` functions used by [main](#main). The `setup` function simply loads the C array containing the tflite model, allocates memory for the tensors, ensures the input tensor's dimensions are the expected ones and finally calls `data_procider.Init` to prepare the environment for the inference. In this particular implementation, `data_procider.Init` connects to a remote TCP server which will send the inference input data and then receive the inference results (i.e. the model's dequantized output and the inference time). Then, the `loop` function presented below will read an image from the TCP server and feed it to the input tensor(by using `data_procider.Read`), invoke the model, read and dequantize the output if necessary(by using `prediction_interpreter.GetResult`), and finally send it back to the TCP server by using `prediction_handler.Update`. The last step is to send the inference time to the TCP server.

```c++
void loop() {
	// Read test data and copy them to the model input tensor
	if (data_provider.Read(model_input)) {
		error_reporter->Report("Failed ro read next image");
		return;
	}

	// Run inference on pre-processed data
	long long start_time = esp_timer_get_time();

	TfLiteStatus invoke_status = interpreter->Invoke();
	if (invoke_status != kTfLiteOk) {
		error_reporter->Report("Invoke failed");
		return;
	}

	long long inference_time = esp_timer_get_time() - start_time;

	// Interpret raw model predictions
	auto prediction = prediction_interpreter.GetResult(model_output, 0.0);

	// Send the result to the server
	prediction_handler.Update(prediction, data_provider.sock);

	// Send inference time to the server
	if (resp(data_provider.sock, (void*) &inference_time, sizeof(inference_time)) < 0) {
		ESP_LOGE(TAG, "Failed to send response to server");
		close(data_provider.sock);
		vTaskDelete(NULL);
	}

	vTaskDelay(0.5 * pdSECOND);
}
```

## DataProvider

The `Init` function below simply connects to a remote TCP server.
```c++
bool DataProvider::Init() {
	sock = connect_to_server(host_ip, port);

	if (sock  == -1) {
		ESP_LOGE(TAG, "Failed to Connect");
		vTaskDelete(NULL);
	}
	
	return true;
}
```

The `Read` function below requests the next image from the TCP Server and copys it onto the model's input tensor
```c++
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
```

## PredictionInterpreter

The `GetResult` function takes the output tensor, dequantizes the results if they have been quantized by the model and then removes any data with scores below a given threshold before returning the final output vector(this can be used when we want to omit predictions with low chance).

```c++
std::vector<float> PredictionInterpreter::GetResult(const TfLiteTensor* output_tensor, float threshold) {
	const float* output_data = output_tensor->data.f;
	std::vector<float> output_vector(output_data, output_data + output_tensor->bytes / sizeof(float));

	// Handle quantized output
	Dequantize(output_tensor, output_vector);

	// Remove elements with scores below the threshold
	for (auto it = output_vector.begin(); it != output_vector.end(); ) {
		if (*it < threshold) {
			it = output_vector.erase(it);
		} else {
			++it;
		}
	}
	return output_vector;
}
```

## PredictionHandler

The `Update` function presented below responds to TCP Server by sending it the final output vector

```c++
void PredictionHandler::Update(const std::vector<float>& predictions, int sock) {
	if (resp(sock, (void*) predictions.data(), predictions.size() * sizeof(float)) < 0) {
		ESP_LOGE(TAG, "Failed to send response to server");
		close(sock);
		vTaskDelete(NULL);
	}
}
```

## TCP_Server

The TCP Server is implemented in the `tcp_image_server.py` file. Its purpose is to iteratively provide an image to the TCP Client on the ESP32 device and then wait for two responses from the client. During the first response, the client sends the inference results to the server and during the second it sends the inference time. The results for the server's whole dataset are stored in the `results.txt` file. If the server starts giving the same data (which means the whole dataset has been parsed), then the `results.txt` file is closed and the results are shown in stdout.

## TCP_Client
The TCP Client is implemented in the `tcp_client.c` file and provides three basic functions:

1. ```c++
	int connect_to_server(const char *host_ip, uint16_t port)
   ```
	* Establishes a connection to the TCP Server

2. ```c++
	int next_image(int sock, void *rx_buffer, size_t buffer_size)
   ```
	* Requests the next available image from the TCP Server and stores it into `rx_buffer`

3. ```c++
	int resp(int sock, void *result_buffer, size_t buffer_size)
   ```
	* Sends the `results_buffer` contents back to the TCP Server