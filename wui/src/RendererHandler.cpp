#include <RendererHandler.hpp>

namespace wui
{

    RendererHandler::RendererHandler(void **destinationPixelBuffer, const size_t width, const size_t height) : height(height), width(width), destinationPixelBuffer(destinationPixelBuffer)
    {
        this->resizing = false;

        this->rgbaPixelBuffer1 = calloc(4 * sizeof(char), height * width); // RGBA
        this->rgbaPixelBuffer2 = calloc(4 * sizeof(char), height * width); // RGBA
        (*destinationPixelBuffer) = this->rgbaPixelBuffer1;

        DLOG(INFO) << "RendererHandler: " << height << " " << width << " " << this->rgbaPixelBuffer1 << " " << this->rgbaPixelBuffer2;
    }

    RendererHandler::~RendererHandler()
    {
    }

    void RendererHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
    {
        DLOG(INFO) << "GetViewRect " << this->width << " " << this->height;
        rect = CefRect(0, 0, width, height);
    }

    // buffer is in format BGRA
    void RendererHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {

        // Memory is unsafe
        if (this->resizing)
        {
            DLOG(INFO) << "OnPaint: resizing";
            return;
        }

        assert(destinationPixelBuffer != nullptr && *destinationPixelBuffer != nullptr && "destinationPixelBuffer is nullptr");

        // The user currently draws this to their screen
        void *currentlyViewedBuffer = nullptr;

        // We draw into this buffer
        void *currentlyDrawTargetBuffer = nullptr;

        if (currentlyShowingBuffer1)
        {
            currentlyViewedBuffer = rgbaPixelBuffer1;
            currentlyDrawTargetBuffer = rgbaPixelBuffer2;
        }
        else
        {
            currentlyViewedBuffer = rgbaPixelBuffer2;
            currentlyDrawTargetBuffer = rgbaPixelBuffer1;
        }

        // Copy the current buffer into the one we draw into next
        // Dirty draws would otherwise not be visible and we'd have partial draws on partial buffers

        size_t pixelBufferSize = 4 * sizeof(char) * this->height * this->width;

        memcpy(currentlyDrawTargetBuffer, currentlyViewedBuffer, pixelBufferSize);

        // check if its the entire thing
        if (dirtyRects.size() == 1 && dirtyRects[0].height == height && dirtyRects[0].width == width)
        {
            memcpy(currentlyDrawTargetBuffer, (void *)((size_t)buffer), pixelBufferSize);
        }
        else
        {
            // DLOG(INFO) << "Drawing dirty rects" << dirtyRects.size();
            // Draw all the dirty rects into the current target buffer then swap buffers
            for (auto const rect : dirtyRects)
            {

                // DLOG(INFO) << "dirty Rect: " << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << " " << width << " " << height;
                // Copy over and convert every pixel
                for (int line = 0; line < rect.height; line++)
                {

                    const size_t line_start_pos = (rect.x + rect.y * width + line * width) * 4;

                    const uint8_t *destination = (((uint8_t *)currentlyDrawTargetBuffer) + line_start_pos);
                    const uint8_t *source = (((uint8_t *)buffer) + line_start_pos);

                    memcpy((void *)destination, (void *)source, rect.width);
                }
            }
        }

        // Flip the buffers
        currentlyShowingBuffer1 = !currentlyShowingBuffer1;
        (*destinationPixelBuffer) = currentlyDrawTargetBuffer;
    }

    bool RendererHandler::coordinateEmpty(size_t x, size_t y) const
    {

        for (int i = 0; i < 4; i++)
        {
            if (((uint8_t *)(*destinationPixelBuffer))[(y * width + x) * 4 + i] != 0)
            {
                for (int i = 0; i < 4; i++)
                    DLOG(INFO) << "color: " << i << " " << (int)((uint8_t *)(*destinationPixelBuffer))[(y * width + x) * 4 + i];

                return false;
            }
        }

        return true;
    }
}