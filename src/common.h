#pragma once

#include <pebble.h>
#include <stdarg.h>

#define DEBUG 1 // mostra mensaxes de rexistro da esfera mediante APP_LOG

// macros útiles:
#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))
#define rexistro(...) if (DEBUG == 1) { app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__); }

// valores persistentes (almacentamento local)
#define MOSTRAR_DIA		0
#define MOSTRAR_DATA	1
#define MOSTRAR_CARGA   2
#define INVERTIR_CORES  3
#define NUMERO_AXUSTES  4