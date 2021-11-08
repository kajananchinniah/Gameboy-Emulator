#ifndef INCLUDE_GB_GUI_HPP_
#define INCLUDE_GB_GUI_HPP_

#include <cstdint>
#include <memory>
#include <vector>

namespace GB {

enum JoyPadButton {
  kStartButtonReleased = -8,
  kSelectButtonReleased = -7,
  kBButtonReleased = -6,
  kAButtonReleased = -5,
  kDownButtonReleased = -4,
  kUpButtonReleased = -3,
  kLeftButtonReleased = -2,
  kRightButtonReleased = -1,
  kNoInput = 0,
  kRightButtonPressed = 1,
  kLeftButtonPressed = 2,
  kUpButtonPressed = 3,
  kDownButtonPressed = 4,
  kAButtonPressed = 5,
  kBButtonPressed = 6,
  kSelectButtonPressed = 7,
  kStartButtonPressed = 8,
};

class GUI {
 public:
  GUI();
  ~GUI();
  void init(int window_width, int window_height);
  void update(std::vector<uint8_t> data, int scale);
  JoyPadButton getKeyboardInput();
  bool shouldQuit();

 private:
  struct GUIImpl;
  std::unique_ptr<GUIImpl> gui_impl_;
};

}  // namespace GB

#endif  // INCLUDE_GB_GUI_HPP_
