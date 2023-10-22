
#include "jsBinding/V8Handler.hpp"

namespace wui
{

    bool V8Handler::Execute(const CefString &name,
                            CefRefPtr<CefV8Value> object,
                            const CefV8ValueList &arguments,
                            CefRefPtr<CefV8Value> &retval,
                            CefString &exception)
    {
        DLOG(INFO) << "V8Handler::Execute" << name.ToString();

        // 2 arguments, eventName (string), eventPayload (object)
        if (name == "sendEvent")
        {
            // arg check
            if (arguments.size() != 2 || !arguments[0]->IsString() || !arguments[1]->IsObject())
            {
                exception = "Invalid arguments";
                return true;
            }

            // get the correct event handler (if any exists)
            std::string eventName = arguments[0]->GetStringValue().ToString();
            auto eventListener = eventListeners_.find(eventName);
            if (eventListener == eventListeners_.end())
            {
                exception = "Event listener does not exist, event swallowed";
                return true;
            }
            else
            {
                DLOG(INFO) << "V8Handler::Execute: event listener found";

                // convert the event payload to cJSON
                cJSON *eventPayload = cJSON_Parse(arguments[1]->GetStringValue().ToString().c_str());
                cJSON *listenerRetval = cJSON_CreateObject();
                std::string exceptionStr;

                // call the event listener
                eventListener->second(eventPayload, listenerRetval, exceptionStr);
                if (!exceptionStr.empty())
                {
                    exception = exceptionStr;
                    cJSON_Delete(eventPayload);
                    cJSON_Delete(listenerRetval);
                    return true;
                }

                // convert the return value to a string

                // TODO, actually parse a ret val
                char *listenerRetvalStr = cJSON_Print(listenerRetval);

                // return the string
                retval = CefV8Value::CreateString(listenerRetvalStr);

                // cleanup
                free(listenerRetvalStr);
                cJSON_Delete(eventPayload);
                cJSON_Delete(listenerRetval);
                return true;
            }
        }

        if (name == "registerEventListener")
        {
            retval = CefV8Value::CreateString("My Value!");
            return true;
        }

        // Function does not exist.
        return false;
    }

    bool V8Handler::addEventListener(const char *eventName, eventListenerFunction_t function)
    {
        if (eventListeners_.find(eventName) != eventListeners_.end())
        {
            DLOG(WARNING) << "addEventListener: event listener already exists, unregister it first";
            return false;
        }

        eventListeners_[eventName] = function;
        return true;
    }

    bool V8Handler::removeEventListener(const char *eventName)
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