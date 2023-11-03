#pragma once

#include "webUiBinding.hpp"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

#include <map>
#include <algorithm>

namespace wui
{

    class SingleEventMessageHandler : public CefMessageRouterBrowserSide::Handler
    {
    private:
        std::map<std::string, eventListenerFunction_t> eventListeners_;

        SingleEventMessageHandler(const SingleEventMessageHandler &) = delete;
        SingleEventMessageHandler &operator=(const SingleEventMessageHandler &) = delete;

    public:
        SingleEventMessageHandler() {}

        // Called due to cefQuery execution in message_router.html.
        bool OnQuery(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     int64_t query_id,
                     const CefString &request,
                     bool persistent,
                     CefRefPtr<Callback> callback) override;

        bool addEventListener(const char *eventName, eventListenerFunction_t function);
        bool removeEventListener(const char *eventName);
    };
}