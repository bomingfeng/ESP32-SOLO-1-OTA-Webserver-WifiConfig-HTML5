#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "OTAServer.h"
#include "MyWiFi.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_KEY_wifi_ap_sta  "wifi_ap_sta"
#define TAG "MyWiFi"

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t wifi_event_group;
#define CONNECTED_BIT  BIT0
#define CONNECTION_FAILED_BIT BIT1

const int AP_CLIENT_CONNECTED_BIT = BIT0;
const int AP_CLIENT_DISCONNECTED_BIT = BIT1;
const int WIFI_STA_CONNECTED_BIT = BIT3;
const int WIFI_STA_DISCONNECTED_BIT = BIT4;



static void ip_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    switch (event_id) {
		case IP_EVENT_STA_GOT_IP: {
			printf("SYSTEM_EVENT_STA_GOT_IP\r\n");
 			ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        	ESP_LOGI("WiFI", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
			//xEventGroupSetBits(wifi_event_group, WIFI_STA_CONNECTED_BIT);
			/* Start the web server */
			start_OTA_webserver();
			/* Signal to continue execution */
			xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
			xEventGroupClearBits(wifi_event_group, CONNECTION_FAILED_BIT);
			break;
		}
		case IP_EVENT_STA_LOST_IP: {
			printf("SYSTEM_EVENT_STA_LOST_IP\r\n");
			break;
		}
		case IP_EVENT_GOT_IP6: {
			printf("SYSTEM_EVENT_GOT_IP6\r\n");
			break;
		}
		case IP_EVENT_ETH_GOT_IP: {
			printf("SYSTEM_EVENT_ETH_GOT_IP\r\n");
			break;
		}
		default: {
			printf("SYSTEM_EVENT_ OTHER\r\n");
			break;
		}	
    }
    return;
}

RTC_DATA_ATTR static int boot_sta = 0xaa;
RTC_DATA_ATTR static int boot_ap = 0xaa;
static int s_retry_num = 0;
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    switch (event_id) {
		case WIFI_EVENT_WIFI_READY: {
			printf("SYSTEM_EVENT_WIFI_READY\r\n");
			break;
		}
		case WIFI_EVENT_SCAN_DONE: {
			printf("SYSTEM_EVENT_SCAN_DONE\r\n");
			break;
		}
		case WIFI_EVENT_STA_START: {
			printf("SYSTEM_EVENT_STA_START\r\n");
			esp_wifi_connect();
			printf("Connectiing To SSID:%s : Pass:%s\r\n", CONFIG_STATION_SSID, CONFIG_STATION_PASSPHRASE);
			break;
		}
		case WIFI_EVENT_STA_STOP: {
			printf("SYSTEM_EVENT_STA_STOP\r\n");
			break;
		}
		case WIFI_EVENT_STA_CONNECTED: {
			printf("SYSTEM_EVENT_STA_CONNECTED\r\n");
			break;
		}
		case WIFI_EVENT_STA_DISCONNECTED: {
			printf("SYSTEM_EVENT_STA_DISCONNECTED\r\n");
			if (s_retry_num < 100) 
			{
				esp_wifi_connect();
				s_retry_num++;
				ESP_LOGI("WiFI","retry to connect to the AP.(%d / %d)\n", s_retry_num,100);
			} 
			else
			{
				boot_ap = 0x55;
				boot_sta = 0x55;
				//xEventGroupClearBits(wifi_event_group, WIFI_STA_DISCONNECTED_BIT);
				ESP_LOGI("WiFI","connect to the AP fail");
				xEventGroupSetBits(wifi_event_group, CONNECTION_FAILED_BIT);
                xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
			}		
			break;
		}
		case WIFI_EVENT_STA_AUTHMODE_CHANGE: {
			printf("SYSTEM_EVENT_STA_AUTHMODE_CHANGE\r\n");
			break;
		}
		case WIFI_EVENT_STA_WPS_ER_SUCCESS: {
			printf("SYSTEM_EVENT_STA_WPS_ER_SUCCESS\r\n");
			break;
		}
		case WIFI_EVENT_STA_WPS_ER_FAILED: {
			printf("SYSTEM_EVENT_STA_WPS_ER_FAILED\r\n");
			break;
		}
		case WIFI_EVENT_STA_WPS_ER_TIMEOUT: {
			printf("SYSTEM_EVENT_STA_WPS_ER_TIMEOUT\r\n");
			break;
		}
		case WIFI_EVENT_STA_WPS_ER_PIN: {
			printf("SYSTEM_EVENT_STA_WPS_ER_PIN\r\n");
			break;
		}
		case WIFI_EVENT_AP_START: {
			printf("SYSTEM_EVENT_AP_START\r\n");
			break;
		}
		case WIFI_EVENT_AP_STOP: {
			printf("SYSTEM_EVENT_AP_STOP\r\n");
			break;
		}
		case WIFI_EVENT_AP_STACONNECTED: {
			printf("SYSTEM_EVENT_AP_STACONNECTED\r\n");
			wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        	ESP_LOGI("WiFI-AP", "station "MACSTR" join, AID=%d",	\
                MAC2STR(event->mac), event->aid);
			//xEventGroupSetBits(wifi_event_group, AP_CLIENT_CONNECTED_BIT);
			/* Start the web server */
			start_OTA_webserver();
			break;
		}
		case WIFI_EVENT_AP_STADISCONNECTED: {
			printf("SYSTEM_EVENT_AP_STADISCONNECTED\r\n");
			wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        	ESP_LOGI("WiFI-AP", "station "MACSTR" leave, AID=%d",	\
                MAC2STR(event->mac), event->aid);
			//xEventGroupSetBits(wifi_event_group, AP_CLIENT_DISCONNECTED_BIT);
			break;
		}
		case WIFI_EVENT_AP_PROBEREQRECVED: {
			printf("SYSTEM_EVENT_AP_PROBEREQRECVED\r\n");
			break;
		}
		case WIFI_EVENT_MAX: {
			printf("SYSTEM_EVENT_MAX\r\n");
			break;
		}
		default: {
			printf("SYSTEM_EVENT_ OTHER\r\n");
			break;
		}	
    }
    return;
}

/* -----------------------------------------------------------------------------
  start_dhcp_server(void)

  Notes:  
  
 -----------------------------------------------------------------------------*/
void start_dhcp_server(void) 
{
	// initialize the tcp stack
	esp_netif_init();
	// stop DHCP server
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	// assign a static IP to the network interface
	tcpip_adapter_ip_info_t info;
	memset(&info, 0, sizeof(info));
	IP4_ADDR(&info.ip, 192, 168, 0, 1);
	IP4_ADDR(&info.gw, 192, 168, 0, 1); //ESP acts as router, so gw addr will be its own addr
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
	// start the DHCP server   
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
	printf("DHCP server started \n");
}
/* -----------------------------------------------------------------------------
  printStationList(void)

 print the list of connected stations  
  
 -----------------------------------------------------------------------------*/
void printStationList(void) 
{
	wifi_sta_list_t wifi_sta_list;
	tcpip_adapter_sta_list_t adapter_sta_list;
   
	memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
	memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

	// Give some time to gather intel of whats connected
	vTaskDelay(500 / portTICK_PERIOD_MS);
	
	ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));	
	ESP_ERROR_CHECK(tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list));

	printf(" Connected Station List:\n");
	printf("--------------------------------------------------\n");
	
	if (adapter_sta_list.num >= 1)
	{
		for (int i = 0; i < adapter_sta_list.num; i++) 
		{
		
			tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
			printf("%d - mac: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x - IP: %s\n",
				i + 1,
				station.mac[0],
				station.mac[1],
				station.mac[2],
				station.mac[3],
				station.mac[4],
				station.mac[5],
				ip4addr_ntoa(&(station.ip)));
		}

		printf("\r\n");
	}
	else
	{
		printf("No Sations Connected\r\n");
	}

}
/* -----------------------------------------------------------------------------
  init_wifi_softap(void)

  Notes:  
  
  // https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/wifi/esp_wifi.html
  // https://github.com/sankarcheppali/esp_idf_esp32_posts/blob/master/esp_softap/main/esp_softap.c
  
 -----------------------------------------------------------------------------*/
void init_wifi_softap(void *arg)
{
	esp_log_level_set("wifi", ESP_LOG_NONE);    // disable wifi driver logging
	
	
	
	wifi_event_group = xEventGroupCreate();
	
	ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
	
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	//ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	
	
	// configure the wifi connection and start the interface
	wifi_config_t ap_config = {
		.ap = {
			.ssid = CONFIG_AP_SSID,
		.password = CONFIG_AP_PASSPHARSE,
		.ssid_len = 0,
		.channel = 0,
		.authmode = AP_AUTHMODE,
		.ssid_hidden = AP_SSID_HIDDEN,
		.max_connection = AP_MAX_CONNECTIONS,
		.beacon_interval = AP_BEACON_INTERVAL,			
		},
	};
	
	
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
	
	ESP_ERROR_CHECK(esp_wifi_start());

	start_dhcp_server();
	
	printf("ESP WiFi started in AP mode \n");
	
	
	 
	//  Spin up a task to show who connected or disconected
	 
	 
	//xTaskCreate(&print_sta_info, "print_sta_info", 4096, NULL, 1, NULL);

	// https://demo-dijiudu.readthedocs.io/en/latest/api-reference/wifi/esp_wifi.html#_CPPv225esp_wifi_set_max_tx_power6int8_t
	// This can only be placed after esp_wifi_start();
	ESP_ERROR_CHECK(esp_wifi_set_max_tx_power(8));
	
}
/* -----------------------------------------------------------------------------
  init_wifi_station(void)

  Notes:  
  
  // https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/wifi/esp_wifi.html
  
 -----------------------------------------------------------------------------*/
void init_wifi_station(void *arg)
{
	esp_log_level_set("wifi", ESP_LOG_NONE);     // disable wifi driver logging
	
	wifi_event_group = xEventGroupCreate();
	
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
	
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &ip_event_handler,
                                                        NULL,
                                                        NULL));


	//ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	
	
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_STATION_SSID,
		.password = CONFIG_STATION_PASSPHRASE
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI("WiFi", "Station Set to SSID:%s Pass:%s\r\n", CONFIG_STATION_SSID, CONFIG_STATION_PASSPHRASE);

}
/* -----------------------------------------------------------------------------
  print_sta_info(void)

  Notes:  
 
 -----------------------------------------------------------------------------*/
void print_sta_info(void *pvParam) 
{
	printf("print_sta_info task started \n");
	
	while (1) 
	{	
		EventBits_t staBits = xEventGroupWaitBits(wifi_event_group, AP_CLIENT_CONNECTED_BIT | AP_CLIENT_DISCONNECTED_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
		
		if ((staBits & AP_CLIENT_CONNECTED_BIT) != 0)
		{
			printf("New station connected\n\n");
		}
		else
		{
			printf("A station disconnected\n\n");
		}
		
		printStationList();
	}
}

void wifi_ap_sta(void *pvParam)
{
	EventBits_t bits;
	if((boot_ap == 0xaa) && (boot_sta == 0xaa))
	{
		bits = xEventGroupWaitBits(wifi_event_group, \
			WIFI_STA_CONNECTED_BIT, \
			pdFALSE,                               \
			pdFALSE,                               \
			190000 / portTICK_PERIOD_MS);
		if((bits & WIFI_STA_CONNECTED_BIT) == WIFI_STA_CONNECTED_BIT) 
		{
			while(1)
			{
				gpio_set_level(19, 0);//橙
				vTaskDelay(100 / portTICK_PERIOD_MS);
				gpio_set_level(19, 1);//橙
				vTaskDelay(6 / portTICK_PERIOD_MS);
			}
		} 
		else
		{
			gpio_set_level(19, 0);//橙
			gpio_set_level(18, 0);//白
			boot_ap = 0x55;
			boot_sta = 0x55;
			const int deep_sleep_sec = 10;
    		ESP_LOGI("WiFi", "Entering deep sleep for %d seconds", deep_sleep_sec);
    		esp_deep_sleep(1000000LL * deep_sleep_sec);	
		} 
		vTaskDelete(NULL);
	}
	else
	{
		xEventGroupWaitBits(wifi_event_group, \
			AP_CLIENT_CONNECTED_BIT, \
			pdFALSE,                               \
			pdFALSE,                               \
			portMAX_DELAY);
		while(1)
		{
			gpio_set_level(18, 0);//橙
			vTaskDelay(100 / portTICK_PERIOD_MS);
			gpio_set_level(18, 1);//橙
			vTaskDelay(6 / portTICK_PERIOD_MS);
		}
		vTaskDelete(NULL);
	}
	vTaskDelete(NULL);
}

esp_err_t storage_Write_WifiModes_Flag(uint8_t u8Flag)
{
	nvs_handle_t my_handle;
	esp_err_t err = ESP_OK;
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) 
	{
		ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
		//ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
	}
	else 
	{	
		err = nvs_set_u8(my_handle, STORAGE_KEY_wifi_ap_sta, u8Flag);
		if(err != ESP_OK)
		{
			ESP_LOGE(TAG, "NVS Write Failed!");
		}
		if(err == ESP_OK)
		{
			//Commit written value to ensure it's updated in NVS
			err = nvs_commit(my_handle);
			if(err != ESP_OK)
			{
				ESP_LOGE(TAG, "NVS Commit Failed!");
			}
		}
		nvs_close(my_handle); //Close
	}	
}

esp_err_t storage_Read_WifiModes_Flag(uint8_t *u8Flag)
{
	nvs_handle_t my_handle;
	esp_err_t err = ESP_OK;
	// Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) 
    {
       printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else 
    {
        printf("Done\n");

        // Read
        printf("Reading restart counter from NVS ... \n");
        
        err = nvs_get_u8(my_handle, STORAGE_KEY_wifi_ap_sta, u8Flag);
        switch (err) 
        {
            case ESP_OK:
                printf("Done\n");
                printf("Restart counter = %d\n", *u8Flag);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
        // Close
        nvs_close(my_handle);
    }
}

static void WIFI_vConfigSoftAP(void)
{
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	// configure the wifi connection and start the interface
	wifi_config_t ap_config = {
		.ap = {
			.ssid = CONFIG_AP_SSID,
		.password = CONFIG_AP_PASSPHARSE,
		.ssid_len = 0,
		.channel = 0,
		.authmode = AP_AUTHMODE,
		.ssid_hidden = AP_SSID_HIDDEN,
		.max_connection = AP_MAX_CONNECTIONS,
		.beacon_interval = AP_BEACON_INTERVAL,			
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	start_dhcp_server();
	printf("ESP WiFi started in AP mode \n");
	//  Spin up a task to show who connected or disconected
	//xTaskCreate(&print_sta_info, "print_sta_info", 4096, NULL, 1, NULL);
	// https://demo-dijiudu.readthedocs.io/en/latest/api-reference/wifi/esp_wifi.html#_CPPv225esp_wifi_set_max_tx_power6int8_t
	// This can only be placed after esp_wifi_start();
	ESP_ERROR_CHECK(esp_wifi_set_max_tx_power(8));
}
static void WIFI_vConfigStation(void)
{
	esp_err_t err = ESP_OK;
//ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	wifi_config_t wifi_config = {
		.sta = {
					.ssid = "",
					.password = "",
					/* Setting a password implies station will connect to all security modes including WEP/WPA.
					* However these modes are deprecated and not advisable to be used. Incase your Access point
					* doesn't support WPA2, these mode can be enabled by commenting below line */
					//.threshold.authmode = WIFI_AUTH_WPA2_PSK,
					.pmf_cfg = {
							.capable = true,
							.required = false
					},
				},
	};
	//read from NVS the Wifi SSID and Password string and their length
	err = storage_errReadWifiSSID((char*)u8TempBuffer, &u8Length);
	memcpy(wifi_config.sta.ssid, u8TempBuffer, u8Length);
	#if 0
	printf("%s%s%s%d \n", "Wifi : ", u8TempBuffer, " loaded: ", err);
	printf("Connecting to %s ", wifi_config.sta.ssid);
	printf("ssid length %d \n", u8Length); 
	#endif

	memset(u8TempBuffer,0,sizeof(u8TempBuffer));
	u8Length = 0;	
	err = storage_errReadWifiPass((char*)u8TempBuffer, &u8Length);
	memcpy(wifi_config.sta.password, u8TempBuffer, u8Length);
	#if 0
	printf("%s%s%s%d \n", "Wifi PASS: ", u8TempBuffer, " loaded: ", err);
	printf(" using password %s\n", wifi_config.sta.password);
	printf("pass length %d", u8Length);
	#endif
	
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	ESP_LOGI("WiFi", "Station Set to SSID:%s Pass:%s\r\n", CONFIG_STATION_SSID, CONFIG_STATION_PASSPHRASE);
}

void mywifi_init(void)
{
	nvs_handle_t my_handle;
	uint8_t u8Length = 0;
	uint8_t u8TempBuffer[33] = {};
	uint8_t WifiModes;//0 sta. 1 ap
	esp_err_t err；

	/* Create the event group to handle wifi events */
    wifi_event_group = xEventGroupCreate();

	esp_log_level_set("wifi", ESP_LOG_NONE);    // disable wifi driver logging
	ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
	
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	//ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &ip_event_handler,
                                                        NULL,
                                                        NULL));

	if(WifiModes != 0)
	{
		//init_wifi_softap(&OTA_server);
		WIFI_vConfigSoftAP();
		
	}
	else
	{
		//init_wifi_station(&OTA_server);
		WIFI_vConfigStation();
	}
	//xTaskCreate(&wifi_ap_sta, "wifi_ap_sta", 2048, NULL, ESP_TASK_PRIO_MIN + 1, NULL);
	for(;;)
	{
        if(WifiMode == 0)
        {
            // wait for connection or disconnect event
            EventBits_t bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT | CONNECTION_FAILED_BIT,pdFALSE,pdFALSE,portMAX_DELAY);
            if (bits & CONNECTED_BIT)
            {
                //printf("Connected!\n");
                
            } 
            else if (bits & CONNECTION_FAILED_BIT) 
            {
                xEventGroupClearBits(wifi_event_group, CONNECTION_FAILED_BIT);
                xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
                //If connection is lost clear the provisioning available flag and stop wifi and re-start in AccessPoint Mode
                ESP_LOGI(TAG, "Connection lost! Stopping Wifi to switch to AccessPoint Mode.");
                ESP_ERROR_CHECK(esp_wifi_stop());
				//storage_Write_WifiModes_Flag(0x01);
				storage_errWriteProvisioningAvailableFlag(0x01)；
                vTaskDelay(50 / portTICK_PERIOD_MS);
	   			WIFI_vConfigSoftAP();
            }
            else 
            {
                ESP_LOGE(TAG, "Unexpected event");
            }
        }
        else
        {//////////////////////////////////////////////
			storage_Read_WifiModes_Flag(&WifiMode);
            ESP_LOGV(TAG,"Wifi Mode: %d",WifiMode);
            //received new Provisioning Data, but not tested yet.
            if(enWifiMode == 0)
            {
                ESP_LOGI(TAG, "New Provisioning data received. Stopping Wifi to switch to Station Mode.");
                ESP_ERROR_CHECK(esp_wifi_stop());
                ESP_LOGI(TAG,"Wifi Station initializing!");
				WIFI_vConfigStation();
            }
            else
            {
                /*Do nothing*/
                ESP_LOGV(TAG,"Wifi AP");
            }
        }
		vTaskDelay((*(uint8_t*)pvParameters) / portTICK_PERIOD_MS);
	}
    vTaskDelete(NULL);
}