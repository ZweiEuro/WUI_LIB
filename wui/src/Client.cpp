#include "Client.hpp"
#include "RenderHandler.hpp"

namespace wui
{

    Client::Client(void **destinationPixelBuffer, const size_t height, const size_t width)

    {
        this->m_renderHandler = new RenderHandler(destinationPixelBuffer, height, width);
    }

    CefRefPtr<CefRenderHandler> Client::GetRenderHandler()
    {
        return m_renderHandler;
    }

    CefRefPtr<RenderHandler> Client::GetWUIRenderHandler()
    {
        return m_renderHandler;
    }

}