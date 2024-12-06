#include "PredictionHandler.h"
#include <iostream>
#include <iomanip>

PredictionHandler::PredictionHandler(const std::vector<std::string>& labels) : labels(labels) {}

void PredictionHandler::Update(const std::vector<std::pair<int, float>>& predictions) {
	std::cout << "#----Start Inference Results----#" << std::endl;
	for (const auto& prediction : predictions) {
		std::cout << "Label " << GetLabel(prediction.first)
				  << ": " << std::fixed << std::setprecision(4)
				  << prediction.second * 100 << "%" << std::endl;
	}
	std::cout << "#-----End Inference Results-----#" << std::endl;
}

std::string PredictionHandler::GetLabel(int label) {
	if (label >= 0 && label < labels.size())
		return labels[label];
	
	return std::to_string(label);
}