#pragma once

#include "tensorflow/lite/c/common.h"
#include <iterator>

class DataProvider {
	public:
	bool Init();
	int Read(TfLiteTensor* modelInput);

	private:
	static constexpr int kNumCols = 28;
	static constexpr int kNumRows = 28;
	static constexpr int kMaxImageSize = kNumCols * kNumRows;
	static constexpr int IMAGE_COUNT = 10;
	static const float* image_database[IMAGE_COUNT];
};

