/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_DISPLAY_SDL2_HPP
#define MGC_DRIVERS_DISPLAY_SDL2_HPP

#include <SDL2/SDL.h>
#include "mgc_cpp/platform/display/display_driver.hpp"

namespace mgc {
namespace drivers {
namespace platform {
namespace display {

struct DisplaySDL2 : mgc::platform::display::DisplayDriver<DisplaySDL2> {

    DisplaySDL2() = default;
    DisplaySDL2(const DisplaySDL2&) = delete;
    DisplaySDL2& operator=(const DisplaySDL2&) = delete;
    DisplaySDL2(DisplaySDL2&&) = default;
    DisplaySDL2& operator=(DisplaySDL2&&) = default;

    void init_impl(uint32_t clock_rate) {
        SDL_Init(SDL_INIT_VIDEO);
        window_ = SDL_CreateWindow(
            "Test Game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); 
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(renderer_, LOGICAL_WIDTH, LOGICAL_HEIGHT);
        texture_ = SDL_CreateTexture(
            renderer_,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            LOGICAL_WIDTH,
            LOGICAL_HEIGHT
        );
    }

    void reset_impl() {
    }

    uint16_t width_impl() const {
        return LOGICAL_WIDTH;
    }

    uint16_t height_impl() const {
        return LOGICAL_HEIGHT;
    }

    bool transfer_region_blocking_impl(const uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        if (!buffer) return false;

        SDL_Rect rect {
            static_cast<int>(x0),
            static_cast<int>(y0),
            static_cast<int>(x1 - x0 + 1),
            static_cast<int>(y1 - y0 + 1)
        };

        const int pitch = rect.w * bytes_per_pixel;

        if (SDL_UpdateTexture(texture_, &rect, buffer, pitch) != 0) {
            return false;
        }

        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);

        return true;
    }

    bool transfer_region_async_aligned_impl(const uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

        return transfer_region_blocking_impl(buffer, len, x0, y0, x1, y1);
    }

    bool transfer_full_region_blocking_impl(const uint8_t *buffer, size_t len) {
        if (!buffer) return false;

        const int pitch = LOGICAL_WIDTH * bytes_per_pixel;

        if (SDL_UpdateTexture(texture_, nullptr, buffer, pitch) != 0) {
            return false;
        }

        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);
        return true;
    }

    bool transfer_full_region_async_impl(const uint8_t *buffer, size_t len) {
        return transfer_full_region_blocking_impl(buffer, len);
    }

    bool is_busy_impl() const {
        return false;
    }

    void wait_until_idle_interrupt_impl() const {
    }

    void wait_until_idle_polling_impl() const {
    }

    void set_on_transfer_async_completed_impl(void (*cb)(void* context), void* ctx) {
    }

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    static constexpr size_t LOGICAL_WIDTH  = 240;
    static constexpr size_t LOGICAL_HEIGHT = 240;
    static constexpr size_t SCALE          = 2;
    static constexpr size_t WINDOW_WIDTH   = LOGICAL_WIDTH  * SCALE;
    static constexpr size_t WINDOW_HEIGHT  = LOGICAL_HEIGHT * SCALE;
    static constexpr size_t bytes_per_pixel = 2;
};

}// namespace display
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_DISPLAY_SDL2_HPP*/
