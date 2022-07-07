#include <stdio.h>
#include <stdlib.h>
#include <nvs_flash.h>

#include "OTAServer.h"
#include "MyWiFi.h"
#include "driver/gpio.h"
#include "esp_task.h"


/*
 *
 * void app_main()
 *
 **/
void app_main()
{
	gpio_reset_pin(3);
    gpio_reset_pin(4);
    gpio_reset_pin(5);
    gpio_reset_pin(18);
    gpio_reset_pin(19);
    gpio_set_level(18, 0);//白
    gpio_set_level(19, 0);//橙
    gpio_set_level(3, 0);
    gpio_set_level(4, 0);
    gpio_set_level(5, 0);
    gpio_set_direction(3, GPIO_MODE_OUTPUT);
    gpio_set_direction(4, GPIO_MODE_OUTPUT);
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    gpio_set_direction(18, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);

	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	
    printf("\r\n***************************\r\n");
    printf("***   ESP32-C3 System Up   ***\r\n");
    printf("***************************\r\n");


	printf("Compiled at:");
	printf(__TIME__);
	printf(" ");
	printf(__DATE__);
	printf("\r\n");
	
	// Need this task to spin up, see why in task			
	xTaskCreate(&systemRebootTask, "rebootTask", 2048, NULL, ESP_TASK_PRIO_MIN + 1, NULL);

	// Start as an AP
	//init_wifi_softap(&OTA_server);

	// Connect to a Router so as to give a quick test
	//init_wifi_station(&OTA_server);

	mywifi_init();
}
