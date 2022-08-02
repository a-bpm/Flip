#ifndef BN_SPRITE_ITEMS_DICE_H
#define BN_SPRITE_ITEMS_DICE_H

#include "bn_sprite_item.h"

//{{BLOCK(dice_bn_gfx)

//======================================================================
//
//	dice_bn_gfx, 32x192@8, 
//	+ palette 160 entries, not compressed
//	+ 96 tiles not compressed
//	Total size: 320 + 6144 = 6464
//
//	Time-stamp: 2022-08-02, 11:21:22
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DICE_BN_GFX_H
#define GRIT_DICE_BN_GFX_H

#define dice_bn_gfxTilesLen 6144
extern const bn::tile dice_bn_gfxTiles[192];

#define dice_bn_gfxPalLen 320
extern const bn::color dice_bn_gfxPal[160];

#endif // GRIT_DICE_BN_GFX_H

//}}BLOCK(dice_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item dice(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(dice_bn_gfxTiles, 192), bpp_mode::BPP_8, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(dice_bn_gfxPal, 160), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

