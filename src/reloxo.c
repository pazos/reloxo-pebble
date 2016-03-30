#include "comun.h"
#include "ventana.h"

int main() {
  LOG("reloxo dixital - github.com/pazos/reloxo-pebble <paziusss@gmail.com> - GPLv2");
  DEBUG("abrindo esfera");
  iniciar_esfera();
  app_event_loop();
  DEBUG("pechando esfera");
}
