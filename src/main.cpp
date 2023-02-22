/*
   Written by me, Andrew, as my first step into game development and as a 
   C++ programming exercise
   */

#include "game.hpp"
#include "general.hpp"

#include "../build/bn_sprite_items_shapecons.h"
#include "../build/bn_sprite_items_dice.h"
#include "../build/bn_sprite_items_circle_card.h"
#include "../build/bn_sprite_items_diamond_card.h"
#include "../build/bn_sprite_items_triangle_card.h"
#include "../build/bn_sprite_items_square_card.h"
#include "../build/bn_sprite_items_pointer.h"
#include "../build/bn_sprite_items_coins.h"
#include "../build/bn_regular_bg_items_title.h"
#include "../build/bn_regular_bg_items_press_start.h"
#include "../build/bn_sound_items.h"
// sprite orientation

// Function declarations

int main()
{
    bn::core::init();
    bn::sound_items::start_up.play(0.5);

    int targetShape = 0;
    bool startScreen = true; // set to false to skip start screen
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
            startScreen = check_start_screen(shape, targetShape, frameCounter);
            seed++;
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

    bool coinGameRunning = true; // set to false to skip coin game
    if (coinGameRunning)
    {
        bn::string<50> displayText = "Choose a coin!";

        CoinGame coiner(0, 15, 0, 50, bn::sprite_items::coins, seed);
        bn::sprite_ptr triCoin = bn::sprite_items::coins.create_sprite(75, 35);
        triCoin.set_tiles(bn::sprite_items::coins.tiles_item().create_tiles(0));
        bn::sprite_ptr diaCoin = bn::sprite_items::coins.create_sprite(-75, 35);
        triCoin.set_tiles(bn::sprite_items::coins.tiles_item().create_tiles(15));
        unfade_all_bg_and_sprite();

        bool playingCoin = true;
        while (playingCoin)
        {
            display_for_x(displayText, 90);
            coiner.add_salt();
            int choice = coiner.select_coin();
            coiner.add_salt();
            int result = coiner.flip_coin();
            wait_for_x(120);

            if (result == choice) // Win
            {
                displayText = "Onto the next round!";
                playingCoin = false;
                display_for_x(displayText, 90);
            }
            else
            {
                coiner.add_salt();
            }
        }
    }

    fade_all_bg_and_sprite();
    // dice game start
    bool diceGameRunning = true; // set to false to skip dice game
    bool drawFlag = false;
    if (diceGameRunning)
    {
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
                    playerDiceControl.add_salt();
                    enemyDiceControl.add_salt();
                }
            }
            int playerRoll = playerDiceControl.shuffle_dice();

            for (int i = 0; i < 30; i++)
            {
                bn::core::update();
                if (bn::keypad::any_pressed() || bn::keypad::any_held())
                {
                    playerDiceControl.add_salt();
                    enemyDiceControl.add_salt();
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
    bool cardGameRunning = true; // set to false to skip card game
    if (cardGameRunning)
    {
        int circPos, sqPos, triPos, diaPos;
        circPos = -78;
        sqPos = -26;
        triPos = -sqPos;
        diaPos = -circPos;

        unfade_all_bg_and_sprite();

        bn::string<50> displayText = "Find the symbol shown!";

        char cardOrder[] = { 'c', 's', 't', 'd' };
        CardGame cardSelector(targetShape, bn::sprite_items::shapecons, seed);
        bool playingCards = true;
        while (playingCards) 
        {
            display_for_x(displayText, 120);
            cardSelector.empty_cards(); 

            bn::sprite_ptr circlePtr = bn::sprite_items::circle_card.create_sprite(circPos, -20);
            cardSelector.add_cards(circlePtr); 
            bn::sprite_ptr squarePtr = bn::sprite_items::square_card.create_sprite(sqPos, -20);
            cardSelector.add_cards(squarePtr); 
            bn::sprite_ptr trianglePtr = bn::sprite_items::triangle_card.create_sprite(triPos, -20);
            cardSelector.add_cards(trianglePtr); 
            bn::sprite_ptr diamondPtr = bn::sprite_items::diamond_card.create_sprite(diaPos, -20);
            cardSelector.add_cards(diamondPtr); 

            wait_for_x(60);
            cardSelector.stack_cards();
            cardSelector.flip_cards(1, cardOrder);

            wait_for_x(60);
            cardSelector.shuffle_cards(targetShape, cardOrder);
            cardSelector.spread_cards(circPos, sqPos);

            if(!cardSelector.correct_card(circPos, targetShape))
            {
                displayText = "Press A to retry!";
                cardSelector.add_salt();
            }
            else
            {
                playingCards = false;
            }

            cardSelector.flip_cards(0, cardOrder);
            wait_for_x(120);
        }

    }

    bn::string<50> displayText = "You win!";
    while(true)
    {
        wait_for_confirm(displayText);
    }
}
