#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>
#include <sys/param.h>
#include <esp_http_server.h>
#include "esp_ota_ops.h"
#include "freertos/event_groups.h"

#include "Message.h"
#include "menu.h"
#include "seg7font.h"
#include "SegDynamicScan.h"

void LED_Seg7Menu_Task(void * parameter);