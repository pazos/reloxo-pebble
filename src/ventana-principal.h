#pragma once

#include "comun.h" // definicións comúns, incluir primeiro
#include "configuracion.h" // necesario para acceder á funcion obter_config()

#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))

void iniciar_esfera();