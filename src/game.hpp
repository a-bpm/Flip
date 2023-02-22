/* Header file for Game Classes
 *
 */
#ifndef GAME_HPP
#define GAME_HPP

#include <bn_core.h>
#include <bn_keypad.h>
#include <bn_optional.h>
#include <bn_string.h>
#include <bn_fixed.h>
#include <bn_color.h>
#include <bn_random.h>

#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_palettes.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_text_generator.h>

#include <bn_bg_palettes.h>
#include <bn_regular_bg_item.h>
#include <bn_regular_bg_ptr.h>

const int COIN_SPACING = 150;
const int CARD_SPACING = 52;

class Game {
    public:
        bn::optional<bn::sprite_ptr> gameSprite;
        int spriteState;
        int stateMax;
        unsigned int salt;

        Game(int initState, int xPos, int yPos, unsigned int seed, bn::sprite_item sprite);

        Game(unsigned int seed);

        void add_salt();
};

class CoinGame: public Game {
    public:
        using Game::Game;

        CoinGame(int initState, int maxState, int x, int y, bn::sprite_item sprite, int seed);

        int select_coin();

        int flip_coin();

        int determine_toss();
};

class CardGame: public Game {
    public:
        using Game::Game;
        bn::vector<bn::sprite_ptr, 4> cards;


        CardGame(int target, bn::sprite_item sprite, int seed);

        void add_cards(bn::sprite_ptr addMe);

        void empty_cards();
        /* matches outer cards x pos to inner cards
         * then brings all cards x pos to center
         */
        void stack_cards();

        /* 0 = showing
         * 1 = hidden
         */
        void flip_cards(int flipTo, char order[]);

        /*
         * swaps cards in array and tracks order
         */
        void shuffle_cards(int& targetShape, char order[]);

        /* 
         * returns true if correct card is picked
         */
        bool correct_card(int startingPos, int target);

        /* 
         * spreads cards
         */
        void spread_cards(int outerPos, int innerPos);
};

class DiceGame: public Game {

    public:
        using Game::Game;
        DiceGame(int initState, int x, int y, unsigned int seed, bn::sprite_item sprite);

        int shuffle_dice();
};
#endif
