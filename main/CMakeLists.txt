set(SOURCES ./src/main.cpp ./src/DataProvider.cpp ./src/main_functions.cpp ./src/micro_model.cpp ./src/PredictionInterpreter.cpp ./src/PredictionHandler.cpp)
idf_component_register(SRCS ${SOURCES}
						INCLUDE_DIRS . inc
						REQUIRES freertos esp_common tfmicro esp-nn esp_timer
						PRIV_REQUIRES test_data)
