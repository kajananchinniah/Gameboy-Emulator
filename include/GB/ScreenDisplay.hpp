#include <memory>

namespace GB {

class ScreenDisplay {
 public:
  ScreenDisplay();
  void init(int window_width, int window_height);
  void update(uint8_t data[160][144][3], int scale);

 private:
  struct ScreenDisplayImpl;
  // std::unique_ptr<ScreenDisplayImpl> display_impl_;
  ScreenDisplayImpl* display_impl_;
};

}  // namespace GB
