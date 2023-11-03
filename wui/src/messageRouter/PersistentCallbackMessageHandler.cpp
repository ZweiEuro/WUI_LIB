#include "messageRouter/PersistentCallbackMessageHandler.hpp"

namespace wui
{

    bool PersistentCallbackMessageHandler::OnQuery(CefRefPtr<CefBrowser> browser,
                                                   CefRefPtr<CefFrame> frame,
                                                   int64_t query_id,
                                                   const CefString &request,
                                                   bool persistent,
                                                   CefRefPtr<Callback> callback)
    {
        if (frame->GetURL().ToString().rfind("file://", 0) != 0)
        {
            DLOG(WARNING) << "got query " << request.ToString() << " origin " << frame->GetURL().ToString() << " but not from file://";
            return false;
        }

        if (persistent == false)
        {
            // only handle persistent queries
            return false;
        }

        DLOG(INFO) << "got persistent query " << request.ToString() << " origin " << frame->GetURL().ToString();

        auto cjson = cJSON_Parse(request.ToString().c_str());

        // check if all necessary properties exist

        if (cjson == nullptr)
        {
            DLOG(WARNING) << "got invalid json";
            callback->Failure(-100, "invalid json");
            return true;
        }

        auto wuiEventName = cJSON_GetObjectItem(cjson, "wuiEventName");
        if (wuiEventName == nullptr)
        {
            DLOG(WARNING) << "got json without wuiEventName";
            callback->Failure(-100, "invalid json");
            goto cleanupCefQuery;
        }

        {
            auto wuiEventAsString = std::string(wuiEventName->valuestring);

            if (js_event_callbacks_.find(wuiEventAsString) != js_event_callbacks_.end())
            {
                // do not blindly overwrite
                DLOG(WARNING) << "JS callback already registered " << wuiEventAsString;
                callback->Failure(-101, "callback " + wuiEventAsString + " is already registered");
                goto cleanupCefQuery;
            }
            else
            {
                // add the callback to the map

                js_event_callbacks_[wuiEventAsString] = std::make_pair(query_id, callback);
                DLOG(INFO) << "registered JS callback " << wuiEventAsString;
                goto cleanupCefQuery;
            }
        }

    cleanupCefQuery:
        cJSON_Delete(cjson);
        return true;
    }

    void PersistentCallbackMessageHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                                                           CefRefPtr<CefFrame> frame,
                                                           int64_t query_id)
    {
        // remove from map if present
        // must search through map by query id

        for (auto it = js_event_callbacks_.begin(); it != js_event_callbacks_.end(); ++it)
        {
            if (it->second.first == query_id)
            {
                js_event_callbacks_.erase(it);
                return;
            }
        }
    }

    bool PersistentCallbackMessageHandler::sendEvent(const char *eventName, const cJSON *eventPayload)
    {

        auto eventListener = js_event_callbacks_.find(eventName);
        if (eventListener == js_event_callbacks_.end())
        {
            DLOG(INFO) << "No Event listener for queried event " << eventName;
            return false;
        }

        auto payloadString = cJSON_PrintUnformatted(eventPayload);
        eventListener->second.second->Success(payloadString);
        free(payloadString);
        return true;
    }
}