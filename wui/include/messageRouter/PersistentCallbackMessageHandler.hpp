#pragma once

#include "webUiBinding.hpp"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

#include <map>
#include <algorithm>

namespace wui
{

    class PersistentCallbackMessageHandler : public CefMessageRouterBrowserSide::Handler
    {
    private:
        // Eventname <-> { query_id, callback }
        std::map<std::string, std::pair<int64_t, CefRefPtr<Callback>>> js_event_callbacks_;

        PersistentCallbackMessageHandler(const PersistentCallbackMessageHandler &) = delete;
        PersistentCallbackMessageHandler &operator=(const PersistentCallbackMessageHandler &) = delete;

    public:
        PersistentCallbackMessageHandler() {}

        // Called due to cefQuery execution in message_router.html.
        bool OnQuery(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     int64_t query_id,
                     const CefString &request,
                     bool persistent,
                     CefRefPtr<Callback> callback) override;

        void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             int64_t query_id) override;

        bool sendEvent(const char *eventName, const cJSON *eventPayload);
    };
}