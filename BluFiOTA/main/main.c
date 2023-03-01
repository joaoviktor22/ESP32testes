/* OTA example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "ota_json.h"
#include "blufi_main.h"

void app_main(void)
{

    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
    //     // partition table. This size mismatch may cause NVS initialization to fail.
    //     // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
    //     // If this happens, we erase NVS partition and initialize NVS again.
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     err = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(err);

    blufi_on();
}
