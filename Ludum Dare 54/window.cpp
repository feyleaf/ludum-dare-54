#include "global.h"

WindowClass::WindowClass() : canvas(ic::vm_sms)
{

}

WindowClass::WindowClass(ic::gameScreen _screen) : canvas(_screen)
{

}

WindowClass::~WindowClass()
{

}

sf::View WindowClass::transformView(sf::View originalView, int winWidth, int winHeight)
{
    sf::View ret = originalView;
    float windowRatio = float(winWidth) / float(winHeight);
    float viewRatio = float(originalView.getSize().x) / float(originalView.getSize().y);
	sf::FloatRect letterBoxingRect = { 0,0,1.0f,1.0f };

    if (windowRatio >= viewRatio)
    {
		letterBoxingRect.width = viewRatio / windowRatio;
		letterBoxingRect.left = (1 - letterBoxingRect.width) / 2.f;
    }
    else
    {
		letterBoxingRect.height = windowRatio / viewRatio;
		letterBoxingRect.top = (1 - letterBoxingRect.height) / 2.f;
    }

    ret.setViewport(letterBoxingRect);
    return ret;
}

sf::Vector2f WindowClass::transformMouse(sf::RenderWindow& _win)
{
    return _win.mapPixelToCoords(sf::Mouse::getPosition(_win));
}

void WindowClass::launchWindow(const sf::String _title)
{
	isFullScreen = false;
	app.create(canvas.getAppropriateVideoMode(), _title, sf::Style::Default);
	videoWidth = canvas.getCanvasWidth();
	videoHeight = canvas.getCanvasHeight();
	gameView = app.getView();
	renewView(videoWidth, videoHeight);
}

void WindowClass::launchFullScreen(const sf::String _title)
{
	isFullScreen = false;
	std::vector<sf::VideoMode> allFSModes = sf::VideoMode::getFullscreenModes();
	sf::VideoMode deskVideo = sf::VideoMode::getDesktopMode();
	sf::VideoMode fullScreen{ 1,1,32 };
	for (auto& k : allFSModes)
	{
		if (k.width == deskVideo.width && k.height == deskVideo.height && k.bitsPerPixel == deskVideo.bitsPerPixel)
			fullScreen = k;
	}
	if (fullScreen.width == 1)
	{
		return;
	}
	app.create(fullScreen, _title, sf::Style::Fullscreen);
	isFullScreen = true;
	renewView(fullScreen.width, fullScreen.height);

}

void WindowClass::renewView(int _wid, int _hig)
{
	app.setView(transformView(canvas.canvasToView(), _wid, _hig));

}

