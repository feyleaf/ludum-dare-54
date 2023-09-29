#ifndef CANVAS_H
#define CANVAS_H

#include "global.h"
namespace ic
{
	struct gameScreen {
		sf::VideoMode mode = sf::VideoMode(800, 600, 32);
		sf::String nickname = "Undefined";
	};

	const static gameScreen vm_cga = { sf::VideoMode(320, 200, 8), "CGA" };
	const static gameScreen vm_qvga = { sf::VideoMode(320, 240, 32), "QVGA" };
	const static gameScreen vm_vga = { sf::VideoMode(640, 480, 32), "VGA" };
	const static gameScreen vm_pal = { sf::VideoMode(768, 576, 32), "PAL" };
	const static gameScreen vm_svga = { sf::VideoMode(800, 600, 32), "SVGA" };
	const static gameScreen vm_xga = { sf::VideoMode(1024, 768, 32), "XGA" };
	const static gameScreen vm_gameboy = { sf::VideoMode(160, 144, 8), "Gameboy" };
	const static gameScreen vm_atari2600 = { sf::VideoMode(160, 192, 8), "Atari 2600" };
	const static gameScreen vm_nes = { sf::VideoMode(256, 240, 8), "NES" };
	const static gameScreen vm_snes = { sf::VideoMode(256, 224, 16), "SNES" };
	const static gameScreen vm_sms = { sf::VideoMode(256, 192, 8), "Sega Master System" };
	const static gameScreen vm_ngage = { sf::VideoMode(176, 208, 16), "N-Gage" };
	const static gameScreen vm_nokia5110 = { sf::VideoMode(84, 84, 8), "Nokia 5110" };
	const static gameScreen vm_razrv3 = { sf::VideoMode(176, 220, 16), "Razr v3" };
	const static gameScreen vm_blackberry8100 = { sf::VideoMode(420, 260, 8), "BlackBerry 8100" };
	const static gameScreen vm_iphone1 = { sf::VideoMode(320, 480, 32), "iPhone 1" };
	const static gameScreen vm_genesis = { sf::VideoMode(320, 224, 16), "Sega Genesis" };
	const static gameScreen vm_wonderswan = { sf::VideoMode(224, 144, 16), "WonderSwan" };
	const static gameScreen vm_gba = { sf::VideoMode(240, 160, 8), "Gameboy Advance" };
	const static gameScreen vm_pokemon = { sf::VideoMode(96, 64, 8), "PokeMini" };
	const static gameScreen vm_ps2 = { sf::VideoMode(512, 448, 32), "PS2" };
	const static gameScreen vm_ds = { sf::VideoMode(256, 192, 32), "Nintendo DS" };
	const static gameScreen vm_psp = { sf::VideoMode(480, 272, 32), "PSP" };


	class Canvas
	{
	public:
		Canvas(gameScreen _screen) : canvasScreen(_screen) {}
		gameScreen canvasScreen = vm_sms;
		
		int getCanvasWidth();
		int getCanvasHeight();
		sf::FloatRect canvasToRect();
		sf::String getCanvasNickname();
		sf::VideoMode getDefaultVideoMode();
		sf::VideoMode getAppropriateVideoMode();
		sf::View canvasToView();
	};
}

#endif//CANVAS_H