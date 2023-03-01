#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#ifdef CONFIG_EXAMPLE_USE_CERT_BUNDLE 
#include "esp_crt_bundle.h"
#endif
#include "cJSON.h"
#include "protocol_examples_common.h"
#include "wifi.h"

#include "ota_json.h"

#define HASH_LEN 32

#define FIRMWARE_VERSION	0.1
#define UPDATE_JSON_URL		"https://192.168.1.112:8070/ota_test.json"
// #define BLINK_GPIO 			2
// #define BLINK_INTERVAL 		1000
static const char *TAG = "ota_example";
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");


// receive buffer
char rcv_buffer[200];

// esp_http_client event handler
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{	
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
		ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client)) {
			strncpy(rcv_buffer, (char*)evt->data, evt->data_len);
		}
		break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

// Check update task
// downloads every 30sec the json file with the latest firmware
void check_update_task(void *pvParameter) {
	
	while(1) {
        
		printf("Looking for a new firmware...\n");
		 esp_http_client_config_t config = {
         .url = UPDATE_JSON_URL,
         .event_handler = _http_event_handler,
		 .cert_pem = (char *)server_cert_pem_start,
		 };

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
     	config.skip_cert_common_name_check = true;
#endif
		esp_http_client_handle_t client = esp_http_client_init(&config);
	
		// downloading the json file
		esp_err_t err = esp_http_client_perform(client);
		if(err == ESP_OK) {
			
			// parse the json file
		cJSON *json = cJSON_Parse(rcv_buffer);
		if (json == NULL)
			printf("downloaded file is not a valid json, aborting...\n");
		else
		{
			cJSON *version = cJSON_GetObjectItemCaseSensitive(json, "version");
			cJSON *file = cJSON_GetObjectItemCaseSensitive(json, "file");

			// check the version
			if (!cJSON_IsNumber(version))
				printf("unable to read new version, aborting...\n");
			else
			{

				double new_version = version->valuedouble;
				if (new_version > FIRMWARE_VERSION)
				{

					printf("current firmware version (%.1f) is lower than the available one (%.1f), upgrading...\n", FIRMWARE_VERSION, new_version);
					if (cJSON_IsString(file) && (file->valuestring != NULL))
					{
						printf("downloading and installing new firmware (%s)...\n",file->valuestring);
						esp_http_client_config_t new_http_client_config = {
								.url = file->valuestring,
								.cert_pem = (char *)server_cert_pem_start,
						};
						esp_https_ota_config_t ota_client_config = {
							.http_config = &new_http_client_config,
						};
							
							esp_err_t ret = esp_https_ota(&ota_client_config);
							if (ret == ESP_OK) {
								printf("OTA OK, restarting...\n");
								esp_restart();
							} else {
								printf("OTA failed...\n");
							}
						}
						else printf("unable to read the new file name, aborting...\n");
					}
					else printf("current firmware version (%.1f) is greater or equal to the available one (%.1f), nothing to do...\n", FIRMWARE_VERSION, new_version);
				}
			}
		}
		else {
			printf("unable to download the json file, aborting...\n");
		}
		
		// cleanup
		esp_http_client_cleanup(client);
		printf("\n");
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}

void ota_update() {
	ESP_LOGI(TAG, "OTA example app_main start");
    // Initialize NVS.

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
#if CONFIG_EXAMPLE_CONNECT_WIFI
    /* Ensure to disable any WiFi power save mode, this allows best throughput
     * and hence timings for overall OTA operation.
     */
	esp_wifi_set_mode(WIFI_MODE_APSTA);
    esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
#endif // CONFIG_EXAMPLE_CONNECT_WIFI
	//ESP_ERROR_CHECK(esp_wifi_get_mode(wifi_mode_t * mode));
	// wifi_initialise();
	// wifi_wait_connected();
	//connect_wifi();
	 	// printf("Connected to wifi network\n");

	xTaskCreate(&check_update_task, "check_update_task", 8192, NULL, 5, NULL);
}