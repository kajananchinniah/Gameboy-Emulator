#include <cstdlib>

#include "GB/GUI.hpp"
#include "GB/MMU.hpp"

namespace {

bool isDirectionalInput(GB::JoyPadButton input) {
  bool direction_input = false;
  direction_input =
      direction_input | (input == GB::JoyPadButton::kRightButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kRightButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kRightButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kRightButtonPressed);
  return direction_input;
}

bool isActionInput(GB::JoyPadButton input) {
  bool direction_input = false;
  direction_input =
      direction_input | (input == GB::JoyPadButton::kAButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kBButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kSelectButtonPressed);
  direction_input =
      direction_input | (input == GB::JoyPadButton::kStartButtonPressed);
  return direction_input;
}

}  // namespace

namespace GB {

void MMU::updateJoyPadState(JoyPadButton input) {
  if (input == JoyPadButton::kNoInput) {
    return;
  } else if (input < JoyPadButton::kNoInput) {
    handleJoyPadRelease(input);
  } else {
    handleJoyPadPress(input);
  }
}

void MMU::handleJoyPadRelease(JoyPadButton input) {
  uint8_t bit_position = static_cast<uint8_t>(std::abs(input) - 1);
  joypad_state = setBit(bit_position, joypad_state);
}

void MMU::handleJoyPadPress(JoyPadButton input) {
  uint8_t bit_position = static_cast<uint8_t>(input - 1);
  bool is_button_not_held = checkBit(bit_position, joypad_state);
  joypad_state = resetBit(bit_position, joypad_state);
  if (shouldRequestJoyPadInterrupt(input, is_button_not_held)) {
    setJoypadInterruptRequest();
  }
}

bool MMU::shouldRequestJoyPadInterrupt(JoyPadButton input,
                                       bool is_button_not_held) {
  if (isDirectionalInput(input) && isDirectionalSelectButtonsEnabled() &&
      is_button_not_held) {
    return true;
  } else if (isActionInput(input) && isActionSelectButtonsEnabled() &&
             is_button_not_held) {
    return true;
  }
  return false;
}

bool MMU::isActionSelectButtonsEnabled() {
  uint8_t JOYP = memory.at(JOYP_addr);
  return !checkBit(4, JOYP);
}

bool MMU::isDirectionalSelectButtonsEnabled() {
  uint8_t JOYP = memory.at(JOYP_addr);
  return !checkBit(5, JOYP);
}

uint8_t MMU::getJOYPRegister() {
  uint8_t JOYP = memory.at(JOYP_addr);
  JOYP = JOYP | 0xC0;  // Set the top 2 bits
  if (isDirectionalSelectButtonsEnabled()) {
    uint8_t top = joypad_state >> 4;
    JOYP = (JOYP & 0xF0) | top;
  } else if (isActionSelectButtonsEnabled()) {
    uint8_t bottom = joypad_state & 0x0F;
    JOYP = (JOYP & 0xF0) | bottom;
  }
  return JOYP;
}

}  // namespace GB
