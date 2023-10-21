#include <RenderHandler.hpp>

namespace wui
{

    RenderHandler::RenderHandler(void **destinationPixelBuffer, const size_t width, const size_t height) : height(height), width(width), destinationPixelBuffer(destinationPixelBuffer)
    {

        this->rgbaPixelBuffer1 = calloc(4 * sizeof(char), height * width);
        this->rgbaPixelBuffer2 = calloc(4 * sizeof(char), height * width);

        assert(this->rgbaPixelBuffer1 != nullptr && "could not allocate rgbaPixelBuffer1");
        assert(this->rgbaPixelBuffer2 != nullptr && "could not allocate rgbaPixelBuffer1");
        (*destinationPixelBuffer) = this->rgbaPixelBuffer1;

        // DLOG(INFO) << "RendererHandler: " << height << " " << width << " " << this->rgbaPixelBuffer1 << " " << this->rgbaPixelBuffer2;
    }

    RenderHandler::~RenderHandler()
    {
    }

    void RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
    {
        // DLOG(INFO) << "GetViewRect " << this->width << " " << this->height;
        rect = CefRect(0, 0, width, height);
    }

    // buffer is in format BGRA
    void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {

        if (width != this->width || height != this->height)
        {
            // This can happen when a resize event was processed fast enough before a onPaint event was finished with the old buffer
            // obviously missaligned buffers are not good
            DLOG(WARNING) << "OnPaint: width and height do not match";
            return;
        }

        // Memory is unsafe
        this->output_buffers_lock.lock();

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

        assert(currentlyViewedBuffer != nullptr && "currentlyViewedBuffer is nullptr");
        assert(currentlyDrawTargetBuffer != nullptr && "currentlyDrawTargetBuffer is nullptr");

        // Copy the current buffer into the one we draw into next
        // Dirty draws would otherwise not be visible and we'd have partial draws on partial buffers

        size_t pixelBufferSize = 4 * sizeof(char) * this->height * this->width;

        memcpy(currentlyDrawTargetBuffer, currentlyViewedBuffer, pixelBufferSize);

        // check if its the entire thing
        if (dirtyRects.size() == 1 && dirtyRects[0].height == height && dirtyRects[0].width == width)
        {
            memcpy(currentlyDrawTargetBuffer, buffer, pixelBufferSize);
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

        this->output_buffers_lock.unlock();
    }

    bool RenderHandler::coordinateEmpty(size_t x, size_t y)
    {
        this->output_buffers_lock.lock();

        for (int i = 0; i < 4; i++)
        {
            if (((uint8_t *)(*destinationPixelBuffer))[(y * width + x) * 4 + i] != 0)
            {
                //  for (int i = 0; i < 4; i++)
                //    { DLOG(INFO) << "color: " << i << " " << (int)((uint8_t *)(*destinationPixelBuffer))[(y * width + x) * 4 + i];}

                this->output_buffers_lock.unlock();
                return false;
            }
        }
        this->output_buffers_lock.unlock();
        return true;
    }

    bool RenderHandler::resize(size_t newWidth, size_t newHeight, void **newDestinationPixelBuffer)
    {

        if (this->width == newWidth && this->height == newHeight)
        {
            // DLOG(INFO) << "No need to resize, same size";
            return false;
        }

        this->output_buffers_lock.lock();

        // DLOG(INFO) << "Resizing to " << newHeight << " " << newWidth;

        // set destination to nullptr
        assert(destinationPixelBuffer != nullptr && "destinationPixelBuffer is nullptr");
        (*destinationPixelBuffer) = nullptr;

        // free existing buffers
        free(this->rgbaPixelBuffer1);
        free(this->rgbaPixelBuffer2);

        // allocate new buffers
        this->rgbaPixelBuffer1 = calloc(1, 4 * newHeight * newWidth);
        this->rgbaPixelBuffer2 = calloc(1, 4 * newHeight * newWidth);

        if (newDestinationPixelBuffer != nullptr)
        {
            this->destinationPixelBuffer = newDestinationPixelBuffer;
        }

        this->currentlyShowingBuffer1 = true;
        (*destinationPixelBuffer) = this->rgbaPixelBuffer1;

        // set new newHeight and newWdith
        this->height = newHeight;
        this->width = newWidth;

        // set resizing to false
        // DLOG(INFO) << "Resizing done";
        this->output_buffers_lock.unlock();
        return true;
    }
}