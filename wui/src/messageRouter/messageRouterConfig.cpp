#include "messageRouter/messageRouterConfig.hpp"

namespace wui
{

    CefMessageRouterConfig getMessageRouterConfig()
    {
        CefMessageRouterConfig config;
        config.js_cancel_function = "WuiQueryCanel";
        config.js_query_function = "WuiQuery";
        return config;
    }

}