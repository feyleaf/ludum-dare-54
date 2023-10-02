#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// all the headers are placed here to be included globally in each header
//////////////////////
/*
- Project Folder
  - Iclea Algorithms
    - Math
      - ic_math
    - Pathfinding
      - ic_pathfinding
    - Random Noise
      - ic_noise
  - Objects
    - Entity
      - entity
      - entity_player
      - entity_npc
      - entity_world
      - box_inventory
    - Tiles
      - tile
    - Tile Entities
      - entity_tile
    - HUD Elements
      - dialogue
      - overlay
    - Items
      - item_ether
      - item_real
  - Render
    - Image Import
      - image_manager
    - Sprite Manager
      - sprite_manager
    - Screen Layers
      - screen_zorder
      - screen_builder
    - Screen Logic
      - screen_logic
    - Window Ratios
      - ratio_handler
  - Application
    - Input Handler
      - mouse
      - keyboard
      - focus
      - delta_timer
    - Update Logic
      - delta_update
      - world_layout
    - Trigger Logic
      - triggers
    - Response Logic
      - response
    - Window App
      - application
      - video_module
    - Game State
      - state
    - Game Loop
      - game
      - delta_loop
*/
/////////////////////

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "chipmunk/chipmunk.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <map>
#include <vector>
#include <deque>
#include <list>
#include "ic_math.h"
//#include "ic_pathing.h"
#include "rigidbody.h"
#include "canvas.h"
#include "window.h"
#include "game.h"

#endif//GLOBAL_HPP