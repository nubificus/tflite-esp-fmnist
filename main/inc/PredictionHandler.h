#pragma once

#include <vector>
#include <string>
#include <utility>

#include "tcp_client.h"

class PredictionHandler {
	public:
		void Update(const std::vector<float>& predictions, int sock);
};