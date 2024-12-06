#pragma once

#include <vector>
#include <string>
#include <utility>

class PredictionHandler {
public:
	PredictionHandler(const std::vector<std::string>& labels);
	void Update(const std::vector<std::pair<int, float>>& predictions);

private:
	std::vector<std::string> labels;
	std::string GetLabel(int label);
};