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

    void RendererHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {

        // Memory is unsafe
        if (this->resizing)
        {
            DLOG(INFO) << "OnPaint: resizing";
            return;
        }

        assert(destinationPixelBuffer != nullptr && *destinationPixelBuffer != nullptr && "destinationPixelBuffer is nullptr");

        DLOG(INFO)
            << "OnPaint: " << type << " " << width << " " << height;

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

#if 0 // Clean draw only dirty areas

        // Draw all the dirty rects into the current target buffer then swap buffers
        for (auto const rect : dirtyRects)
        {

            DLOG(INFO) << "dirty Rect: " << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << " " << width << " " << height;

            // the buffer always holds the complete image
            // so we need to calculate the offset and only use overwrite dirty rects

            // dirty rect RGBA buffer

            const int dirty_size = rect.width * rect.height * 4;

            auto dirty_buffer = new uint8_t[dirty_size];
            memset(dirty_buffer, 0, dirty_size);

            // Draw each pixel individually (coordinates are else wrong), thinkg overdrawing a moving window in one block
            for (int dirty_y = 0; dirty_y < rect.y; dirty_y++)
            {
                for (int dirty_x = 0; dirty_x < rect.x; dirty_x++)
                {
                    const size_t destination_x = dirty_x + rect.x;
                    const size_t destination_y = dirty_y + rect.y;

                    const size_t destination_offset = (destination_y * width + destination_x) * 4;
                    const size_t source_offset = (dirty_y * rect.width + dirty_x) * 4;

                    ((uint8_t *)currentlyDrawTargetBuffer)[destination_offset + 0] = ((uint8_t *)buffer)[source_offset * 4 + 3];
                    ((uint8_t *)currentlyDrawTargetBuffer)[destination_offset + 1] = ((uint8_t *)buffer)[source_offset * 4 + 0];
                    ((uint8_t *)currentlyDrawTargetBuffer)[destination_offset + 2] = ((uint8_t *)buffer)[source_offset * 4 + 1];
                    ((uint8_t *)currentlyDrawTargetBuffer)[destination_offset + 3] = ((uint8_t *)buffer)[source_offset * 4 + 2];
                }
            }
        }

#else

        // bad idea block draw
        auto buffer_rgba = new uint8_t[pixelBufferSize];
        memset(buffer_rgba, 0, pixelBufferSize);

        for (int i = 0; i < width * height; i++)
        {

            buffer_rgba[i * 4 + 0] = ((uint8_t *)buffer)[i * 4 + 3];
            buffer_rgba[i * 4 + 1] = ((uint8_t *)buffer)[i * 4 + 0];
            buffer_rgba[i * 4 + 2] = ((uint8_t *)buffer)[i * 4 + 1];
            buffer_rgba[i * 4 + 3] = ((uint8_t *)buffer)[i * 4 + 2];

            /*
            B  -> A
            G  -> R
            R  -> G
            A  -> B

            */
        }

        memcpy(currentlyDrawTargetBuffer, (void *)((size_t)buffer_rgba), pixelBufferSize);

#endif

        // Flip the buffers
        currentlyShowingBuffer1 = !currentlyShowingBuffer1;
        (*destinationPixelBuffer) = currentlyDrawTargetBuffer;
    }
}