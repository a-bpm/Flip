#ifndef BN_REGULAR_BG_ITEMS_PRESS_START_H
#define BN_REGULAR_BG_ITEMS_PRESS_START_H

#include "bn_regular_bg_item.h"

//{{BLOCK(press_start_bn_gfx)

//======================================================================
//
//	press_start_bn_gfx, 256x256@8, 
//	+ palette 176 entries, not compressed
//	+ 99 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 352 + 6336 + 2048 = 8736
//
//	Time-stamp: 2022-07-19, 12:52:28
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PRESS_START_BN_GFX_H
#define GRIT_PRESS_START_BN_GFX_H

#define press_start_bn_gfxTilesLen 6336
extern const bn::tile press_start_bn_gfxTiles[198];

#define press_start_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell press_start_bn_gfxMap[1024];

#define press_start_bn_gfxPalLen 352
extern const bn::color press_start_bn_gfxPal[176];

#endif // GRIT_PRESS_START_BN_GFX_H

//}}BLOCK(press_start_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item press_start(
            regular_bg_tiles_item(span<const tile>(press_start_bn_gfxTiles, 198), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(press_start_bn_gfxPal, 176), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(press_start_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

