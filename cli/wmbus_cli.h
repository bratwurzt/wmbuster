#pragma once

#include "../wmbus_app_i.h"

typedef struct WmbusApp WmbusApp;

void wmbus_cli_register(WmbusApp* app);
void wmbus_cli_unregister(WmbusApp* app);

