#pragma once

#include <cjson/cJSON.h>
#include <string>
#include <functional>
namespace wui
{
    typedef std::function<int(const cJSON *eventPayload, cJSON *successRetObj, std::string &exception)> eventListenerFunction_t;

    // return values unequal 0 are considered errors, the exception and the return code get forwarded to the js binding

    bool registerEventListener(const char *eventName, eventListenerFunction_t function);
    bool unregisterEventListener(const char *eventName);

    bool sendEvent(const char *eventName, const cJSON *eventPayload);
}
