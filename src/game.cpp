#include "game.hpp"

#include "../build/bn_sprite_items_shapecons.h"
#include "../build/bn_sprite_items_dice.h"
#include "../build/bn_sprite_items_pointer.h"
#include "../build/bn_sprite_items_circle_card.h"
#include "../build/bn_sprite_items_diamond_card.h"
#include "../build/bn_sprite_items_triangle_card.h"
#include "../build/bn_sprite_items_square_card.h"
#include "../build/bn_sprite_items_pointer.h"
#include "../build/bn_sprite_items_coins.h"

Game::Game(int initState, int x, int y, unsigned int seed, bn::sprite_item sprite)
{
    salt = seed;
    spriteState = initState;
    gameSprite = sprite.create_sprite(x, y);
}

Game::Game(unsigned int seed)
{
    salt = seed;
}

void Game::add_salt()
{
    bn::random shaker;
    for (unsigned int i = 0; i < this->salt; i++)
    {
        shaker.update();
    }


    this->salt += (shaker.get_int(5));

    for (int i = 0; i < 60; i++)
    {
        if (bn::keypad::any_pressed() || bn::keypad::any_held())
        {
            this->salt += (shaker.get_int(10));
        }
    }
}

CoinGame::CoinGame(int initState, int maxState, int x, int y, bn::sprite_item sprite, int seed)
        : Game(seed)
{
    gameSprite = sprite.create_sprite(x, y);
    spriteState = initState;
    stateMax = maxState;
}

int CoinGame::select_coin()
{
    bn::sprite_ptr pointerPtr = bn::sprite_items::pointer.create_sprite(-73, 72);
    int cursorSelection = 0;

    while(!bn::keypad::a_pressed())
    {
        if(bn::keypad::right_pressed() && cursorSelection < 1) // move right if not at rightmost
        {
            cursorSelection++;
            pointerPtr.set_x(pointerPtr.x() + COIN_SPACING);
            add_salt();
        }

        if(bn::keypad::left_pressed() && cursorSelection > 0) // move left if not at leftmost
        {
            cursorSelection--;
            pointerPtr.set_x(pointerPtr.x() - COIN_SPACING);
            add_salt();
        }
        add_salt();
        bn::core::update();
    }

    return cursorSelection;
}

int CoinGame::flip_coin()
{
    bn::sprite_animate_action<16> flipAction = bn::create_sprite_animate_action_forever(
            gameSprite.value(), 0, bn::sprite_items::coins.tiles_item()
            , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

    while (gameSprite.value().y() > -30)
    {
        flipAction.update();
        gameSprite.value().set_y(gameSprite.value().y()-2);
        bn::core::update();
    }

    int timeCount = 0;
    while (timeCount < 35)
    {
        timeCount++;
        flipAction.update();
        bn::core::update();
    }

    while (gameSprite.value().y() < 50)
    {
        flipAction.update();
        gameSprite.value().set_y(gameSprite.value().y()+3);
        bn::core::update();
    }

    int result = determine_toss();
    if (result) //triangle chosen
    {
        gameSprite.value().set_tiles(bn::sprite_items::coins.tiles_item().create_tiles(11));
    }
    else 
    {
        gameSprite.value().set_tiles(bn::sprite_items::coins.tiles_item().create_tiles(5));
    }

    return result;
}

int CoinGame::determine_toss()
{
    int result = 0;
    bn::random coinFlip;
    for (unsigned int i = 0; i < this->salt; i++)
    {
        coinFlip.update();
    }

    if (coinFlip.get_int(100) < coinFlip.get_int(100))
    {
        result = 1;
    }

    return result;
}

CardGame::CardGame(int target, bn::sprite_item sprite, int seed)
    : Game(seed)
{
    gameSprite = sprite.create_sprite(0, 60);
    gameSprite.value().set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(target));
}

void CardGame::add_cards(bn::sprite_ptr addMe)
        {
            cards.push_back(addMe);
        }

void CardGame::empty_cards()
        {
            cards.clear();
        }
        /* matches outer cards x pos to inner cards
         * then brings all cards x pos to center
         */
        void CardGame::stack_cards()
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
        void CardGame::flip_cards(int flipTo, char order[])
        {
            for (int i = 0; i < 4; i++)
            {
                switch(order[i])
                {
                    case 'c':
                        cards.at(i).set_tiles(bn::sprite_items::circle_card.tiles_item().create_tiles(flipTo));
                        break;

                    case 's':
                        cards.at(i).set_tiles(bn::sprite_items::square_card.tiles_item().create_tiles(flipTo));
                        break;

                    case 't':
                        cards.at(i).set_tiles(bn::sprite_items::triangle_card.tiles_item().create_tiles(flipTo));
                        break;

                    case 'd':
                        cards.at(i).set_tiles(bn::sprite_items::diamond_card.tiles_item().create_tiles(flipTo));
                        break;
                    default:
                        break;
                }
            }
        }

        /*
         * swaps cards in array and tracks order
         */
        void CardGame::shuffle_cards(int& targetShape, char order[])
        {
            bn::random cardDecider;
            for (unsigned int i = 0; i < this->salt; i++)
            {
                cardDecider.update();
            }

            for (unsigned int i = 0; i < this->salt; i++)
            {
            int swaps = cardDecider.get_int(4);
            int swapTo = cardDecider.get_int(4);
            if (swaps == swapTo)
            {
                swapTo = 3 - cardDecider.get_int(4);
            }
                switch(swaps)
                {
                    case 0:
                        if (targetShape == swaps)
                        {
                            targetShape = swapTo;
                        }
                        else if (targetShape == swapTo)
                        {
                            targetShape = swaps;
                        }

                        bn::swap(cards[swaps], cards[swapTo]);
                        std::swap(order[swaps], order[swapTo]);

                        break;

                    case 1:
                        if (targetShape == swaps)
                        {
                            targetShape = swapTo;
                        }
                        else if (targetShape == swapTo)
                        {
                            targetShape = swaps;
                        }

                        bn::swap(cards[swaps], cards[swapTo]);
                        std::swap(order[swaps], order[swapTo]);

                        break;

                    case 2:
                        if (targetShape == swaps)
                        {
                            targetShape = swapTo;
                        }
                        else if (targetShape == swapTo)
                        {
                            targetShape = swaps;
                        }

                        bn::swap(cards[swaps], cards[swapTo]);
                        std::swap(order[swaps], order[swapTo]);

                        break;

                    case 3:
                        if (targetShape == swaps)
                        {
                            targetShape = swapTo;
                        }
                        else if (targetShape == swapTo)
                        {
                            targetShape = swaps;
                        }

                        bn::swap(cards[swaps], cards[swapTo]);
                        std::swap(order[swaps], order[swapTo]);

                        break;
                    default:
                        break;
                }
            }
            bn::core::update();
        } 

        bool CardGame::correct_card(int startingPos, int target)
        {
            bool winner = false;
            bn::sprite_ptr pointerPtr = bn::sprite_items::pointer.create_sprite(startingPos, 21);
            int cursorSelection = 0;
            while (!bn::keypad::a_pressed())
            {
                if(bn::keypad::right_pressed() && cursorSelection < 3) // move right if not at rightmost
                {
                    cursorSelection++;
                    pointerPtr.set_x(pointerPtr.x() + CARD_SPACING);
                }

                if(bn::keypad::left_pressed() && cursorSelection > 0) // move left if not at leftmost
                {
                    cursorSelection--;
                    pointerPtr.set_x(pointerPtr.x() - CARD_SPACING);
                }
                bn::core::update();
            }


            if (cursorSelection == target)
            {
                winner = true;
            }

            return winner;
        }

        void CardGame::spread_cards(int outerPos, int innerPos)
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

DiceGame::DiceGame(int initState, int x, int y, unsigned int seed, bn::sprite_item sprite)
            : Game(initState, x, y, seed, sprite)
        {
            salt = seed;
            spriteState = initState;
            gameSprite = sprite.create_sprite(x, y);
        }

        int DiceGame::shuffle_dice()
        {
            int finalNum = 0;
            bn::random faceSelector;
            for (unsigned int i = 0; i < this->salt; i++)
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
