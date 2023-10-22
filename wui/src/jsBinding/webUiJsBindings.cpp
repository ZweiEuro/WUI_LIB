
#include "webUiJsBindings.hpp"
#include "webUiPrivat.hpp"
namespace wui
{
    bool registerEventListener(const char *eventName, eventListenerFunction_t function)
    {
        if (!appExists())
        {
            DLOG(WARNING) << "registerEventListener: app does not exist";
            return false;
        }

        return app->GetWUIRenderProcessHandler()->addEventListener(eventName, function);
    }

    bool unregisterEventListener(const char *eventName)
    {
        if (!appExists())
        {
            DLOG(WARNING) << "unregisterEventListener: app does not exist";
            return false;
        }

        return app->GetWUIRenderProcessHandler()->removeEventListener(eventName);
    }

}