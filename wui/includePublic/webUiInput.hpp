
#include <cstdint>

/**
 * Type definition for CEF input handling (needed as mirror for CEF)
 * Why is this mirrored? Decoupling from CEF headers.
 */

namespace wui
{
    //
    // Mouse Events
    //

    // Based on cef_mouse_event_t
    typedef struct _wui_mouse_event_t
    {
        ///
        /// X coordinate relative to the left side of the view.
        ///
        int x;

        ///
        /// Y coordinate relative to the top side of the view.
        ///
        int y;

        ///
        /// Bit flags describing any pressed modifier keys. See
        /// cef_event_flags_t for values.
        ///
        uint32_t modifiers;
    } wui_mouse_event_t;

    typedef enum
    {
        MBT_LEFT = 0,
        MBT_MIDDLE,
        MBT_RIGHT,
    } wui_mouse_button_type_t;

    /**
     * @brief Send a mouse click event to the web ui.
     * @param force If true, the event will be sent even if the mouse is not over the UI.
     *
     * @return true if the event was taken over a pixel that has UI content. False otherwise.
     */
    bool CEFSendMouseClickEvent(const wui_mouse_event_t &event, wui_mouse_button_type_t type, bool mouseUp, int clickCount, bool force = false);

    /**
     * @brief Send a mouse move event to the web ui.
     *        Move commands should always be forwarded to the UI (else hover effects break).
     *
     */
    void CEFSendMouseMoveEvent(const wui_mouse_event_t &event, bool mouseLeave);

    /**
     * @brief Send a mouse wheel event to the web ui.
     * @param force If true, the event will be sent even if the mouse is not over the UI.
     *
     * @return true if the event was taken over a pixel that has UI content. False otherwise.
     */
    bool CEFSendMouseWheelEvent(const wui_mouse_event_t &event, int deltaX, int deltaY, bool force = false);

    //
    // Key events
    //

    ///
    /// Key event types.
    ///
    typedef enum
    {
        ///
        /// Notification that a key transitioned from "up" to "down".
        ///
        KEYEVENT_RAWKEYDOWN = 0,

        ///
        /// Notification that a key was pressed. This does not necessarily correspond
        /// to a character depending on the key and language. Use KEYEVENT_CHAR for
        /// character input.
        ///
        KEYEVENT_KEYDOWN,

        ///
        /// Notification that a key was released.
        ///
        KEYEVENT_KEYUP,

        ///
        /// Notification that a character was typed. Use this for text input. Key
        /// down events may generate 0, 1, or more than one character event depending
        /// on the key, locale, and operating system.
        ///
        KEYEVENT_CHAR
    } wui_key_event_type_t;

    ///
    /// Structure representing keyboard event information.
    ///
    typedef struct _wui_key_event_t
    {
        ///
        /// The type of keyboard event.
        ///
        wui_key_event_type_t type;

        ///
        /// Bit flags describing any pressed modifier keys. See
        /// cef_event_flags_t for values.
        ///
        uint32_t modifiers;

        ///
        /// The Windows key code for the key event. This value is used by the DOM
        /// specification. Sometimes it comes directly from the event (i.e. on
        /// Windows) and sometimes it's determined using a mapping function. See
        /// WebCore/platform/chromium/KeyboardCodes.h for the list of values.
        ///
        int windows_key_code;

        ///
        /// The actual key code genenerated by the platform.
        ///
        int native_key_code;

        ///
        /// Indicates whether the event is considered a "system key" event (see
        /// http://msdn.microsoft.com/en-us/library/ms646286(VS.85).aspx for details).
        /// This value will always be false on non-Windows platforms.
        ///
        int is_system_key;

        ///
        /// The character generated by the keystroke.
        ///
        char16_t character;

        ///
        /// Same as |character| but unmodified by any concurrently-held modifiers
        /// (except shift). This is useful for working out shortcut keys.
        ///
        char16_t unmodified_character;

        ///
        /// True if the focus is currently on an editable field on the page. This is
        /// useful for determining if standard key events should be intercepted.
        ///
        int focus_on_editable_field;
    } wui_key_event_t;

    void CEFSendKeyEvent(const wui_key_event_t &event);
}