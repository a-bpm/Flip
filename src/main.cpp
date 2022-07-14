#include "bn_core.h"
#include <bn_keypad.h>

#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_builder.h>
#include "bn_sprite_items_shapecons.h"

#include <bn_regular_bg_item.h>
#include <bn_regular_bg_ptr.h>
#include "bn_regular_bg_items_title.h"

void update_from_button(bn::sprite_ptr& sprite)
{

    if (bn::keypad::a_pressed())
    {
        sprite.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(0));
    }

    if (bn::keypad::b_pressed())
    {
        sprite.set_tiles(bn::sprite_items::shapecons.tiles_item().create_tiles(1));
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
}

int main()
{
    bn::core::init();

    bn::sprite_ptr shape = bn::sprite_items::shapecons.create_sprite(0, 30);

    while(true)
    {
        bn::regular_bg_ptr title_screen = bn::regular_bg_items::title.create_bg(0, 0);
        
        update_from_button(shape);

        bn::core::update();
    }
}
