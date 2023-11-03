#pragma once
#include "messageRouter/SingleEventMessageHandler.hpp"
#include "messageRouter/PersistentCallbackMessageHandler.hpp"

namespace wui
{

    struct MessageHandlers
    {
        std::shared_ptr<SingleEventMessageHandler> single_event_message_handler;
        std::shared_ptr<PersistentCallbackMessageHandler> persistent_callback_message_handler;
    };
}