// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "scroll.h"
#include "color.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================
u8  g_Scroll_Count = 0;
u16 g_Scroll_OffsetX = 0;
u16 g_Scroll_OffsetY = 0;
u16 g_Scroll_Map;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void Scroll_Initialize(u16 map)
{	
	g_Scroll_Map = map;

	#if ((MSX_VERSION >= MSX_2) && SCROLL_SPRITE_MASK)
	// Initialize mask sprites
	VDP_EnableSprite(true);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2);
	VDP_FillVRAM(0xFF, g_SpritePatternLow, g_SpritePatternHigh, 8*4);
	VDP_SetSpriteExUniColor(0,  0, (u8)0,   0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(1,  0, (u8)32,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(2,  0, (u8)64,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(3,  0, (u8)96,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(4,  0, (u8)128, 0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(5,  0, (u8)160, 0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(6,  0, (u8)0,   0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(7,  0, (u8)32,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(8,  0, (u8)64,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(9,  0, (u8)96,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(10, 0, (u8)128, 0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(11, 0, (u8)160, 0, COLOR_BLACK);
	VDP_DisableSpritesFrom(12);
	#endif
}

//-----------------------------------------------------------------------------
// 
void Scroll_SetOffset(i8 offset)
{
	if(offset > 0)
	{
		if(g_Scroll_OffsetX + offset < (SCROLL_SRC_W - SCROLL_DST_W) * 8)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = (SCROLL_SRC_W - SCROLL_DST_W) * 8;
	}
	else
	{
		if(g_Scroll_OffsetX > -offset)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = 0;
	}

	// if(offset > 0)
	// {
		// #if (SCROLL_WRAP)
		// g_Scroll_OffsetX++;
		// if(g_Scroll_OffsetX == SCROLL_SRC_W)
			// g_Scroll_OffsetX = 0;
		// #else
		// if(g_Scroll_OffsetX < SCROLL_SRC_W - SCROLL_DST_W)
			// g_Scroll_OffsetX++;
		// #endif
	// }
	// else if(offset < 0)
	// {
		// #if (SCROLL_WRAP)
		// g_Scroll_OffsetX--;
		// if(g_Scroll_OffsetX == 0xFFFF)
			// g_Scroll_OffsetX = SCROLL_SRC_W-1;
		// #else
		// if(g_Scroll_OffsetX > 0)
			// g_Scroll_OffsetX--;
		// #endif
	// }
}

//-----------------------------------------------------------------------------
// 
void Scroll_Update()
{	
	#if (SCROLL_SKIP != SCROLL_SKIP_NONE)
	if(count & SCROLL_SKIP)
		continue;
	#endif

	#if (MSX_VERSION >= MSX_2)
	u8 offsetStep = g_Scroll_OffsetX & 0x7;
	VDP_SetAdjustOffset(offsetStep);
		#if (SCROLL_SPRITE_MASK)
		VDP_SetSpritePosition(0, offsetStep, (u8)255+0); // Left mask
		VDP_SetSpritePosition(1, offsetStep, (u8)255+32);
		VDP_SetSpritePosition(2, offsetStep, (u8)255+64);
		VDP_SetSpritePosition(3, offsetStep, (u8)255+96);
		VDP_SetSpritePosition(4, offsetStep, (u8)255+128);
		VDP_SetSpritePosition(5, offsetStep, (u8)255+160);
		offsetStep += 255 - 7;
		VDP_SetSpritePosition(6,  offsetStep, (u8)255+0); // Right mask
		VDP_SetSpritePosition(7,  offsetStep, (u8)255+32);
		VDP_SetSpritePosition(8,  offsetStep, (u8)255+64);
		VDP_SetSpritePosition(9,  offsetStep, (u8)255+96);
		VDP_SetSpritePosition(10, offsetStep, (u8)255+128);
		VDP_SetSpritePosition(11, offsetStep, (u8)255+160);
		#endif
	#endif

	u16 offsetTileX = g_Scroll_OffsetX >> 3;

	u16 src = (u16)g_Scroll_Map + offsetTileX + SCROLL_SRC_Y * SCROLL_SRC_W;
	u16 dst = g_ScreenLayoutLow + (SCROLL_DST_Y * 32) + SCROLL_DST_X;
	for(u8 y = 0; y < SCROLL_DST_H; ++y)
	{
		#if(SCROLL_WRAP)
		if(offsetTileX + (SCROLL_DST_W * 8) > (SCROLL_SRC_W * 8))
		{
			u16 sw = SCROLL_SRC_W - offsetTileX;
			VDP_WriteVRAM_16K((const u8*)src, dst, sw);
			VDP_WriteVRAM_16K((const u8*)src - offsetTileX, dst + sw, SCROLL_DST_W - sw);
		}
		else
		#endif
		VDP_WriteVRAM_16K((const u8*)src, dst, SCROLL_DST_W);
		dst += 32;
		src += SCROLL_SRC_W;
	}
}


