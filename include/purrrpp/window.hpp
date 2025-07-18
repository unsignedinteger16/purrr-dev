#ifndef   _PURRR_WINDOW_HPP_
#define   _PURRR_WINDOW_HPP_

namespace purrr {

enum class Key {
  Space        = ' ',
  Apostrophe   = '\'',
  Comma        = ',',
  Minus        = '-',
  Period       = '.',
  Slash        = '/',
  N0           = '0',
  N1           = '1',
  N2           = '2',
  N3           = '3',
  N4           = '4',
  N5           = '5',
  N6           = '6',
  N7           = '7',
  N8           = '8',
  N9           = '9',
  Semicolon    = ';',
  Equal        = '=',
  A            = 'A',
  B            = 'B',
  C            = 'C',
  D            = 'D',
  E            = 'E',
  F            = 'F',
  G            = 'G',
  H            = 'H',
  I            = 'I',
  J            = 'J',
  K            = 'K',
  L            = 'L',
  M            = 'M',
  N            = 'N',
  O            = 'O',
  P            = 'P',
  Q            = 'Q',
  R            = 'R',
  S            = 'S',
  T            = 'T',
  U            = 'U',
  V            = 'V',
  W            = 'W',
  X            = 'X',
  Y            = 'Y',
  Z            = 'Z',
  LeftBracket  = '[',
  Backslash    = '\\',
  RightBracket = ']',
  GraveAccent  = '`',
  LeftControl,
  RightControl,
  LeftShift,
  RightShift,
  LeftAlt,
  RightAlt,
  LeftSuper,
  RightSuper,
  Backspace,
  Delete,
  End,
  Enter,
  Escape,
  Home,
  Insert,
  Menu,
  PageDown,
  PageUp,
  Pause,
  Tab,
  CapsLock,
  NumLock,
  ScrollLock,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  PrintScreen,
  Left,
  Right,
  Up,
  Down,
  NP0,
  NP1,
  NP2,
  NP3,
  NP4,
  NP5,
  NP6,
  NP7,
  NP8,
  NP9,
  NPAdd,
  NPDecimal,
  NPDivide,
  NPEnter,
  NPEqual,
  NPMultiply,
  NPSubtract,
  World2
};

enum class KeyModifier {
  Shift    = (1<<0),
  Control  = (1<<1),
  Alt      = (1<<2),
  Super    = (1<<3),
  CapsLock = (1<<4),
  NumLock  = (1<<5)
};

struct KeyModifiers {
  KeyModifiers() = default;

  inline KeyModifiers(uint8_t bitset)
    : mBitset(bitset)
  {}

  inline void set(KeyModifier modifier) { mBitset |= (uint8_t)modifier; }
  inline void unset(KeyModifier modifier) { mBitset &= ~((uint8_t)modifier); }
  inline bool isSet(KeyModifier modifier) const { return (mBitset & (uint8_t)modifier) == (uint8_t)modifier; }

  inline bool operator[](KeyModifier modifier) const {
    return isSet(modifier);
  }
private:
  uint8_t mBitset = 0;
};

enum class Action {
  Release = PURRR_ACTION_RELEASE,
  Press = PURRR_ACTION_PRESS,
  Repeat = PURRR_ACTION_REPEAT
};

enum class MouseButton {
  Left,
  Middle,
  Right,
  N4,
  N5
};

class Window : public Wrapper<Purrr_Window> {
  friend class Renderer;
public:
  using KeyCallback = void (*)(const Window &, int16_t, Key, Action, KeyModifiers);
  using CursorMoveCallback = void (*)(const Window &, double, double);
  using CursorEnterCallback = void (*)(const Window &);
  using CursorLeaveCallback = void (*)(const Window &);
  using ScrollCallback = void (*)(const Window &, double, double);
  using MouseButtonCallback = void (*)(const Window &, MouseButton, Action, KeyModifiers);

  struct CreateInfo {
    const char *title;
    uint32_t width, height;
    bool depth;
  };
private:
  inline Window(Purrr_Renderer renderer, const CreateInfo &createInfo)
  {
    Result result = purrr_create_window(renderer, (Purrr_Window_Create_Info){
      createInfo.title,
      (int)createInfo.width,
      (int)createInfo.height,
      createInfo.depth
    }, &mHandle);
    if (!result) throw ResultException(result);
  }
public:
  Window() = default;

  inline Window(Purrr_Window window)
    : Wrapper<Purrr_Window>(window)
  {}

  inline Image getImage() const {
    Purrr_Image purrrImage = PURRR_NULL_HANDLE;
    Result result = purrr_get_window_image(mHandle, &purrrImage);
    if (!result) throw ResultException(result);
    return Image(purrrImage);
  }

  inline bool shouldClose() const {
    return purrr_should_window_close(mHandle);
  }

  inline void getSize(int &width, int &height) const {
    Result result = purrr_get_window_size(mHandle, &width, &height);
    if (!result) throw ResultException(result);
  }

  inline bool isKeyDown(Key key) const {
    Result result = purrr_is_window_key_down(mHandle, (Purrr_Key)key);
    if (!result) throw ResultException(result);
    return result == Result::True;
  }

  inline bool isKeyUp(Key key) const {
    Result result = purrr_is_window_key_up(mHandle, (Purrr_Key)key);
    if (!result) throw ResultException(result);
    return result == Result::True;
  }

  inline bool isMouseButtonDown(MouseButton button) const {
    Result result = purrr_is_window_mouse_button_down(mHandle, (Purrr_Mouse_Button)button);
    if (!result) throw ResultException(result);
    return result == Result::True;
  }

  inline bool isMouseButtonUp(MouseButton button) const {
    Result result = purrr_is_window_mouse_button_up(mHandle, (Purrr_Mouse_Button)button);
    if (!result) throw ResultException(result);
    return result == Result::True;
  }

  inline void getCursorPos(double &xpos, double &ypos) {
    Result result = purrr_get_window_cursor_pos(mHandle, &xpos, &ypos);
    if (!result) throw ResultException(result);
  }

  inline void setCursorPos(double xpos, double ypos) {
    Result result = purrr_set_window_cursor_pos(mHandle, xpos, ypos);
    if (!result) throw ResultException(result);
  }

  inline void setKeyCallback(KeyCallback cb)                 { purrr_set_window_key_callback(mHandle, (Purrr_Window_Key_Callback)cb); }
  inline void setCursorMoveCallback(CursorMoveCallback cb)   { purrr_set_window_cursor_move_callback(mHandle, (Purrr_Window_Cursor_Move_Callback)cb); }
  inline void setCursorEnterCallback(CursorEnterCallback cb) { purrr_set_window_cursor_enter_callback(mHandle, (Purrr_Window_Cursor_Enter_Callback)cb); }
  inline void setCursorLeaveCallback(CursorLeaveCallback cb) { purrr_set_window_cursor_leave_callback(mHandle, (Purrr_Window_Cursor_Leave_Callback)cb); }
  inline void setScrollCallback(ScrollCallback cb)           { purrr_set_window_scroll_callback(mHandle, (Purrr_Window_Scroll_Callback)cb); }
  inline void setMouseButtonCallback(MouseButtonCallback cb) { purrr_set_window_mouse_button_callback(mHandle, (Purrr_Window_Mouse_Button_Callback)cb); }

  inline void setUserPointer(void *userPointer) { purrr_set_window_user_pointer(mHandle, userPointer); }
  inline void *getUserPointer() const { return purrr_get_window_user_pointer(mHandle); }

  inline Program createProgram(const Program::CreateInfo &createInfo) {
    return Program((Purrr_Handle)((Purrr_Window)mHandle), createInfo);
  }
public:
  Window(Window &&other) = default;
  Window &operator=(Window &&other) = default;
public:
  inline static double getTime() { return purrr_get_windows_time(); }
  inline static void poll() { purrr_poll_windows(); }
  inline static void wait() { purrr_wait_windows(); }
public:
  virtual void destroy() override {
    purrr_destroy_window(mHandle);
  }
};

}

#endif // _PURRR_WINDOW_HPP_