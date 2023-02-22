#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <bn_core.h>
#include <bn_keypad.h>
#include <bn_string.h>
#include <bn_sprite_palettes.h>
#include <bn_bg_palettes.h>
#include <bn_sprite_item.h>

#include "../include/common_info.h"
#include "../include/common_stats.h"
#include "../include/common_variable_8x16_sprite_font.h"
    
enum directions {
    RESET = -1,
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
    BLANK = 4
};

void wait_for_x(int x);
void display_for_x(bn::istring displayText, int x);
bool check_start_screen(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter);
bool wait_for_confirm(bn::istring);
void fade_all_bg_and_sprite();
void unfade_all_bg_and_sprite();
#endif
