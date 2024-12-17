#pragma once

#include <vector>
#include <string>
#include "tensorflow/lite/c/common.h"

class PredictionInterpreter {
public:
	PredictionInterpreter();
	std::vector<float> GetResult(const TfLiteTensor* output_tensor, float threshold);

private:
	void Dequantize(const TfLiteTensor* output_tensor, std::vector<float>& output_data);
};
