set(SOURCES ./src/main.cpp
			./src/DataProvider.cpp
			./src/main_functions.cpp
			./src/micro_model.cpp
			./src/PredictionInterpreter.cpp
			./src/PredictionHandler.cpp
			./src/wifi.c
			./src/tcp_client.c)

idf_component_register(SRCS ${SOURCES}
						INCLUDE_DIRS . inc
						REQUIRES freertos esp_common tfmicro esp-nn esp_timer
						PRIV_REQUIRES spi_flash esp_netif esp_wifi nvs_flash)
