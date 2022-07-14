#ifndef BN_SPRITE_ITEMS_SHAPECONS_H
#define BN_SPRITE_ITEMS_SHAPECONS_H

#include "bn_sprite_item.h"

//{{BLOCK(shapecons_bn_gfx)

//======================================================================
//
//	shapecons_bn_gfx, 32x64@4, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2022-07-14, 15:57:49
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SHAPECONS_BN_GFX_H
#define GRIT_SHAPECONS_BN_GFX_H

#define shapecons_bn_gfxTilesLen 1024
extern const bn::tile shapecons_bn_gfxTiles[32];

#define shapecons_bn_gfxPalLen 32
extern const bn::color shapecons_bn_gfxPal[16];

#endif // GRIT_SHAPECONS_BN_GFX_H

//}}BLOCK(shapecons_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item shapecons(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(shapecons_bn_gfxTiles, 32), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(shapecons_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

