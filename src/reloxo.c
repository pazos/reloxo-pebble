#include "comun.h"
#include "eventos.h"
#include "ventana.h"

int main() {
	LOG("abrindo esfera");
	app_message_register_inbox_received(mensaxeria);
    app_message_open(38, 38); // engade 176B de memoria non liberable...
	iniciar_esfera();
	app_event_loop();
	LOG("pechando esfera");
}