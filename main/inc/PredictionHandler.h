#pragma once

#include <vector>
#include <string>
#include <utility>

#include "tcp_client.h"

class PredictionHandler {
public:
	PredictionHandler(const std::vector<std::string>& labels);
	void Update(const std::vector<float>& predictions, int sock);

private:
	std::vector<std::string> labels;
	std::string GetLabel(int label);
};