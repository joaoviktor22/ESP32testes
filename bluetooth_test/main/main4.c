#include <stdint.h>
#include <string.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include "nvs.h"
 #include "nvs_flash.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
 #include "esp_log.h"
 #include "esp_bt.h"
 #include "esp_bt_main.h"
 #include "esp_gap_bt_api.h"
 #include "esp_bt_device.h"
 #include "esp_spp_api.h"

//#include "time.h"
//#include "sys/time.h"

#define SPP_TAG "SPP_ACCEPTOR_DEMO/Servidor"
#define SPP_SERVER_NAME "SPP_SERVER/ServidorESP"
#define EXAMPLE_DEVICE_NAME "ESP_SPP_ACCEPTOR/Servidor"
#define SPP_SHOW_DATA 0
// #define SPP_SHOW_SPEED 1
#define SPP_SHOW_MODE SPP_SHOW_DATA    /*Choose show mode: show data or speed*/

static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;

// static struct timeval time_new, time_old;
// static long data_num = 0;

static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;

// static void print_speed(void)
// {
//     float time_old_s = time_old.tv_sec + time_old.tv_usec / 1000000.0;
//     float time_new_s = time_new.tv_sec + time_new.tv_usec / 1000000.0;
//     float time_interval = time_new_s - time_old_s;
//     float speed = data_num * 8 / time_interval / 1000.0;
//     ESP_LOGI(SPP_TAG, "speed(%fs ~ %fs): %f kbit/s" , time_old_s, time_new_s, speed);
//     data_num = 0;
//     time_old.tv_sec = time_new.tv_sec;
//     time_old.tv_usec = time_new.tv_usec;
// }

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    char buf[1024];
    char spp_data[256];
    switch (event) {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
        esp_bt_dev_set_device_name(EXAMPLE_DEVICE_NAME);
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE,ESP_BT_GENERAL_DISCOVERABLE);
        esp_spp_start_srv(sec_mask,role_slave, 0, SPP_SERVER_NAME);
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
    case ESP_SPP_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
        break;
    case ESP_SPP_START_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
        break;
    case ESP_SPP_CL_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
        break;
    case ESP_SPP_DATA_IND_EVT:
#if (SPP_SHOW_MODE == SPP_SHOW_DATA)
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len=%d handle=%d",
                 param->data_ind.len, param->data_ind.handle);
        if (param->data_ind.len < 1023) {
            //salvar a string com tamanho exato em buf
            snprintf(buf, (size_t)param->data_ind.len-1, (char *)param->data_ind.data);//coloquei tamanho-1 ao invés de tamanho
            printf("Dados: %s.\n", buf);//printf clássico
            ESP_LOGI(SPP_TAG, "DATA: %s.",buf);//esp logi,funciona melhor para ESP, LOG I(informação)
            //salvar a string em spp_data
            sprintf(spp_data, "Received characters: %d.\n", param->data_ind.len-2);
            //mandar spp_data para celular
            esp_spp_write(param->write.handle, strlen(spp_data), (uint8_t *)spp_data);
        }
        else {
            esp_log_buffer_hex("",param->data_ind.data,param->data_ind.len);
        }
#else
        printf("Modo Comentado de Mostrar Tempo.");
        // gettimeofday(&time_new, NULL);
        // data_num += param->data_ind.len;
        // if (time_new.tv_sec - time_old.tv_sec >= 3) {
        //     print_speed();
        // }
#endif
        break;
    case ESP_SPP_CONG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
        // gettimeofday(&time_old, NULL);
        break;
    default:
        break;
    }
}

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );


    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed\n", __func__);
        return;
    }

    if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed\n", __func__);
        return;
    }

    if (esp_bluedroid_init() != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed\n", __func__);
        return;
    }

    if (esp_bluedroid_enable() != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed\n", __func__);
        return;
    }

    if (esp_spp_register_callback(esp_spp_cb) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed\n", __func__);
        return;
    }

    if (esp_spp_init(esp_spp_mode) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed\n", __func__);
        return;
    }
}
