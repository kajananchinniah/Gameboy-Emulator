#include "GB/GUI.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "SDL2/SDL.h"

namespace {

#define CHECK_SDL(result) \
  { checkSDL(result, __LINE__, __FILE__); }
inline void checkSDL(int status, int line_number, const char *file_name) {
  if (status < 0) {
    throw std::runtime_error("SDL failed at: " + std::to_string(line_number) +
                             " in file: " + std::string(file_name) +
                             "! Error: " + std::string(SDL_GetError()));
  }
}

#define CHECK_ALLOC(ptr) \
  { checkAlloc(ptr, __LINE__, __FILE__); }
inline void checkAlloc(void *ptr, int line_number, const char *file_name) {
  if (!ptr) {
    throw std::runtime_error(
        "Error: memory allocation failed at: " + std::to_string(line_number) +
        "in file: " + std::string(file_name));
  }
}

}  // namespace

namespace GB {

struct GUI::GUIImpl {
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> rend_;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture_;
  std::string window_title_{"Gameboy Emulator"};
  static constexpr uint32_t kGBWidth{160};
  static constexpr uint32_t kGBHeight{144};
  bool should_quit{false};
  bool should_save{false};

  GUIImpl()
      : window_{nullptr, SDL_DestroyWindow},
        rend_{nullptr, SDL_DestroyRenderer},
        texture_{nullptr, SDL_DestroyTexture} {}

  void init(int window_width, int window_height) {
    CHECK_SDL(SDL_Init(SDL_INIT_VIDEO));
    window_.reset(SDL_CreateWindow(
        window_title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height, SDL_WINDOW_SHOWN));
    CHECK_ALLOC(window_.get());

    uint32_t render_flags =
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    int render_index = -1;
    rend_.reset(SDL_CreateRenderer(window_.get(), render_index, render_flags));
    CHECK_ALLOC(rend_.get());

    texture_.reset(SDL_CreateTexture(rend_.get(), SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING, kGBWidth,
                                     kGBHeight));
    CHECK_ALLOC(texture_.get());
    SDL_SetWindowResizable(window_.get(), SDL_TRUE);
  }

  ~GUIImpl() {
    rend_.reset();
    window_.reset();
    texture_.reset();
    SDL_Quit();
  }

  void update(std::vector<uint8_t> data, int pitch) {
    SDL_UpdateTexture(texture_.get(), nullptr, data.data(), pitch);
    SDL_RenderCopy(rend_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(rend_.get());
  }

  JoyPadButton getKeyboardInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          should_quit = true;
          should_save = true;
          return JoyPadButton::kNoInput;
        }
        case SDL_KEYDOWN:
          return getKeyboardDownInput(event);
        case SDL_KEYUP:
          return getKeyboardUpInput(event);
      }
    }
    return JoyPadButton::kNoInput;
  }

  JoyPadButton getKeyboardDownInput(SDL_Event event) {
    switch (event.key.keysym.scancode) {
      case SDL_SCANCODE_ESCAPE: {
        should_quit = true;
        return JoyPadButton::kNoInput;
      }
      case SDL_SCANCODE_RIGHT:
        return JoyPadButton::kRightButtonPressed;
      case SDL_SCANCODE_LEFT:
        return JoyPadButton::kLeftButtonPressed;
      case SDL_SCANCODE_UP:
        return JoyPadButton::kUpButtonPressed;
      case SDL_SCANCODE_DOWN:
        return JoyPadButton::kDownButtonPressed;
      case SDL_SCANCODE_Z:
        return JoyPadButton::kAButtonPressed;
      case SDL_SCANCODE_X:
        return JoyPadButton::kBButtonPressed;
      case SDL_SCANCODE_SPACE:
        return JoyPadButton::kSelectButtonPressed;
      case SDL_SCANCODE_RETURN:
        return JoyPadButton::kStartButtonPressed;
      default:
        return JoyPadButton::kNoInput;
    }
  }

  JoyPadButton getKeyboardUpInput(SDL_Event event) {
    switch (event.key.keysym.scancode) {
      case SDL_SCANCODE_RIGHT:
        return JoyPadButton::kRightButtonReleased;
      case SDL_SCANCODE_LEFT:
        return JoyPadButton::kLeftButtonReleased;
      case SDL_SCANCODE_UP:
        return JoyPadButton::kUpButtonReleased;
      case SDL_SCANCODE_DOWN:
        return JoyPadButton::kDownButtonReleased;
      case SDL_SCANCODE_Z:
        return JoyPadButton::kAButtonReleased;
      case SDL_SCANCODE_X:
        return JoyPadButton::kBButtonReleased;
      case SDL_SCANCODE_SPACE:
        return JoyPadButton::kSelectButtonReleased;
      case SDL_SCANCODE_RETURN:
        return JoyPadButton::kStartButtonReleased;
      default:
        return JoyPadButton::kNoInput;
    }
  }

  bool shouldQuit() { return should_quit; }
  bool shouldSave() { return should_save; }
};

GUI::GUI() : gui_impl_{std::make_unique<GUIImpl>()} {}
void GUI::init(int window_width, int window_height) {
  gui_impl_->init(window_width, window_height);
}

GUI::~GUI() = default;

void GUI::update(std::vector<uint8_t> data, int pitch) {
  gui_impl_->update(data, pitch);
}

JoyPadButton GUI::getKeyboardInput() { return gui_impl_->getKeyboardInput(); }
bool GUI::shouldQuit() { return gui_impl_->shouldQuit(); }
bool GUI::shouldSave() { return gui_impl_->shouldSave(); }

}  // namespace GB
