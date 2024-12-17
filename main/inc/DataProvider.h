#pragma once

#include "tensorflow/lite/c/common.h"
#include <iterator>

#include "tcp_client.h"

class DataProvider {
	public:
	int sock;
	
	bool Init();
	int Read(TfLiteTensor* modelInput);
};