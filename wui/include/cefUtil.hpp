#pragma once

#include "include/cef_command_line.h"

namespace wui
{

    // Create a new CommandLine object for use before CEF initialization.
    CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs &main_args);

    // Process types that may have different CefApp instances.
    enum ProcessType
    {
        PROCESS_TYPE_BROWSER,
        PROCESS_TYPE_RENDERER,
        PROCESS_TYPE_LINUX_ZYGOTE,
        PROCESS_TYPE_OTHER,
    };

    // Determine the process type based on command-line arguments.
    ProcessType GetProcessType(const CefRefPtr<CefCommandLine> &command_line);
}
