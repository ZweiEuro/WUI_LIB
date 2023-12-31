#pragma once

#include <string>

namespace wui
{

    /**
     * @brief Initialize the CEF framework
     * Must be the first function to be called.
     *
     */
    bool CEFInit(int argc, char **argv);

    /**
     * @brief Start the web ui.
     * @note Check validity of the pixelbuffer against nullptr
     * @note MUST be called after CEFInit and from the same thread.
     *
     * @param pixelBuffer Destination Pointer. Internally double-buffered. Format:BGRA (low to high)
     * @param initialHeight Initial height of the web ui.
     * @param initialWidth Initial width of the web ui.
     * @param path Relative path to the html folter (default: "html/") that contains a index.html file.
     */
    bool startWui(void **pixelBuffer, const size_t initialWidth, const size_t initialHeight, std::string path = "html/");

    /**
     * @brief Resize the web ui.
     * @note Check validity of the pixelbuffer against nullptr
     *
     * @param newWidth New width of the web ui.
     * @param newHeight New height of the web ui.
     * @param pixelbuffer New Destination Pointer. if nullptr, the old pointer is used. Format:BGRA (low to high)
     */
    void resizeUi(const size_t newWidth, const size_t newHeight, void **newDestinationPixelBuffer = nullptr);

    /**
     * @brief Exit when receiving shutdown command, never returns.
     *
     */
    void CEFRunMessageLoop();

    // What it says on the box
    void CEFShutdown();

}