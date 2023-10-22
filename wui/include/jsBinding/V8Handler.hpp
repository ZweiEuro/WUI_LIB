
#pragma once

#include "include/cef_v8.h"
#include "webUiJsBindings.hpp"
#include <map>

namespace wui
{

    // Handler implements JS to CEF side binding
    // A handler is the "CEF side" implementation of a function and we can bind it to a js object
    class V8Handler : public CefV8Handler
    {
    private:
        IMPLEMENT_REFCOUNTING(V8Handler);

        std::map<std::string, eventListenerFunction_t> eventListeners_;

    public:
        V8Handler()
        {
            DLOG(INFO) << "Created V8Handler";
        }

        bool Execute(const CefString &name,
                     CefRefPtr<CefV8Value> object,
                     const CefV8ValueList &arguments,
                     CefRefPtr<CefV8Value> &retval,
                     CefString &exception) override;

        // WUI functions
        bool addEventListener(const char *eventName, eventListenerFunction_t function);
        bool removeEventListener(const char *eventName);
    };
}