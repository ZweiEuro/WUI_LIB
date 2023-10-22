#pragma once

#include <cjson/cJSON.h>
#include <string>
namespace wui
{
    typedef void (*eventListenerFunction_t)(const cJSON *eventPayload, cJSON *retVal, std::string &exception);

    bool registerEventListener(const char *eventName, eventListenerFunction_t function);
    bool unregisterEventListener(const char *eventName);
}

// cb8693b058ba302f4829ec6d03f609ac6f848546