#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include <fstream>
#include <vector>

#include "MMU.hpp"

namespace GB
{

struct Colour {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    enum ColourCode { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK };
    Colour() : red{0}, green{0}, blue{0}
    {
    }
    Colour(uint8_t r, uint8_t g, uint8_t b) : red{r}, green{g}, blue{b}
    {
    }
};

class PPU
{
public:
    explicit PPU(MMU *mmu);
    ~PPU();
    void updatePPU(int clock_cycles);
    unsigned int getLCDViewportWidth();
    unsigned int getLCDViewportHeight();
    std::vector<uint8_t> getDisplayBuffer();
    int getDisplayBufferPitch();
    void saveState(std::ofstream &save_state_file);
    void loadState(std::ifstream &save_state_file);

private:
    std::vector<uint8_t> display_buffer;
    std::vector<uint8_t> display_buffer_colour_id;

    static const unsigned int kLCDViewPortWidth{160};
    static const unsigned int kLCDViewPortHeight{144};
    static const unsigned int kNumDisplayBufferChannels{4};
    static const unsigned int kBackgroundWidth{256};
    static const unsigned int kBackgroundHeight{256};
    static const unsigned int kWindowWidth{256};
    static const unsigned int kWindowHeight{256};
    static const unsigned int kEndOfScanLineCycles{456};
    static const unsigned int kMaxScanLine{153};
    static const unsigned int kRedDisplayBufferIndex{2};
    static const unsigned int kGreenDisplayBufferIndex{1};
    static const unsigned int kBlueDisplayBufferIndex{0};
    static const unsigned int kAlphaDisplayBufferIndex{3};
    static const unsigned int kMode2Cycles{80};
    static const unsigned int kMode3Cycles{252};

    unsigned int ppu_clock_cycles{0};
    uint16_t internal_window_counter{0};
    bool requested_coincidence_interrupt{false};

    MMU *mmu;
    static const uint16_t BGP_addr{0xFF47};
    static const uint16_t OBP0_addr{0xFF48};
    static const uint16_t OBP1_addr{0xFF49};
    static const uint16_t background_map1_addr{0x9C00};
    static const uint16_t background_map2_addr{0x9800};

    // LCD / general
    void resetPPU();
    void updatePPULCD();
    void doOAMScanMode();
    void doDrawingMode();
    void doHBlankMode();
    void doVBlankMode();
    void updateCoincidenceFlag();
    void prepareForNextScanLine();
    void renderScanLine();
    void renderTiles();
    void draw(uint8_t pixel, uint8_t scanline, Colour colour,
              uint8_t colour_id);

    // Background
    void renderBackground();
    uint16_t getBackgroundTileNumberYOffset();
    uint16_t getBackgroundTileNumberXOffset(uint8_t pixel);
    uint16_t getBackgroundMemoryAddress();
    uint16_t getBackgroundTileDataYOffset();
    uint16_t getBackgroundTileDataXOffset(uint8_t pixel);
    uint8_t getBackgroundColourPosition(uint16_t data);

    // Window
    void renderWindow();
    bool shouldRenderWindow();
    uint16_t getWindowTileNumberYOffset();
    uint16_t getWindowTileNumberXOffset(uint8_t pixel);
    uint16_t getWindowMemoryAddress();
    uint16_t getWindowTileDataYOffset();
    uint16_t getWindowTileDataXOffset(uint8_t pixel);
    uint8_t getWindowColourPosition(uint16_t data);

    // Sprite
    struct SpriteOAMEntry {
        uint8_t y;
        uint8_t x;
        uint8_t tile_number;
        uint8_t sprite_flags;
        uint16_t entry_address;
    };
    std::vector<SpriteOAMEntry> sprite_buffer;
    void renderSprites();
    uint16_t getOAMSpriteAddress(uint16_t offset);
    uint8_t getYPositionFromOAM(uint32_t entry);
    uint8_t getXPositionFromOAM(uint32_t entry);
    uint8_t getTileNumberFromOAM(uint32_t entry);
    uint8_t getSpriteFlagsFromOAM(uint32_t entry);
    bool isValidSpriteOAMEntry(uint8_t y, uint8_t x);
    void sortSpriteBufferByPriority();
    static bool compareOAMEntry(const SpriteOAMEntry &left_entry,
                                const SpriteOAMEntry &right_entry);
    void addSpriteToDisplayBuffer(SpriteOAMEntry entry);
    uint8_t getSpriteTileDataYOffset(uint8_t y_position, uint8_t sprite_flags);
    uint16_t getSpriteTileDataAddress(uint8_t tile_number);
    uint8_t getSpriteColourPosition(int8_t tile_pixel, uint8_t sprite_flags);
    uint16_t getSpriteColourAddress(uint8_t sprite_flags);
    uint8_t getSpritePixelLocation(uint8_t x_position, int8_t tile_pixel);
    bool shouldDrawPixel(uint8_t sprite_flags, uint8_t colour_id, uint8_t pixel,
                         uint8_t scanline);
    bool shouldUsePalette1(uint8_t sprite_flags);
    bool shouldSpriteXFlip(uint8_t sprite_flags);
    bool shouldSpriteYFlip(uint8_t sprite_flags);
    bool isBackgroundPrioritized(uint8_t sprite_flags);

    // Misc
    bool shouldUse8000Mode();
    Colour decodeColour(uint8_t colour_id, uint16_t palette_addr);
    Colour getRGBColour(int decoded_colour, uint16_t palette_addr);
    uint8_t getWindowHorizontalPosition();
    uint8_t getWindowVerticalPosition();
    uint8_t get2BPPPixel(uint8_t byte1, uint8_t byte2, int position);
    uint16_t get2BPPPixelRow(uint8_t byte1, uint8_t byte2);
    size_t getFlattenedDisplayBufferIndex(uint8_t pixel, uint8_t scanline);
    size_t getFlattenedDisplayBufferColourIDIndex(uint8_t pixel,
                                                  uint8_t scanline);

    uint8_t getBit(uint8_t n, uint8_t value)
    {
        return ((value >> n) & 0x01);
    }
    bool checkBit(uint8_t n, uint8_t value)
    {
        if ((value >> n) & 0x01) {
            return true;
        } else {
            return false;
        }
    }
};

} // namespace GB

#endif // INCLUDE_GB_PPU_HPP_
