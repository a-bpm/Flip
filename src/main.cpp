/*
Written by me, Andrew, as my first step into game development and as a 
C++ programming exercise
*/

#include "bn_core.h"
#include <bn_keypad.h>
#include <bn_optional.h>

#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_palettes.h>
#include <bn_sprite_builder.h>
#include "bn_sprite_items_shapecons.h"
#include "bn_sprite_items_dice.h"
#include "bn_sprite_items_pointer.h"
#include "bn_sprite_text_generator.h"

#include <bn_regular_bg_item.h>
#include <bn_regular_bg_ptr.h>
#include "bn_regular_bg_items_title.h"
#include "bn_regular_bg_items_press_start.h"

#include <bn_bg_palettes.h>

#include "bn_sound_items.h"
#include <bn_fixed.h>
#include <bn_color.h>
#include <bn_random.h>

#include "common_variable_8x16_sprite_font.h"

// sprite orientation
enum directions {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

// class defs
class DiceSprite {
    bn::optional<bn::sprite_ptr> dice_sprite;
    int spriteState;
    unsigned int salt = 0;
    public:
        DiceSprite(int initState, int x, int y)
        {
            dice_sprite = bn::sprite_items::dice.create_sprite(x, y);
            salt = 0;
            spriteState = initState;
        }

        unsigned int getSalt()
        {
            return this->salt;
        }

        void addSalt(unsigned int input)
        {
            this->salt += input;
        }

        void setSalt(unsigned int input)
        {
            this->salt = input;
        }

        int shuffleDice()
        {
            int finalNum = 0;
            bn::random faceSelector;
            for (unsigned int i = 0; i < salt; i++)
            {
                faceSelector.update();
            }

            for (int i = 0; i < 25; i++)
            {
                int selectedFace = faceSelector.get_int(6);
                dice_sprite->set_tiles(bn::sprite_items::dice.tiles_item().create_tiles(selectedFace));
                
                for (int j = 0; j < 7; j++)
                {
                    bn::core::update();
                    if (bn::keypad::any_pressed())
                    {
                        salt++;
                    }
                }
                
                if (i == 24)
                {
                    finalNum = selectedFace;
                }
            }
            return finalNum;
        }
};

// Function declarations
bool check_start_screen(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter);
bool still_waiting();
void fade_all_bg_and_sprite();
void unfade_all_bg_and_sprite();
void fade_all_sprite();
void unfade_all_sprite();
void fade_all_bg();
void unfade_all_bg();
void reveal_all_bg_and_sprite();

int main()
{
    bn::core::init();
    bn::sound_items::start_up.play(0.5);

    int spriteState = 1;
    bool startScreen = true;
    int frameCounter = 0;

    // start screen start
    if (startScreen)
    {
        bn::sprite_ptr shape = bn::sprite_items::shapecons.create_sprite(0, -30);
        bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(-12, -24);
        bn::regular_bg_ptr press_start = bn::regular_bg_items::press_start.create_bg(0, 16);
        do
        {
            startScreen = check_start_screen(shape, spriteState, frameCounter);
            if (frameCounter == 40)
            {
                press_start.set_visible(! press_start.visible());
            }
            else if (frameCounter == 50)
            {
                press_start.set_visible(press_start.visible());
            }

            if (!startScreen)
            {
                fade_all_bg_and_sprite();
                shape.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(0));
            }
        }
        while (startScreen);
    }

    unfade_all_bg_and_sprite();

    bool coinGameRunning = true;
    if (coinGameRunning)
    {

    }
    // dice game start
    bool diceGameRunning = true;

    if (diceGameRunning)
    {
        bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(-12, -24);
        bool waitingForRoll = true;
        while(waitingForRoll)
        {
            // Press a to start
            waitingForRoll = still_waiting();
        }

        bool playingDice = true;
        while(playingDice) //
        {
            DiceSprite playerDiceControl = DiceSprite(0, -60, 0);
            DiceSprite enemyDiceControl = DiceSprite(0, 60, 0);

            if (bn::keypad::any_pressed())
            {
                playerDiceControl.addSalt(3);
                enemyDiceControl.addSalt(5);
            }

            bn::sprite_ptr pointerSprite = bn::sprite_items::pointer.create_sprite(-85, 0);
            pointerSprite.set_tiles(bn::sprite_items::pointer.tiles_item().create_tiles(RIGHT));
            int playerRoll = playerDiceControl.shuffleDice();
            int enemyRoll = enemyDiceControl.shuffleDice();

            for (int i = 0; i < 30; i++)
            {
                bn::core::update();
            }

            if (playerRoll < enemyRoll) // lose
            {
                // Lost! Retry? [Yes, Yes!!]

                playingDice = false;
            }
            else if (playerRoll > enemyRoll) // win
            {
                // Win! Move on?
                playingDice = false;
            }
            else if (playerRoll == enemyRoll) // draw
            {
                // Draw! display, confirm reroll
            }
        }
    }

    for (int i = 0; i < 60; i++)
    {
        bn::core::update();
    }
}

// Function definitions

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
        for (int i = 0; i < 5; i++)
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

void reveal_all_bg_and_sprite()
{
    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
    bn::core::update();
}

bool still_waiting()
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bool waiting = true;
    if (bn::keypad::a_pressed())
    {
        waiting = false;
    }
    bn::core::update();
    return waiting;
}