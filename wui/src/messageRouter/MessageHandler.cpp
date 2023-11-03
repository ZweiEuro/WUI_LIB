#include "messageRouter/MessageHandler.hpp"

namespace wui
{

    bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 int64_t query_id,
                                 const CefString &request,
                                 bool persistent,
                                 CefRefPtr<Callback> callback)
    {
        DLOG(INFO) << "got query" << request.ToString() << " origin " << frame->GetURL().ToString();

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

    bool MessageHandler::addEventListener(const char *eventName, eventListenerFunction_t function)
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

    bool MessageHandler::removeEventListener(const char *eventName)
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