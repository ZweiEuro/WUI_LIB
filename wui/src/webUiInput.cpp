#include <webUiInput.hpp>
#include <webUiPrivat.hpp>
#include <include/cef_browser.h>

namespace wui
{
    bool CEFSendMouseClickEvent(const wui_mouse_event_t &event, wui_mouse_button_type_t type, bool mouseUp,
                                int clickCount, bool force)
    {

        if (browserProcessApp.get() == nullptr)
        {
            DLOG(INFO) << "App is nullptr";
            return false;
        }

        // DLOG(INFO) << "Send Mouse Click Event: "
        //            << " X: " << event.x
        //            << " Y: " << event.y
        //            << " Type: " << type
        //            << " MouseUp: " << mouseUp
        //            << " ClickCount: " << clickCount;

        if (force == false && browserProcessApp->GetWUIBrowserProcessHandler()->GetClient()->GetWUIRenderHandler()->coordinateEmpty(event.x,
                                                                                                                                    event.y))
        {
            DLOG(INFO) << "Coordinate Empty";
            return false;
        }

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browserProcessApp->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->SendMouseClickEvent(CefMouseEvent(cefEvent), (CefBrowserHost::MouseButtonType)type, mouseUp, clickCount);
        return true;
    }

    void CEFSendMouseMoveEvent(const wui_mouse_event_t &event, bool mouseLeave)
    {

        if (browserProcessApp.get() == nullptr)
        {
            DLOG(INFO) << "App is nullptr";
            return;
        }
        // DLOG(INFO) << "Send Mouse Move Event: "
        //            << " X: " << event.x
        //            << " Y: " << event.y
        //            << " MouseLeave: " << mouseLeave;

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browserProcessApp->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->SendMouseMoveEvent(CefMouseEvent(cefEvent), mouseLeave);
    }

    bool CEFSendMouseWheelEvent(const wui_mouse_event_t &event, int deltaX, int deltaY, bool force)
    {
        if (browserProcessApp.get() == nullptr)
        {
            DLOG(INFO) << "App is nullptr";
            return false;
        }

        // DLOG(INFO) << "Send Mouse Wheel Event: "
        //            << " X: " << event.x
        //            << " Y: " << event.y
        //            << " DeltaX: " << deltaX
        //            << " DeltaY: " << deltaY;

        if (force == false && browserProcessApp->GetWUIBrowserProcessHandler()->GetClient()->GetWUIRenderHandler()->coordinateEmpty(event.x,
                                                                                                                                    event.y))
        {
            DLOG(INFO) << "Coordinate Empty";
            return false;
        }

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browserProcessApp->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->SendMouseWheelEvent(CefMouseEvent(cefEvent), deltaX, deltaY);
        return true;
    }

    void CEFSendKeyEvent(const wui_key_event_t &event)
    {
        if (browserProcessApp.get() == nullptr)
        {
            DLOG(INFO) << "App is nullptr";
            return;
        }
        // DLOG(INFO) << "Send Key Event: "
        //            << " Type: " << event.type
        //            << " Modifiers: " << event.modifiers
        //            << " WindowsKeyCode: " << event.windows_key_code
        //            << " NativeKeyCode: " << event.native_key_code
        //            << " IsSystemKey: " << event.is_system_key
        //            << " FocusOnEditableField: " << event.focus_on_editable_field;

        cef_key_event_t cefEvent = {
            .type = (cef_key_event_type_t)event.type,
            .modifiers = event.modifiers,
            .windows_key_code = event.windows_key_code,
            .native_key_code = event.native_key_code,
            .is_system_key = event.is_system_key,
            .character = event.character,
            .unmodified_character = event.unmodified_character,
            .focus_on_editable_field = event.focus_on_editable_field,
        };

        browserProcessApp->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->SendKeyEvent(CefKeyEvent(cefEvent));
    }
}