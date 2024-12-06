#include "DataProvider.h"

extern const float image0[] asm("_binary_Tshirt_top_start");
extern const float image1[] asm("_binary_Trouser_start");
extern const float image2[] asm("_binary_Pullover_start");
extern const float image3[] asm("_binary_Dress_start");
extern const float image4[] asm("_binary_Coat_start");
extern const float image5[] asm("_binary_Sandal_start");
extern const float image6[] asm("_binary_Shirt_start");
extern const float image7[] asm("_binary_Sneaker_start");
extern const float image8[] asm("_binary_Bag_start");
extern const float image9[] asm("_binary_Ankle_boot_start");

int DataProvider::Read(TfLiteTensor* modelInput) {
	static int data_index = 0;

	// There are no other data for inference
	if (data_index >= IMAGE_COUNT) {
		return 1;
	}

	std::copy (
		image_database[data_index],
		image_database[data_index] + kMaxImageSize,
		modelInput->data.f
	);
	
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