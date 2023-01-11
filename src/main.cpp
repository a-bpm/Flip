/*
   Written by me, Andrew, as my first step into game development and as a 
   C++ programming exercise
*/

#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_optional.h>
#include <bn_string.h>

#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_palettes.h>
#include <bn_sprite_builder.h>
#include "../build/bn_sprite_items_shapecons.h"
#include "../build/bn_sprite_items_dice.h"
#include "../build/bn_sprite_items_pointer.h"
#include "../build/bn_sprite_items_circle_card.h"
#include "../build/bn_sprite_items_diamond_card.h"
#include "../build/bn_sprite_items_triangle_card.h"
#include "../build/bn_sprite_items_square_card.h"
#include "../build/bn_sprite_items_pointer.h"

#include <bn_sprite_text_generator.h>

#include <bn_regular_bg_item.h>
#include <bn_regular_bg_ptr.h>
#include "../build/bn_regular_bg_items_title.h"
#include "../build/bn_regular_bg_items_press_start.h"

#include <bn_bg_palettes.h>

#include "../build/bn_sound_items.h"
#include <bn_fixed.h>
#include <bn_color.h>
#include <bn_random.h>

#include "../include/common_info.h"
#include "../include/common_stats.h"
#include "../include/common_variable_8x16_sprite_font.h"

// sprite orientation
enum directions {
    RESET = -1,
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
    BLANK = 4
};

// class defs
class Game {
    public:
        bn::optional<bn::sprite_ptr> gameSprite;
        int spriteState;
        int stateMax;
        unsigned int salt = 0;

        Game(int initState, int x, int y, unsigned int seed, bn::sprite_item sprite)
        {
            salt = seed;
            spriteState = initState;
            gameSprite = sprite.create_sprite(x, y);
        }

        Game(unsigned int seed)
        {
            salt = seed;
        }

        unsigned int getSalt()
        {
            return this->salt;
        }

        void addSalt()
        {
            bn::random shaker;
            for (unsigned int i = 0; i < salt<<3; i++)
            {
                shaker.update();
            }

            for (int i = 0; i < 60; i++)
            {
                if (bn::keypad::any_pressed() || bn::keypad::any_held())
                {
                    this->salt += (shaker.get_int(10));
                }
            }
        }

        void setSalt(unsigned int input)
        {
            this->salt = input;
        }

};

class CardGame: public Game {
    public:
        using Game::Game;
        bn::vector<bn::sprite_ptr, 4> cards;

        CardGame(int targetShape)
        : Game(targetShape)
        {
            spriteState = targetShape;
        }

        void add_cards(bn::sprite_ptr addMe)
        {
            cards.push_back(addMe);
        }

        void empty_cards()
        {
            cards.clear();
        }
        /* matches outer cards x pos to inner cards
         * then brings all cards x pos to center
         */
        void stack_cards()
        {
            while (cards.at(0).x() < cards.at(1).x())
            {
                cards.at(0).set_x(cards.at(0).x() + 1);
                cards.at(3).set_x(cards.at(3).x() - 1);
                bn::core::update();
            }
            while (cards.at(0).x() != 0)
            {
                for (bn::sprite_ptr s : cards)
                {
                    if (s.x() < 0)
                    {
                        s.set_x(s.x() + 2);
                    }
                    else
                    {
                        s.set_x(s.x() - 2);
                    }
                    bn::core::update();
                }
            }
        }

        /* 0 = showing
         * 1 = hidden
         */
        void flip_cards(int flipTo)
        {
            cards.at(0).set_tiles(bn::sprite_items::circle_card.tiles_item().create_tiles(flipTo));
            cards.at(1).set_tiles(bn::sprite_items::square_card.tiles_item().create_tiles(flipTo));
            cards.at(2).set_tiles(bn::sprite_items::triangle_card.tiles_item().create_tiles(flipTo));
            cards.at(3).set_tiles(bn::sprite_items::diamond_card.tiles_item().create_tiles(flipTo));
        }

        void shuffle_cards()
        {
             
        } 

        void spread_cards(int outerPos, int innerPos)
        {
            cards.at(0).set_x(-2);
            cards.at(1).set_x(-2);
            cards.at(2).set_x(2);
            cards.at(3).set_x(2);

            while (cards.at(0).x() > innerPos)
            {
                for (bn::sprite_ptr s : cards)
                {
                    if (s.x() < 0)
                    {
                        s.set_x(s.x() - 2);
                    }
                    else
                    {
                        s.set_x(s.x() + 2);
                    }
                    bn::core::update();
                }
            }

            while (cards.at(0).x() > outerPos)
            {
                cards.at(0).set_x(cards.at(0).x() - 1);
                cards.at(3).set_x(cards.at(3).x() + 1);
                bn::core::update();
            }

        } 
};

class DiceGame: public Game {

    public:
        using Game::Game;
        DiceGame(int initState, int x, int y, unsigned int seed, bn::sprite_item sprite)
        : Game(initState, x, y, seed, sprite)
        {
            salt = seed;
            spriteState = initState;
            gameSprite = sprite.create_sprite(x, y);
        }

        int shuffle_dice()
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
                gameSprite->set_tiles(bn::sprite_items::dice.tiles_item().create_tiles(selectedFace));

                for (int j = 0; j < 7; j++)
                {
                    bn::core::update();
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
void wait_for_x(int x);
bool check_start_screen(bn::sprite_ptr& sprite, int& spriteState, int& frameCounter);
bool wait_for_confirm(bn::istring);
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
    bool startScreen = false; // set to false to skip start screen
    int frameCounter = 0;
    unsigned int seed = 0;

    // start screen start
    if (startScreen)
    {
        bn::sprite_ptr shape = bn::sprite_items::shapecons.create_sprite(0, -30);
        bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(-12, -24);
        bn::regular_bg_ptr press_start = bn::regular_bg_items::press_start.create_bg(0, 16);
        do
        {
            startScreen = check_start_screen(shape, spriteState, frameCounter);
            seed += 1;
            if (frameCounter == 40)
            {
                seed += 2;
                press_start.set_visible(! press_start.visible());
            }
            else if (frameCounter == 50)
            {
                seed += 3;
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


        fade_all_bg_and_sprite();
    }

    fade_all_bg_and_sprite();
    // dice game start
    bool diceGameRunning = false; // set to false to skip game
    bool drawFlag = false;
    if (diceGameRunning)
    {
        bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(-12, -24);
        unfade_all_bg_and_sprite();

        bool playingDice = true;
        while(playingDice) //
        {
            bn::string<50> displayText = "Press A to start the dice roll!";
            if (!drawFlag)
            {
                wait_for_confirm(displayText);
            }

            DiceGame playerDiceControl(0, -60, 0, seed, bn::sprite_items::dice);
            DiceGame enemyDiceControl(0, 60, 0, seed<<2, bn::sprite_items::dice);


            bn::sprite_ptr pointerSprite = bn::sprite_items::pointer.create_sprite(-85, 0);
            pointerSprite.set_tiles(bn::sprite_items::pointer.tiles_item().create_tiles(RIGHT));

            for (int i = 0; i < 30; i++)
            {
                bn::core::update();
                if (bn::keypad::any_pressed() || bn::keypad::any_held())
                {
                    playerDiceControl.addSalt();
                    enemyDiceControl.addSalt();
                }
            }
            int playerRoll = playerDiceControl.shuffle_dice();

            for (int i = 0; i < 30; i++)
            {
                bn::core::update();
                if (bn::keypad::any_pressed() || bn::keypad::any_held())
                {
                    playerDiceControl.addSalt();
                    enemyDiceControl.addSalt();
                }
            }
            int enemyRoll = enemyDiceControl.shuffle_dice();


            if (playerRoll < enemyRoll) // lose
            {
                // Lost! Retry? [Yes, Yes!!]
                displayText = "You lose! Press A to retry!";
                wait_for_confirm(displayText);
            }
            else if (playerRoll > enemyRoll) // win
            {
                // Win! Move on?
                displayText = "You win! Press A to move on!";
                wait_for_confirm(displayText);
                playingDice = false;
            }
            else if (playerRoll == enemyRoll) // draw
            {
                // Draw! display, confirm reroll
                displayText = "Draw! Press A to retry!";
                wait_for_confirm(displayText);
                drawFlag = true;
            }
        }
    }

    //card game start
    fade_all_bg_and_sprite();
    bool cardGameRunning = true;
    bool retry = true;
    if (cardGameRunning)
    {
        // 0 is the target shape based off shape.bmp
        CardGame cardSelector(0);
        int circPos, sqPos, triPos, diaPos;
        circPos = -78;
        sqPos = -26;
        triPos = -sqPos;
        diaPos = -circPos;

        unfade_all_bg_and_sprite();

        bool playingCards = true;
        while (playingCards) 
        {
            bn::string<50> displayText = "Press A to start the card game!";
            if(retry)
            {
                wait_for_confirm(displayText);
            }
            cardSelector.empty_cards(); 
            bn::sprite_ptr circlePtr = bn::sprite_items::circle_card.create_sprite(circPos, -20);
            bn::sprite_ptr squarePtr = bn::sprite_items::square_card.create_sprite(sqPos, -20);
            bn::sprite_ptr trianglePtr = bn::sprite_items::triangle_card.create_sprite(triPos, -20);
            bn::sprite_ptr diamondPtr = bn::sprite_items::diamond_card.create_sprite(diaPos, -20);
            cardSelector.add_cards(circlePtr); 
            cardSelector.add_cards(squarePtr); 
            cardSelector.add_cards(trianglePtr); 
            cardSelector.add_cards(diamondPtr); 

            cardSelector.stack_cards();
            cardSelector.flip_cards(1);
            cardSelector.shuffle_cards();
            cardSelector.spread_cards(circPos, sqPos);

            bn::sprite_ptr pointerPtr = bn::sprite_items::pointer.create_sprite(circPos, 21);
            wait_for_x(120);

            // TODO: Define goal shape, shuffle card function, pointer position movement
            // cardSelector.flip_cards(0);

        }

    }
}

// Function definitions

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

        spriteState += 1;
        if (spriteState > 3)
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

    //BN_LOG("X val: ", sprite.x());
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

void reveal_all_bg_and_sprite()
{
    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
    bn::core::update();
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
