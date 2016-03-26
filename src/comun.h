#pragma once

#include <pebble.h> // SDK de pebble. 

// -- Almacenamento local	
#define MOSTRAR_DIA		0
#define MOSTRAR_DATA	1
#define MOSTRAR_CARGA   2
#define INVERTIR_CORES  3
#define NUMERO_AXUSTES  4

// -- Rexistros da aplicación
#define HAI_QUE_REXISTRAR 	1
#define LOG(...) if (HAI_QUE_REXISTRAR == 1) { app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__); }

#define esperar_eventos() app_event_loop()