#include "config.h"

static bool array[NUMERO_AXUSTES];

void config_init() {
  if(!persist_exists(1024)) {
    rexistro("almacenamento persistente non detectado: poblando valores iniciais");
    persist_write_bool(1024, true);
    persist_write_bool(MOSTRAR_DIA, true);
    persist_write_bool(MOSTRAR_DATA, true);
    persist_write_bool(MOSTRAR_CARGA, true);
    persist_write_bool(INVERTIR_CORES, false);
  } else {
    rexistro("detectado almacenamento persistente!");
  }

  for(int i = 0; i < NUMERO_AXUSTES; i++) {
    array[i] = persist_read_bool(i);
  }
}

bool config_get(int key) {
  return array[key];
}

