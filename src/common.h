#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))
#define rexistro(...) app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)

#define MOSTRAR_DIA		0
#define MOSTRAR_DATA	1
#define MOSTRAR_CARGA   2
#define INVERTIR_CORES  3
#define NUMERO_AXUSTES  4

#pragma once
#include <pebble.h>
#include <stdio.h>
#include <time.h>