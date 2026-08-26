#include "../wmbus_app_i.h"
#include "../subghz/wmbus_worker.h"

#include <furi.h>
#include <stdio.h>
#include <string.h>

#include <applications/services/cli/cli.h>
#include <toolbox/cli/cli_registry.h>
#include <toolbox/cli/cli_command.h>

static CliRegistry* wmbus_cli_registry = NULL;

static void wmbus_cli_print_usage(void) {
    printf(
        "usage: wmbuster <command>\r\n"
        "commands:\r\n"
        "  scan   start wM-Bus receiver\r\n"
        "  stop   stop wM-Bus receiver\r\n"
        "  stats  show receiver statistics\r\n");
}

static void wmbus_cli_command(
    PipeSide* pipe,
    FuriString* args,
    void* context) {

    (void)pipe;

    WmbusApp* app = context;
    const char* cmd = furi_string_get_cstr(args);

    if(strcmp(cmd, "scan") == 0) {
        wmbus_scanning_start(app);
        printf("wM-Bus scan started\r\n");
        return;
    }

    if(strcmp(cmd, "stop") == 0) {
        wmbus_scanning_stop(app);
        printf("wM-Bus scan stopped\r\n");
        return;
    }

    if(strcmp(cmd, "stats") == 0) {
        WmbusWorkerStats stats;
        wmbus_worker_get_stats(app->worker, &stats);

        printf(
            "sync=%lu decoded_a=%lu decoded_b=%lu\r\n"
            "crc_fail=%lu 3of6_err=%lu fifo_overflow=%lu\r\n"
            "telegrams=%lu meters=%lu\r\n",
            (unsigned long)stats.sync_locks,
            (unsigned long)stats.decoded_a,
            (unsigned long)stats.decoded_b,
            (unsigned long)stats.crc_fails,
            (unsigned long)stats.three_of_six_err,
            (unsigned long)stats.fifo_overflows,
            (unsigned long)app->total_telegrams,
            (unsigned long)app->meter_count);

        return;
    }

    wmbus_cli_print_usage();
}

void wmbus_cli_register(WmbusApp* app) {
    if(wmbus_cli_registry) return;

    wmbus_cli_registry = furi_record_open(RECORD_CLI);

    cli_registry_add_command(
        wmbus_cli_registry,
        "wmbuster",
        CliCommandFlagParallelSafe,
        wmbus_cli_command,
        app);
}

void wmbus_cli_unregister(WmbusApp* app) {
    UNUSED(app);

    if(!wmbus_cli_registry) return;

    cli_registry_delete_command(
        wmbus_cli_registry,
        "wmbuster");

    furi_record_close(RECORD_CLI);
    wmbus_cli_registry = NULL;
}