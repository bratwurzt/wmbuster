#include "../wmbus_app_i.h"
#include "../subghz/wmbus_worker.h"

#include <furi.h>
#include <cli/cli.h>
#include <string.h>

static void wmbus_cli_print_usage(FuriString* out) {
    furi_string_cat_printf(
        out,
        "Usage:\r\n"
        "  wmbuster scan [t1|c1|ct|s1]\r\n"
        "  wmbuster stats\r\n"
        "  wmbuster stop\r\n");
}
