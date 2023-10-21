#pragma once

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

#include <atomic>
#include <mutex>

namespace wui
{
    /**
     * Rendering Handler to render into a buffer
     * Its an offscreen renderer with internal double buffering
     */
    class RenderHandler : public CefRenderHandler
    {

    private:
        size_t width = 0;
        size_t height = 0;

        // Double-buffering
        bool currentlyShowingBuffer1 = true;
        void *rgbaPixelBuffer1 = nullptr;
        void *rgbaPixelBuffer2 = nullptr;

        void **destinationPixelBuffer = nullptr;

        std::mutex output_buffers_lock;

    public:
        RenderHandler(void **destinationPixelBuffer, const size_t height, const size_t width);
        ~RenderHandler();

        // Interface implementation
        void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

        IMPLEMENT_REFCOUNTING(RenderHandler);

    public:
        // Utility for library functionality

        // Check the coordinate, if all pixel values are 0 we return false
        bool coordinateEmpty(size_t x, size_t y);

        // Resize and change destination buffer IF a new one is given
        // return false if already resizing
        bool resize(size_t newWdith, size_t newHeight, void **newDestinationPixelBuffer = nullptr);
    };

}