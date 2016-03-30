#pragma once
#include <pebble.h> // SDK de pebble.

#define REXISTRO	1	// -- Información da aplicación
#define DEPURACION	0	// -- Informacion de depuración
#define DIA		0	// -- Comezo almacenamento local	
#define DATA		1	//    ...
#define CARGA		2	//    ...
#define INVERTIR	3	//    ...
#define AXUSTES 	4	// -- Fin de almacenamento local

#define LOG(...) if (REXISTRO == 1) { app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__); }
#define DEBUG(...) if (DEPURACION == 1) { app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__); }
#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))
