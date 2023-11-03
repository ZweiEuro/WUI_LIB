
#include "webUiBinding.hpp"
#include "webUiPrivat.hpp"
namespace wui
{
    bool registerEventListener(const char *eventName, eventListenerFunction_t function)
    {
        if (!browserProcessExists())
        {
            DLOG(WARNING) << "registerEventListener: app does not exist";
            return false;
        }

        return browserProcessApp->addEventListener(eventName, function);
    }

    bool unregisterEventListener(const char *eventName)
    {
        if (!browserProcessExists())
        {
            DLOG(WARNING) << "unregisterEventListener: app does not exist";
            return false;
        }
        return browserProcessApp->removeEventListener(eventName);
    }

    bool sendEvent(const char *eventName, const cJSON *eventPayload)
    {
        if (!browserProcessExists())
        {
            DLOG(WARNING) << "sendEvent: app does not exist";
            return false;
        }

        return browserProcessApp->sendEvent(eventName, eventPayload);
    }

}