#include "MessageHandler.hpp"

namespace wui
{

    bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 int64_t query_id,
                                 const CefString &request,
                                 bool persistent,
                                 CefRefPtr<Callback> callback)
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
}