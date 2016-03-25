#pragma once

#include "common.h"

void config_init();
void debug(char *log, ... );
bool config_get(int key);
