#pragma once

#include "common.h"
#include "config.h"

#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))

void main_window_push();