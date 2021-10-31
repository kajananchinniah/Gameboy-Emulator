#ifndef INCLUDE_GB_GUI_HPP_
#define INCLUDE_GB_GUI_HPP_

#include <cstdint>
#include <memory>

namespace GB {

class GUI {
 public:
  GUI();
  ~GUI();
  void init(int window_width, int window_height);
  void update(uint8_t data[160 * 144 * 4], int scale);

 private:
  struct GUIImpl;
  std::unique_ptr<GUIImpl> gui_impl_;
};

}  // namespace GB

#endif  // INCLUDE_GB_GUI_HPP_
