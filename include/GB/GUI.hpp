#ifndef INCLUDE_GB_GUI_HPP_
#define INCLUDE_GB_GUI_HPP_

#include <cstdint>
#include <memory>
#include <vector>

namespace GB {

class GUI {
 public:
  GUI();
  ~GUI();
  void init(int window_width, int window_height);
  void update(std::vector<uint8_t> data, int scale);

 private:
  struct GUIImpl;
  std::unique_ptr<GUIImpl> gui_impl_;
};

}  // namespace GB

#endif  // INCLUDE_GB_GUI_HPP_
