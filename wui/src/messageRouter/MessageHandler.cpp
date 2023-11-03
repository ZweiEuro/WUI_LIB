#include "messageRouter/MessageHandler.hpp"

namespace wui
{

    bool SingleEventMessageHandler::OnQuery(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            int64_t query_id,
                                            const CefString &request,
                                            bool persistent,
                                            CefRefPtr<Callback> callback)
    {
        if (frame->GetURL().ToString().rfind("file://", 0) != 0)
        {
            DLOG(WARNING) << "got query" << request.ToString() << " origin " << frame->GetURL().ToString() << " but not from file://";
            return false;
        }

        if (persistent)
        {
            // persistent queries are not handled by this handler
            return false;
        }

        DLOG(INFO) << "got query" << request.ToString() << " origin " << frame->GetURL().ToString();

        auto cjson = cJSON_Parse(request.ToString().c_str());

        // check if all necessary properties exist

        if (cjson == nullptr)
        {
            DLOG(WARNING) << "got invalid json";
            callback->Failure(-100, "invalid json");
            return true;
        }

        auto wuiEventName = cJSON_GetObjectItem(cjson, "wuiEventName");
        auto wuiEventPayload = cJSON_GetObjectItem(cjson, "wuiEventPayload");

        if (wuiEventName == nullptr || wuiEventPayload == nullptr)
        {
            DLOG(WARNING) << "got json without wuiEventName or wuiEventPayload";
            callback->Failure(-100, "invalid json");
            cJSON_Delete(cjson);
            return true;
        }

        // Ok now find the handler and process it
        cJSON *successObject = cJSON_CreateObject();
        std::string exceptionStr;
        int retVal = 0;

        auto eventListener = eventListeners_.find(wuiEventName->valuestring);
        if (eventListener == eventListeners_.end())
        {
            DLOG(INFO) << "No Event listener for queried event " << wuiEventName->valuestring;
            DLOG(INFO) << "got " << eventListeners_.size() << " event listeners in handler " << this;
            callback->Failure(-101, "No listener registered for event" + std::string(wuiEventName->valuestring));
            goto cleanupCefQuery;
        }

        retVal = eventListener->second(wuiEventPayload, successObject, exceptionStr);

        if (retVal != 0)
        {
            callback->Failure(retVal, exceptionStr);
            goto cleanupCefQuery;
        }
        else
        {
            auto printed = cJSON_Print(successObject);
            callback->Success(printed);
            free(printed);
            goto cleanupCefQuery;
        }

    cleanupCefQuery:
        cJSON_Delete(cjson);
        cJSON_Delete(successObject);
        return true;
    }

    bool SingleEventMessageHandler::addEventListener(const char *eventName, eventListenerFunction_t function)
    {
        if (eventListeners_.find(eventName) != eventListeners_.end())
        {
            DLOG(WARNING) << "addEventListener: event listener already exists, unregister it first";
            return false;
        }

        DLOG(INFO) << "Registered event listener for " << eventName << " to " << this;

        eventListeners_[eventName] = function;
        return true;
    }

    bool SingleEventMessageHandler::removeEventListener(const char *eventName)
    {
        if (eventListeners_.find(eventName) == eventListeners_.end())
        {
            DLOG(WARNING) << "removeEventListener: event listener does not exist";
            return false;
        }

        eventListeners_.erase(eventName);
        return true;
    }

}