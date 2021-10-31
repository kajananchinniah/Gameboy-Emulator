#include "GB/GUI.hpp"

#include <memory>
#include <stdexcept>
#include <string>

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

  void update(uint8_t data[160 * 144 * 4], int pitch) {
    SDL_UpdateTexture(texture_.get(), nullptr, data, 160 * sizeof(uint8_t) * 4);
    SDL_RenderCopy(rend_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(rend_.get());

    // TODO: delete this once debugging done
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        throw std::runtime_error("Done");
      }
    }
  }
};

GUI::GUI() : gui_impl_{std::make_unique<GUIImpl>()} {}
void GUI::init(int window_width, int window_height) {
  gui_impl_->init(window_width, window_height);
}

GUI::~GUI() = default;

void GUI::update(uint8_t data[160 * 144 * 4], int pitch) {
  gui_impl_->update(data, pitch);
}

}  // namespace GB
