#pragma once

#include "Client.hpp"
#include <algorithm>
#include "include/wrapper/cef_helpers.h"

namespace wui
{
    const char kTestMessageName[] = "MessageRouterTest";

    class MessageHandler : public CefMessageRouterBrowserSide::Handler
    {
    public:
        MessageHandler() {}

        // Called due to cefQuery execution in message_router.html.
        bool OnQuery(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     int64_t query_id,
                     const CefString &request,
                     bool persistent,
                     CefRefPtr<Callback> callback) override;

    private:
        MessageHandler(const MessageHandler &) = delete;
        MessageHandler &operator=(const MessageHandler &) = delete;
    };
}