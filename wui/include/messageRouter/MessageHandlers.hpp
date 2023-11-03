#pragma once
#include "MessageHandler.hpp"

namespace wui
{

    struct MessageHandlers
    {
        std::shared_ptr<SingleEventMessageHandler> single_event_message_handler;
    };
}