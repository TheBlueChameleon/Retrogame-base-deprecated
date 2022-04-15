// ========================================================================== //
// Depenencies

// STL
#include <exception>

#include <iostream>

#include <string>
using namespace std::string_literals;

// own
#include "../window.hpp"
#include "visualeffect.hpp"
#include "simplefadeout.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    SimpleFadeout::SimpleFadeout(
        const FadeoutType fadeoutType,
        double milliseconds,
        double fps,
        const SDL_Color color
    ) :
        VisualEffect(fps, fps * milliseconds / 1000.),
        fadeoutType(fadeoutType),
        color(color)
    {}

// ========================================================================== //
// Getters, Setters

    const SDL_Color& SimpleFadeout::getColor() const
    {
        return color;
    }

    void SimpleFadeout::setColor(const SDL_Color& newColor)
    {
        color = newColor;
    }

    SimpleFadeout::FadeoutType SimpleFadeout::getFadeoutType() const
    {
        return fadeoutType;
    }

    void SimpleFadeout::setFadeoutType(FadeoutType newFadeoutType)
    {
        fadeoutType = newFadeoutType;
    }

// ========================================================================== //
// visualEffect interface

    std::function<void (void*)> SimpleFadeout::getRenderer()
    {
        switch (fadeoutType)
        {
            case FadeoutType::Blur:
                return renderBlur;
            case FadeoutType::Pixelate:
                return renderPixelate;
            case FadeoutType::Desaturate:
                return renderDesaturate;
        }

        // cannot happen, but appeases compiler
        return nullptr;
    }

// -------------------------------------------------------------------------- //

    void SimpleFadeout::prepareInstance()
    {
        switch (fadeoutType)
        {
            case FadeoutType::Blur:
                prepareInstanceBlur();
                break;
            case FadeoutType::Pixelate:
                break;
            case FadeoutType::Desaturate:
                break;
        }
    }

// .......................................................................... //

    void SimpleFadeout::tidyUpInstance()
    {
        switch (fadeoutType)
        {
            case FadeoutType::Blur:
                tidyUpInstanceBlur();
                break;

            case FadeoutType::Pixelate:
                break;
            case FadeoutType::Desaturate:
                break;
        }
    }

// ========================================================================== //
// renderers

    // ...................................................................... //
    // Blur

    void SimpleFadeout::prepareInstanceBlur()
    {
        // ensure no violation of boundaries
        auto winWidth  = window->getWidth();
        auto winHeight = window->getHeight();

        if (effectX < 0)
        {
            effectWidth += effectX;
            effectX = 0;
        }
        if (effectX > winWidth)
        {
            effectX = winWidth - 1;
            effectWidth = 0;
        }

        if (effectY < 0)
        {
            effectHeight += effectY;
            effectY = 0;
        }
        if (effectY > winHeight)
        {
            effectY = winHeight - 1;
            effectHeight = 0;
        }

        if (effectWidth <= 0)
        {
            effectWidth = 0;
            return;
        }
        if (effectHeight <= 0)
        {
            effectHeight = 0;
            return;
        }

        // extract metadata
        const auto surfaceData   = reinterpret_cast<Uint8*>(windowSurface->pixels);
        const auto bytesPerPixel = windowSurface->format->BytesPerPixel;
        const auto pitch         = windowSurface->pitch;

        // construct buffer and bufferViews
        buffer.resize(effectHeight * effectWidth * bytesPerPixel);

        const auto bufferData = buffer.begin();
        for (auto row = 0; row < effectHeight; ++row)
        {
            bufferViews.emplace_back(bufferData +  row      * effectWidth * bytesPerPixel,
                                     bufferData + (row + 1) * effectWidth * bytesPerPixel);
        }

        // construct surfaceViews
        for (auto row = 0; row < effectHeight; ++row)
        {
            surfaceViews.emplace_back(surfaceData
                                      + (row + effectY) * pitch
                                      +        effectX  * bytesPerPixel,
                                      effectWidth * bytesPerPixel);
        }
    }

    void SimpleFadeout::tidyUpInstanceBlur()
    {
        buffer      .resize(0);
        bufferViews .resize(0);
        surfaceViews.resize(0);
    }


    void accumulatePixelValuesForBlur(std::span<Uint16>& buffer, const std::span<Uint8>& surfacePixels, const Uint8 bpp, const int offsetX)
    {
        switch (offsetX)
        {
            case 0:
                std::transform(surfacePixels.begin(), surfacePixels.end(),
                               buffer.begin(),
                               buffer.begin(),
                               std::plus<> {});
                break;

            case 1:
                std::transform(surfacePixels.begin() + bpp, surfacePixels.end(),
                               buffer.begin(),
                               buffer.begin(),
                               std::plus<> {});
                std::transform(surfacePixels.end() - bpp, surfacePixels.end(),          // extra weight to border pixels to allow uniform rescaling
                               buffer.end() - bpp,
                               buffer.end() - bpp,
                               std::plus<> {});
                break;

            case -1:
                std::transform(surfacePixels.begin(), surfacePixels.end() - bpp,
                               buffer.begin() + bpp,
                               buffer.begin() + bpp,
                               std::plus<> {});
                std::transform(surfacePixels.begin(), surfacePixels.begin() + bpp,      // extra weight to border pixels to allow uniform rescaling
                               buffer.begin(),
                               buffer.begin(),
                               std::plus<> {});
                break;

            default:
                throw std::invalid_argument(THROWTEXT("Invalid x-offset: "s + std::to_string(offsetX)));
                break;
        }
    }

    void SimpleFadeout::renderBlur(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<SimpleFadeout>(instanceData);

        // nothing to animate? Advance Time and skip the heavy lifting
        if (self.effectWidth * self.effectHeight == 0)
        {
            self.advanceFrame();
            return;
        }

        auto& surface = *self.windowSurface;
        const auto bytesPerPixel = surface.format->BytesPerPixel;

        // if last frame: simply show final colour
        if (self.frameID + 1 == self.totalFrames)
        {
            SDL_Color color = self.getColor();
            win.fbox(self.effectX,     self.effectY,
                     self.effectWidth, self.effectHeight,
                     color);
            self.advanceFrame();
            return;
        }

        // actual blur code
        const auto repeats = std::max(1ul, std::min(self.effectWidth, self.effectHeight) / (10 * self.totalFrames));
        for (auto i = 0u; i < repeats; ++i)
        {
            std::fill(self.buffer.begin(), self.buffer.end(), 0);

            // sum up pixels in buffer
            for (auto row = 0; row < self.effectHeight; ++row)
            {
                // same row blur
                accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  0);
                accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  1);
                accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel, -1);

                // blur to above
                if (row)
                {
                    accumulatePixelValuesForBlur(self.bufferViews[row - 1], self.surfaceViews[row], bytesPerPixel,  0);
                    accumulatePixelValuesForBlur(self.bufferViews[row - 1], self.surfaceViews[row], bytesPerPixel,  1);
                    accumulatePixelValuesForBlur(self.bufferViews[row - 1], self.surfaceViews[row], bytesPerPixel, -1);
                }
                else
                {
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  0);
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  1);
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel, -1);
                }

                // blur to below
                if (row < self.effectHeight - 1)
                {
                    accumulatePixelValuesForBlur(self.bufferViews[row + 1], self.surfaceViews[row], bytesPerPixel,  0);
                    accumulatePixelValuesForBlur(self.bufferViews[row + 1], self.surfaceViews[row], bytesPerPixel,  1);
                    accumulatePixelValuesForBlur(self.bufferViews[row + 1], self.surfaceViews[row], bytesPerPixel, -1);
                }
                else
                {
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  0);
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel,  1);
                    accumulatePixelValuesForBlur(self.bufferViews[row], self.surfaceViews[row], bytesPerPixel, -1);
                }
            }

            // rescale
            std::transform(self.buffer.begin(), self.buffer.end(),
                           self.buffer.begin(),
                           [] (const auto& input)
            {
                return input / 9;
            });

            // copy back into SDL buffer
            // std::copy won't do, as we need conversion back from Uint16 -> Uint8
            for (auto row = 0; row < self.effectHeight; ++row)
            {
                std::transform(self.bufferViews [row].begin(), self.bufferViews[row].end(),
                               self.surfaceViews[row].begin(),
                               [] (const auto& x)
                {
                    return x;
                });
            }
        }

        self.updateStoredTexture();
        self.renderStoredState();

        self.advanceFrame();
    }

    // ...................................................................... //
    // Pixelate

    SDL_Color getAverageColor  (SDL_Surface* surface,
                                const int startX, const int startY,
                                const int width, const int height)
    {
        SDL_Color reVal;

        long long r = 0, g = 0, b = 0;
        const long long area = width * height;

        for     (auto x=startX; x < startX+width ; ++x)
        {
            for (auto y=startY; y < startY+height; ++y)
            {
                const auto thisPixel = getPixelFromSurface(surface, x, y);

                r += thisPixel.r;
                g += thisPixel.g;
                b += thisPixel.b;
            }
        }

        reVal.r = r / area;
        reVal.g = g / area;
        reVal.b = b / area;
        reVal.a = 255;

        return reVal;
    }

    void SimpleFadeout::renderPixelate(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<SimpleFadeout>(instanceData);

        const int pixelWidth  = self.effectWidth  * self.progress + 1;
        const int pixelHeight = self.effectHeight * self.progress + 1;

        // if last frame: simply show final colour
        if (self.frameID + 1 == self.totalFrames)
        {
            SDL_Color color = self.getColor();
            win.fbox(self.effectX,     self.effectY,
                     self.effectWidth, self.effectHeight,
                     color);
            self.advanceFrame();
            return;
        }

        self.renderStoredState();

        for     (auto x=self.effectX; x < self.effectWidth ; x+= pixelWidth)
        {
            for (auto y=self.effectY; y < self.effectHeight; y+= pixelHeight)
            {
                auto color = getAverageColor(self.windowSurface, x, y, pixelWidth, pixelHeight);
                win.fbox(x, y, pixelWidth, pixelHeight, color);
            }
        }

        self.advanceFrame();
    }

    // ...................................................................... //

    void SimpleFadeout::renderDesaturate(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<SimpleFadeout>(instanceData);

        SDL_Color color = self.getColor();
        color.a = self.progress * 255;

        self.renderStoredState();
        win.fbox(self.effectX, self.effectY, self.effectWidth, self.effectHeight, color);

        self.advanceFrame();
    }
}
