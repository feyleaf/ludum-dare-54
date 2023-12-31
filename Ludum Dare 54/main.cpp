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
    game.applicationWindow.launchWindow("Clutterphobia");

    while (game.applicationWindow.app.isOpen())
        game.gameLoop();

    return 0;
}