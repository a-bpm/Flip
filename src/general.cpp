#include "general.hpp"

#include <bn_sprite_tiles_ptr.h>
#include <bn_sprite_text_generator.h>
#include "../build/bn_sprite_items_shapecons.h"
#include "../build/bn_sound_items.h"

void wait_for_x(int x)
{
    for (int i = 0; i < x; i++)
        bn::core::update();
}

bool check_start_screen(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter)
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

    if (bn::keypad::a_pressed())
    {
        if (spriteState == UP || spriteState == DOWN) 
        {
            bn::sound_items::blip.play(0.5);

        } else {

            bn::sound_items::blop.play(0.5);
        }

        spriteState++;
        if (spriteState > LEFT)
        {
            spriteState = UP;
        }
        sprite.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(spriteState));
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
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 5; i++)
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
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 4; i++)
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

bool wait_for_confirm(bn::istring displayText)
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bool waiting = true;
    bn::vector<bn::sprite_ptr, 32> text_sprites;
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();
    text_generator.generate(0, 20, displayText, text_sprites);

    while (!bn::keypad::a_pressed())
    {
        waiting = false;
        bn::core::update();
    }
    return waiting;
}

void display_for_x(bn::istring displayText, int x)
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 32> text_sprites;
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();
    text_generator.generate(0, 20, displayText, text_sprites);

    for (int i = 0; i < x; i++)
    {
        bn::core::update();
    }
}
