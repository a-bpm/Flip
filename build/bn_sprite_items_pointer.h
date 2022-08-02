#ifndef BN_SPRITE_ITEMS_POINTER_H
#define BN_SPRITE_ITEMS_POINTER_H

#include "bn_sprite_item.h"

//{{BLOCK(pointer_bn_gfx)

//======================================================================
//
//	pointer_bn_gfx, 16x64@8, 
//	+ palette 144 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 288 + 1024 = 1312
//
//	Time-stamp: 2022-08-02, 14:18:39
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_POINTER_BN_GFX_H
#define GRIT_POINTER_BN_GFX_H

#define pointer_bn_gfxTilesLen 1024
extern const bn::tile pointer_bn_gfxTiles[32];

#define pointer_bn_gfxPalLen 288
extern const bn::color pointer_bn_gfxPal[144];

#endif // GRIT_POINTER_BN_GFX_H

//}}BLOCK(pointer_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item pointer(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(pointer_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(pointer_bn_gfxPal, 144), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

