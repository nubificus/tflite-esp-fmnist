#pragma once

#include <vector>
#include <string>
#include "tensorflow/lite/c/common.h"

class PredictionInterpreter {
public:
	PredictionInterpreter();
	std::vector<std::pair<int, float>> GetResult(const TfLiteTensor* output_tensor, float threshold);

private:
	void Dequantize(const TfLiteTensor* output_tensor, std::vector<float>& output_data);
	std::vector<std::pair<int, float>> Sort(const std::vector<float>& data, float threshold);
};
