#include "PredictionInterpreter.h"
#include <algorithm>
#include <iostream>

PredictionInterpreter::PredictionInterpreter() {}

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

void PredictionInterpreter::Dequantize(const TfLiteTensor* output_tensor, std::vector<float>& output_data) {
	if (output_tensor->type == kTfLiteUInt8) {
		float scale = output_tensor->params.scale;
		int zero_point = output_tensor->params.zero_point;
		if (scale != 0) {
			int size = output_data.size();
			for (int i = 0; i < size; i++) {
				output_data[i] = (output_data[i] - zero_point) * scale;
			}
		}
	}
}
