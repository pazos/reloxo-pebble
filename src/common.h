#pragma once

#include <pebble.h>

#define DEBUG 1
#define LOG(...) if (DEBUG == 1) { app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__); }

#define MOSTRAR_DIA		0
#define MOSTRAR_DATA	1
#define MOSTRAR_CARGA   2
#define INVERTIR_CORES  3
#define NUMERO_AXUSTES  4