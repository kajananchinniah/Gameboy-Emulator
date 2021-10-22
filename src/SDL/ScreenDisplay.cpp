#include "GB/ScreenDisplay.hpp"

#include <iostream>
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

struct ScreenDisplay::ScreenDisplayImpl {
  // std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
  // std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> rend_;
  SDL_Window *window_;
  SDL_Renderer *rend_;

  std::string window_title_{"Gameboy Emulator"};

  ScreenDisplayImpl() {}
  // : window_{nullptr, SDL_DestroyWindow},
  // rend_{nullptr, SDL_DestroyRenderer} {}
  void init(int window_width, int window_height) {
    CHECK_SDL(SDL_Init(SDL_INIT_VIDEO));
    window_ = SDL_CreateWindow(window_title_.c_str(), 0, 0, window_width,
                               window_height, SDL_WINDOW_SHOWN);
    CHECK_ALLOC(window_);

    uint32_t render_flags =
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    int render_index = -1;
    rend_ = SDL_CreateRenderer(window_, render_index, render_flags);
  }

  ~ScreenDisplayImpl() {
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(rend_);
    SDL_Quit();
  }

  void update(uint8_t data[160][144][3], int scale) {
    SDL_Rect cell;
    for (int i = 0; i < 160; ++i) {
      for (int j = 0; j < 144; ++j) {
        cell.x = i * scale;
        cell.y = j * scale;
        cell.w = scale;
        cell.h = scale;
        int r = (int)data[i][j][0];
        int g = (int)data[i][j][0];
        int b = (int)data[i][j][0];
        SDL_SetRenderDrawColor(rend_, data[i][j][0], data[i][j][1],
                               data[i][j][2], 255);
        SDL_RenderFillRect(rend_, &cell);
      }
    }
    SDL_RenderPresent(rend_);
  }
};

ScreenDisplay::ScreenDisplay() { display_impl_ = new ScreenDisplayImpl(); }
void ScreenDisplay::init(int window_width, int window_height) {
  display_impl_->init(window_width, window_height);
}

void ScreenDisplay::update(uint8_t data[160][144][3], int scale) {
  display_impl_->update(data, scale);
}

}  // namespace GB
