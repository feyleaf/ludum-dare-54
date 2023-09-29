#include "global.h"
#include "canvas.h"

int ic::Canvas::getCanvasWidth()
{
    return canvasScreen.mode.width;
}

int ic::Canvas::getCanvasHeight()
{
    return canvasScreen.mode.height;
}

sf::FloatRect ic::Canvas::canvasToRect()
{
    return sf::FloatRect(0,0, (float)getCanvasWidth(), (float)getCanvasHeight());
}

sf::String ic::Canvas::getCanvasNickname()
{
    return canvasScreen.nickname;
}

sf::VideoMode ic::Canvas::getDefaultVideoMode()
{
    return canvasScreen.mode;
}

sf::VideoMode ic::Canvas::getAppropriateVideoMode()
{
    float factorReturn = 1.0f;
    float factorSeek = factorReturn;
    unsigned int demo = sf::VideoMode::getDesktopMode().height;
    unsigned int real = getCanvasHeight();
    factorSeek = float(demo) / float(real);
    if (factorSeek < 1.0f)
    {
        if (factorSeek < 0.5f)
        {
            if (factorSeek < 0.25f)
            {
                factorReturn = 0.0125f;
            }
            factorReturn = 0.25f;
        }
        factorReturn = 0.5f;
    }
    else
    {
        if (factorSeek > 2.0f)
        {
            if (factorSeek > 3.0f)
            {
                if (factorSeek > 4.0f)
                {
                    factorReturn = 4.0f;
                }
                factorReturn = 3.0f;
            }
            factorReturn = 2.0f;
        }
    }
    sf::VideoMode ret = {
        unsigned int(getCanvasWidth() * factorReturn),
        unsigned int(getCanvasHeight() * factorReturn),
        canvasScreen.mode.bitsPerPixel
    };
    return ret;
}

sf::View ic::Canvas::canvasToView()
{
    sf::View ret = sf::View(sf::FloatRect(0.0f, 0.0f, float(canvasScreen.mode.width), float(canvasScreen.mode.height)));
    return ret;
}
