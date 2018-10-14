//
// Created by snorri on 14.10.2018.
//

#include "telemetry.h"
#include "Logger.h"

static auto logger = GetLogger("telemetry");

#if TELEMETRY_ENABLED
bool ConnectTelemetry() {
    tm_int32 telemetry_memory_size = 8 * 1024 * 1024;
    char* telemetry_memory = (char*)malloc(telemetry_memory_size);
    tmInitialize(telemetry_memory_size, telemetry_memory);

    char exename[1024];
    auto result = readlink("/proc/self/exe", exename, 1024);
    exename[result] = 0;

    auto err = tmOpen(
            0,
            exename,
            __DATE__ " " __TIME__,
            "localhost",
            TMCT_TCP,
            4719,
            TMOF_INIT_NETWORKING,
            100
            );

    logger->debug("tmOpen returned {}", err);

    tmThreadName(
            0,                        // Capture mask (0 means capture everything)
            0,                        // Thread id (0 means use the current thread)
            "Main Thread"             // Name of the thread
    );

    return err == TM_OK;
}

#else
bool ConnectTelemetry() {
    return false;
}
#endif

