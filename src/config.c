#include "config.h"

// Almacenamento persistente empregando o Storage API de Pebble

// a meirande parte das configuracións que queremos implementar
// son booleanas, polo que empregaremos un array de lonxitude NUMERO_AXUSTES
static bool array[NUMERO_AXUSTES];

void config_init() {
  if(!persist_exists(1024)) {       // -> Comprobamos que non hai marca de instalacion
    persist_write_bool(1024, true); // e escribimos nos unha para futuras comprobacións

    // BOOLEANAS
    persist_write_bool(MOSTRAR_DIA, true);
    persist_write_bool(MOSTRAR_DATA, true);
    persist_write_bool(MOSTRAR_CARGA, true);
    persist_write_bool(INVERTIR_CORES, false);
  }

  for(int i = 0; i < NUMERO_AXUSTES; i++) {
    array[i] = persist_read_bool(i);
  }
}

bool config_get(int key) {
  return array[key];
}

