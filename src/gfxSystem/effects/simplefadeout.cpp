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

    void SimpleFadeout::prepareInstanceBlur()
    {
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
            surfaceViews.emplace_back(surfaceData + row * pitch,            // TODO: offset effectX, effectY
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
        auto& self = castToVisualEffect<SimpleFadeout>(instanceData);
        self.advanceFrame();
    }

    /*
            void SimpleFadeout::renderBlur(void* userDataPointer)
            {
                auto [userData, win, self] = unpackUserdataPointer<SimpleFadeout>(userDataPointer);
                const auto [width, height] = win.getDimension();

                auto& surface = *userData.windowSurface;
                const auto bytesPerPixel = surface.format->BytesPerPixel;

                // if last frame: simply show final colour
                if (userData.frameID + 1 == self.totalFrames)
                {
                    SDL_Color color = self.getColor();
                    win.fbox(0, 0, width, height, color);
                    self.progress();
                    return;
                }

                // actual blur code
                const auto repeats = std::max(1ul, std::min(height, width) / (10 * self.totalFrames));
                for (auto i = 0u; i < repeats; ++i)
                {
                    std::fill(self.buffer.begin(), self.buffer.end(), 0);

                    // sum up pixels in buffer
                    for (auto row = 0; row < height; ++row)
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
                        if (row < height - 1)
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
                    for (auto row = 0; row < height; ++row)
                    {
                        std::transform(self.bufferViews [row].begin(), self.bufferViews[row].end(),
                                       self.surfaceViews[row].begin(),
                                       [] (const auto& x)
                        {
                            return x;
                        });
                    }
                }

                userData.updateTexture();
                self.renderStoredState();

                self.progress();
            }
    */

    // .......................................................................... //

    void SimpleFadeout::renderPixelate(void* instanceData)
    {
        auto& self = castToVisualEffect<SimpleFadeout>(instanceData);
        self.advanceFrame();
    }
    /*
        void SimpleFadeout::renderPixelate(void* userDataPointer)
        {
            auto [userData, win, self] = unpackUserdataPointer<SimpleFadeout>(userDataPointer);

            const auto [width, height]  = win.getDimension();
            const int pixelWidth  = width  * userData.progress + 1;
            const int pixelHeight = height * userData.progress + 1;

            // if last frame: simply show final colour
            if (userData.frameID + 1 == self.totalFrames)
            {
                SDL_Color color = self.getColor();
                win.fbox(0, 0, width, height, color);
                self.progress();
                return;
            }

            // acutal pixelate code
            auto getAverageColor = [](SDL_Surface* surface, const int startX, const int startY, const int width, const int height)
            {
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

                SDL_Color reVal;
                reVal.r = r / area;
                reVal.g = g / area;
                reVal.b = b / area;
                reVal.a = 255;

                return reVal;
            };

            self.renderStoredState();

            for     (auto x=0u; x < width ; x+= pixelWidth)
            {
                for (auto y=0u; y < height; y+= pixelHeight)
                {
                    auto color = getAverageColor(userData.windowSurface, x, y, pixelWidth, pixelHeight);
                    win.fbox(x, y, pixelWidth, pixelHeight, color);
                }
            }

            self.progress();
        }
    */
    // .......................................................................... //

    void SimpleFadeout::renderDesaturate(void* instanceData)
    {
        auto& self = castToVisualEffect<SimpleFadeout>(instanceData);
        auto& win = *self.window;

        SDL_Color color = self.getColor();
        color.a = self.progress * 255;

        self.renderStoredState();
        win.fbox(self.effectX, self.effectY, self.effectWidth, self.effectHeight, color);

        self.advanceFrame();
    }
}
