#include "bn_core.h"
#include <bn_keypad.h>

#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_palettes.h>
#include <bn_sprite_builder.h>
#include "bn_sprite_items_shapecons.h"

#include <bn_regular_bg_item.h>
#include <bn_regular_bg_ptr.h>
#include "bn_regular_bg_items_title.h"
#include "bn_regular_bg_items_press_start.h"

#include <bn_bg_palettes.h>

#include "bn_sound_items.h"
#include <bn_fixed.h>
#include <bn_color.h>


// Function declarations
bool checkStartScreenState(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter);
void fade_all_bg_and_sprite();
void unfade_all_bg_and_sprite();
void fade_all_sprite();
void unfade_all_sprite();
void fade_all_bg();
void unfade_all_bg();

int main()
{
    bn::core::init();

    bn::sprite_ptr shape = bn::sprite_items::shapecons.create_sprite(0, -30);
    bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(-12, -24);
    bn::regular_bg_ptr press_start = bn::regular_bg_items::press_start.create_bg(0, 16);

    while(true)
    {
        bn::sound_items::start_up.play(0.5);
        int spriteState = 1;

        bool startScreen = true;
        int frameCounter = 0;
        while(startScreen)
        {
            startScreen = checkStartScreenState(shape, spriteState, frameCounter);
            if(frameCounter == 20)
            {
                press_start.set_visible(! press_start.visible());
            }
            else if (frameCounter == 40)
            {
                press_start.set_visible(press_start.visible());
            }

            if(!startScreen)
            {
                fade_all_bg_and_sprite();
                shape.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(0));
                unfade_all_bg_and_sprite();
            }
        }
    }
}

// Function definitions

bool checkStartScreenState(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter)
{
    bool startScreen = true;

    if (frameCounter > 60)
    {
        frameCounter = 0;
    }

    if (bn::keypad::start_pressed())
    {
        startScreen = false;
    }

    if (bn::keypad::a_pressed() && spriteState == 0)
    {
        sprite.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(0));
        spriteState = 1;
        bn::sound_items::blip.play(0.5);
    }

    if (bn::keypad::b_pressed() && spriteState == 1)
    {
        sprite.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(1));
        spriteState = 0;
        bn::sound_items::blop.play(0.5);
    }

    if (bn::keypad::l_pressed())
    {
        bn::sound_items::start_up.play(0.5);
    }

    if (bn::keypad::r_pressed())
    {
        bn::sound_items::test.play(0.5);
    }
    if (bn::keypad::left_held())
    {
        sprite.set_x(sprite.x() - 1);
    }
    else if (bn::keypad::right_held())
    {
        sprite.set_x(sprite.x() + 1);
    }
    
    if (bn::keypad::up_held())
    {
        sprite.set_y(sprite.y() - 1);
    }
    else if (bn::keypad::down_held())
    {
        sprite.set_y(sprite.y() + 1);
    }

    bn::core::update();
    frameCounter++;
    return startScreen;
}

void fade_all_bg_and_sprite()
{
    bn::fixed fader = 0.1;
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 5; i++)
        {
            bn::core::update();
        }
        if (fader < 1)
        {
            fader += 0.1;
        }
        if (fader > 1)
        {
            fader = 1;
        }
        bn::bg_palettes::set_fade_intensity(fader);
        bn::sprite_palettes::set_fade_intensity(fader);
        bn::core::update();
    }
}

void unfade_all_bg_and_sprite()
{
    bn::fixed fader = 0.9;
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 5; i++)
        {
            bn::core::update();
        }
        if (fader > 0)
        {
            fader -= 0.1;
        }
        if (fader < 0)
        {
            fader = 0;
        }
        bn::bg_palettes::set_fade_intensity(fader);
        bn::sprite_palettes::set_fade_intensity(fader);
        bn::core::update();
    }
}