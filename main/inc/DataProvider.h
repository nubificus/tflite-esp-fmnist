#pragma once

#include "tensorflow/lite/c/common.h"
#include <iterator>

class DataProvider {
	public:
	bool Init();
	int Read(TfLiteTensor* modelInput);
};