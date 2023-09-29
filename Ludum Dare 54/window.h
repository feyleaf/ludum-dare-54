#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "global.h"




class WindowClass
{
public:
	ic::Canvas canvas = ic::vm_sms;

	sf::RenderWindow app;
	
	WindowClass();
	WindowClass(ic::gameScreen _screen);
	~WindowClass();
	sf::View transformView(sf::View originalView, int winWidth, int winHeight);
	sf::Vector2f transformMouse(sf::RenderWindow& _win);
	float acceptableWindowScaleMultiple = 1.0f;
	sf::Vector2u desktopDimension = { sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height };

	sf::View gameView;
	int videoWidth = 0;
	int videoHeight = 0;
	bool isFullScreen = false;

	void launchWindow(const sf::String _title = "SFML Game");
	void launchFullScreen(const sf::String _title = "SFML Game (Fullscreen)");
	void renewView(int _wid, int _hig);

};

#endif //WINDOW_HPP