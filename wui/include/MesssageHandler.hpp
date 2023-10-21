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
                     CefRefPtr<Callback> callback) override
        {
            DLOG(INFO) << "got query" << request.ToString();

            const std::string &message_name = request;
            if (message_name.find(kTestMessageName) == 0)
            {
                // Reverse the string and return.
                std::string result = message_name.substr(sizeof(kTestMessageName));
                std::reverse(result.begin(), result.end());
                callback->Success(result);
                return true;
            }

            return false;
        }

    private:
        MessageHandler(const MessageHandler &) = delete;
        MessageHandler &operator=(const MessageHandler &) = delete;
    };
}