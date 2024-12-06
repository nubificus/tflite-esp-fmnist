#include "PredictionInterpreter.h"
#include <algorithm>
#include <iostream>

PredictionInterpreter::PredictionInterpreter() {}

std::vector<std::pair<int, float>> PredictionInterpreter::GetResult(const TfLiteTensor* output_tensor, float threshold) {
	const float* output_data = output_tensor->data.f;
	std::vector<float> output_vector(output_data, output_data + output_tensor->bytes / sizeof(float));

	// Handle quantized output
	Dequantize(output_tensor, output_vector);

	// Sort the results
	return Sort(output_vector, threshold);
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

std::vector<std::pair<int, float>> PredictionInterpreter::Sort(const std::vector<float>& data, float threshold) {
	std::vector<std::pair<int, float>> results;
	for (size_t i = 0; i < data.size(); ++i) {
		if (data[i] >= threshold) {
			results.emplace_back(i, data[i]);
		}
	}
	std::sort(results.begin(), results.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
		return a.second > b.second;
	});
	return results;
}