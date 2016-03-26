#include "comun.h"
#include "comunicacion.h"
#include "configuracion.h"
#include "ventana-principal.h"

int main() {
cargar_comunicacions();
cargar_configuracion(); 
iniciar_esfera();
esperar_eventos();
}