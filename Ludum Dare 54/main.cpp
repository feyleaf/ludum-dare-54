//Game Development Practice for Game Jam Warmup
//Benjamin C. Watt (feyleaf.com)
//September 11,2023
// //////////////////////
// Using SFML 2.6.0 official release, and C++
////////////////////////////////////
#include "global.h"

int WinMain()
{
    GameClass game(ic::vm_sms);
    game.applicationWindow.launchWindow("I've Reached My Limit with Clutter! (feyleaf.com)");

    while (game.applicationWindow.app.isOpen())
        game.gameLoop();

    return 0;
}