#include "configuracion.h"
#include "comunicacion.h"

// selección de condicións booleanas con lonxitude NUMERO_AXUSTES
static bool condicion[NUMERO_AXUSTES];

void cargar_configuracion() {
  // se non hai almacenamento local previo gardamos os valores por defecto
  if(!persist_exists(1024)) {
    persist_write_bool(1024, true); 
    //-----------------------------//
    persist_write_bool(MOSTRAR_DIA, true);
    persist_write_bool(MOSTRAR_DATA, true);
    persist_write_bool(MOSTRAR_CARGA, true);
    persist_write_bool(INVERTIR_CORES, false);
  }

  // obtemos o valor para cada unha das condicións de verdade/falsedade e gardamolo
  // na nosa selección con i = dirección en memoria
  for(int i = 0; i < NUMERO_AXUSTES; i++) {
    condicion[i] = persist_read_bool(i);
  }
}


// obtemos o valor para unha chave dada
bool obter_config(int chave) {
  return condicion[chave];
}

