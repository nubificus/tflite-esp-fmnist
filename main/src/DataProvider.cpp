#include "DataProvider.h"
#include <vector>
#include <string>
#include <iostream>

extern const float image0[] asm("_binary_Tshirt_top_bin_start");
extern const float image1[] asm("_binary_Trouser_bin_start");
extern const float image2[] asm("_binary_Pullover_bin_start");
extern const float image3[] asm("_binary_Dress_bin_start");
extern const float image4[] asm("_binary_Coat_bin_start");
extern const float image5[] asm("_binary_Sandal_bin_start");
extern const float image6[] asm("_binary_Shirt_bin_start");
extern const float image7[] asm("_binary_Sneaker_bin_start");
extern const float image8[] asm("_binary_Bag_bin_start");
extern const float image9[] asm("_binary_Ankle_boot_bin_start");

static constexpr int kNumCols = 28;
static constexpr int kNumRows = 28;
static constexpr int kMaxImageSize = kNumCols * kNumRows;
static constexpr int IMAGE_COUNT = 10;
static const float* image_database[IMAGE_COUNT];

std::vector<std::string> labels = {"T-shirt_top", "Trouser", "Pullover", "Dress", "Coat",
									"Sandal", "Shirt", "Sneaker", "Bag", "Ankle_boot"};

int DataProvider::Read(TfLiteTensor* modelInput) {
	static int data_index = 0;

	// There are no other data for inference
	if (data_index >= IMAGE_COUNT) {
		return 1;
	}

	// Copy the data to the model input tensor
	std::copy (
		image_database[data_index],
		image_database[data_index] + kMaxImageSize,
		modelInput->data.f
	);

	std::cout << std::endl << "Reading image: " << labels[data_index] << std::endl;
	
	data_index++;

	return 0;
}

bool DataProvider::Init() {
	image_database[0] = (float *) (image0);
	image_database[1] = (float *) (image1);
	image_database[2] = (float *) (image2);
	image_database[3] = (float *) (image3);
	image_database[4] = (float *) (image4);
	image_database[5] = (float *) (image5);
	image_database[6] = (float *) (image6);
	image_database[7] = (float *) (image7);
	image_database[8] = (float *) (image8);
	image_database[9] = (float *) (image9);
	
	return true;
}