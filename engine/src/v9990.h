// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄   ▄▄   ▄▄   ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ █▄██ █▄██ █▄██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀   ▄█▀  ▄█▀  ▄█▀ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// References:
// - V9990 Application Manual
// - http://msxbanzai.tni.nl/v9990/manual.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "v9990_reg.h"

//=============================================================================
// DEFINES
//=============================================================================

#define V9_PALETTE_YSGBR_16			0
#define V9_PALETTE_GBR_16			1
#define V9_PALETTE_RGB_24			2


// Palette data format (can be YsGRB 16-bits or RGB 24-bits)
#define V9_PALETTE_MODE				V9_PALETTE_RGB_24

// RGB color definition macro
#define V9_RGB(r, g, b)				(u16)(((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))
#define V9_RGBYs(r, g, b, ys)		(u16)((((ys) & 0x01) << 15) + ((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))

// V9990 address structure
struct V9_Address
{
	u8 Lower;
	u8 Center;
	u8 Upper;
};

//
extern struct V9_Address g_V9_Address;

//
#define V9_REG(n)					n
#define V9_RII						0b01000000
#define V9_WII						0b10000000

// Screen modes
enum V9_SCREEN_MODE
{
	V9_MODE_P1, // Pattern mode 0 256x212
	V9_MODE_P2, // Pattern mode 1 512x212
	V9_MODE_B0, // Bitmap mode 1 192x240 (Undocumented v9990 mode)
	V9_MODE_B1, // Bitmap mode 1 256x212
	V9_MODE_B2, // Bitmap mode 2 384x240
	V9_MODE_B3, // Bitmap mode 3 512x212
	V9_MODE_B4, // Bitmap mode 4 768x240
	V9_MODE_B5, // Bitmap mode 5 640x400 (VGA)
	V9_MODE_B6, // Bitmap mode 6 640x480 (VGA)
	V9_MODE_B7, // Bitmap mode 7 1024x212 (Undocumented v9990 mode)
};

// Color modes
enum V9_COLOR_MODE
{	//                 R#6  R#13
	//                 CLRM PLTM YAE
	// 2 bits per pixel
	V9_COLOR_BP2,	// 0    0    0	Color palette (4 colors out of 32768 colors)
	// 4 bits per pixel
	V9_COLOR_BP4,	// 1    0    0	Color palette (16 colors out of 32768 colors)
	V9_COLOR_PP,	// 				Display type when the display mode is P1 or P2
	// 8 bits per pixel
	V9_COLOR_BP6,	// 2    0    0	Color palette (64 colors out of 32768 colors)
	V9_COLOR_BD8,	// 2    1    0	Direct RGB [G:3|R:3|B:2] (256 colors)
	V9_COLOR_BYJK,	// 2    2    0	YJK Decoder (19268 colors)
	V9_COLOR_BYJKP,	// 2    2    1	YJK Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	V9_COLOR_BYUV,	// 2    3    0	YUV Decoder (19268 colors)
	V9_COLOR_BYUVP,	// 2    3    1	YUV Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	// 16 bits per pixel
	V9_COLOR_BD16,	// 3    0    0	Direct RGB [YS|G:5|R:2][R:3|B:5] (32768 colors)
};

#define V9_R06_COLOR_MASK			0b00000011
#define V9_R13_COLOR_MASK			0b11100000

// P1 mode VRAM layout
#define V9_P1_PGT_A					0x00000	// Pattern Generator Table (Layer A). 8160 patterns max
#define V9_P1_SGT					0x00000	// Sprite Generator Table  
#define V9_P1_SPAT					0x3FE00	// Sprite Attribute Table
#define V9_P1_PGT_B					0x40000	// Pattern Generator Table (Layer B). 7680 patterns max
#define V9_P1_PNT_A					0x7C000	// Pattern Name Table (Layer A)
#define V9_P1_PNT_B					0x7E000	// Pattern Name Table (Layer B)

// P2 mode VRAM layout
#define V9_P2_PGT					0x00000 // Pattern Generator Table. 15360 patterns max
#define V9_P2_SGT					0x00000	// Sprite Generator Table
#define V9_P2_SPAT					0x7BE00 // Sprite Attribute Table
#define V9_P2_PNT					0x7C000 // Pattern Name Table

// Bitmap mode VRAM layout
#define V9_BMP_PGT					0x00000 // Pattern Generator Table (bitmap data)
#define V9_BMP_CUR					0x7FE00	// Cursor area (512 bytes)

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Core
//-----------------------------------------------------------------------------

// Function: V9_SetPort
// Set port value
void V9_SetPort(u8 port, u8 value) __PRESERVES(b, d, e, h, iyl, iyh);

// Function: V9_GetPort
// Get port value
u8 V9_GetPort(u8 port) __PRESERVES(b, d, e, h, l, iyl, iyh);

// Function: V9_SetRegister
// Set register value
void V9_SetRegister(u8 reg, u8 val) __PRESERVES(b, c, d, e, h, iyl, iyh);

// Function: V9_GetRegister
// Get register value
u8 V9_GetRegister(u8 reg) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: 
//
inline void V9_SetFlag(u8 reg, u8 mask, u8 flag) { V9_SetRegister(reg, V9_GetRegister(reg) & (~mask) | flag); }

//-----------------------------------------------------------------------------
// Group: Setting
//-----------------------------------------------------------------------------

// Function: V9_SetMode
// Set the current screen mode
void V9_SetMode(u8 mode);

// Function: V9_SetBPP
// Set bit number per pixel
inline void V9_SetBPP(u8 bpp) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_BPP_MASK) | bpp); }

// Function: V9_GetBPP
// Get bit number per pixel
inline u8 V9_GetBPP() { return V9_GetRegister(6) & (~V9_R06_BPP_MASK); }

// Function: V9_SetImageSpaceWidth
// Set number of pixels in X direction of image space
inline void V9_SetImageSpaceWidth(u8 width) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_WIDH_MASK) | width); }

// Function: V9_GetImageSpaceWidth
// Get number of pixels in X direction of image space
inline u8 V9_GetImageSpaceWidth() { return V9_GetRegister(6) & (~V9_R06_WIDH_MASK); }

// Function: 
//
inline void V9_SetDisplayEnable(bool enable) { V9_SetFlag(8, V9_R08_DISP_ON, enable ? V9_R08_DISP_ON : 0); }

// Function: 
//
inline void V9_SetSpriteEnable(bool enable) { V9_SetFlag(8, V9_R08_SPD_OFF, enable ? 0 : V9_R08_SPD_OFF); }

// Function: V9_SetScrollingX
//
void V9_SetScrollingX(u16 x);

// Function: V9_SetScrollingY
//
void V9_SetScrollingY(u16 y);

// Function: V9_SetScrolling
//
inline void V9_SetScrolling(u16 x, u16 y) { V9_SetScrollingX(x);  V9_SetScrollingY(y); }

// Function: V9_SetScrollingBX
//
void V9_SetScrollingBX(u16 x);

// Function: V9_SetScrollingBY
//
void V9_SetScrollingBY(u16 y);

// Function: V9_SetScrollingB
//
inline void V9_SetScrollingB(u16 x, u16 y) { V9_SetScrollingBX(x);  V9_SetScrollingBY(y); }

//-----------------------------------------------------------------------------
// Group: VRAM access
//-----------------------------------------------------------------------------

// Function: V9_SetWriteAddress
// Initialize VRAM address for writing
void V9_SetWriteAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_SetReadAddress
// Initialize VRAM address for reading
void V9_SetReadAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_FillVRAM_CurrentAddr
// 
void V9_FillVRAM_CurrentAddr(u8 value, u16 count);

// Function: V9_WriteVRAM_CurrentAddr
//
void V9_WriteVRAM_CurrentAddr(const u8* src, u16 count);

// Function: V9_ReadVRAM_CurrentAddr
//
void V9_ReadVRAM_CurrentAddr(const u8* dest, u16 count);

// Function: V9_Poke_CurrentAddr
//
void V9_Poke_CurrentAddr(u8 val) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Peek_CurrentAddr
//
u8 V9_Peek_CurrentAddr() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_FillVRAM
//
inline void V9_FillVRAM(u32 addr, u8 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM_CurrentAddr(value, count); }

// Function: V9_WriteVRAM
//
inline void V9_WriteVRAM(u32 addr, const u8* src, u16 count) { V9_SetWriteAddress(addr); V9_WriteVRAM_CurrentAddr(src, count); }

// Function: V9_ReadVRAM
//
inline void V9_ReadVRAM(u32 addr, const u8* dest, u16 count) { V9_SetReadAddress(addr); V9_ReadVRAM_CurrentAddr(dest, count); }

// Function: V9_Poke
//
inline void V9_Poke(u32 addr, u8 val) { V9_SetWriteAddress(addr); V9_Poke_CurrentAddr(val); }

// Function: V9_Peek
//
inline u8 V9_Peek(u32 addr) { V9_SetWriteAddress(addr); return V9_Peek_CurrentAddr(); }

//-----------------------------------------------------------------------------
// Group: Sprite
//-----------------------------------------------------------------------------

// Function: V9_SetSpritePatternAddr
// Set sprite patterns VRAM address
inline void V9_SetSpritePatternAddr(u8 addr) { V9_SetRegister(25, addr); }

//
struct V9_Sprite
{
	u8		Y;			// Sprite Y-coordinate (Actual display position is one line below specified)
	u8		Pattern;	// Sprite Pattern Number (Pattern Offset is specified in R#25 SGBA)
	u16		X  : 10;	// Sprite X-coordinate
	u16		_1 : 2;
	u16		P  : 1;		// Sprite is in front of the front layer when P=0, sprite is behind the front layer when P=1.
	u16		D  : 1;		// Sprite is disabled when D=1
	u16		SC : 2;		// Palette offset for sprite colors
};

// Function: V9_SetSpriteP1
// Set sprite attribute for P1 mode
inline void V9_SetSpriteP1(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P1_SPAT + 4 * id, (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP1
// Set sprite pattern for P1 mode
inline void V9_SetSpritePatternP1(u8 id, u8 pat) { V9_Poke(V9_P1_SPAT + 4 * id + 1, pat); }

// Function: V9_SetSpriteP2
// Set sprite attribute for P2 mode
inline void V9_SetSpriteP2(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P2_SPAT + 4 * id, (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP2
// Set sprite pattern for P2 mode
inline void V9_SetSpritePatternP2(u8 id, u8 pat) { V9_Poke(V9_P2_SPAT + 4 * id + 1, pat); }

//-----------------------------------------------------------------------------
// Group: Palette
//-----------------------------------------------------------------------------

//
struct V9_Color
{
	u16 B  : 5;
	u16 R  : 5;
	u16 G  : 5;
	u16 Ys : 1;
};

#if ((V9_PALETTE_MODE == V9_PALETTE_GBR_16) || (V9_PALETTE_MODE == V9_PALETTE_YSGBR_16))

// Function: V9_SetPaletteEntry
// Set the color of a given palette entry.
//
// Parameters:
//   index - Index of the palette entry (0-63)
//   color - 16 bits color value
//           Format: [Ys:1|green:5|red:5|blue:5]
void V9_SetPaletteEntry(u8 index, u16 color);

// Function: V9_SetPalette
// Set the colors of a given palette entries.
//
// Parameters:
//   first - Index of the first entry to set (0-63)
//   num   - Numer of entries to set (1-64)
//   table - 16 bits color table
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPalette(u8 first, u8 num, const u16* table) { for(u8 i = 0; i < num; ++i) V9_SetPaletteEntry(first++, *(table++)); }

// Function: V9_SetPaletteAll
// Set the colors of all the palette 64 entries.
//
// Parameters:
//   table - 16 bits color table
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPaletteAll(const u16* table) { V9_SetPalette(0, 64, table); }

#elif (V9_PALETTE_MODE == V9_PALETTE_RGB_24)

// Set the color of a given palette entry.
void V9_SetPaletteEntry(u8 index, const u8* color);

// Set the colors of a given palette entries.
inline void V9_SetPalette(u8 first, u8 num, const u8* table) { for(u8 i = 0; i < num; ++i) { V9_SetPaletteEntry(first++, table); table += 3; } }

// Set the colors of all the palette 64 entries.
inline void V9_SetPaletteAll(const u8* table) { V9_SetPalette(0, 64, table); }

#endif

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: V9_Detect
// Detecting the presence of a V9990 card
bool V9_Detect();

// Function: V9_ClearVRAM
// Clear the whole 512 KB of VRAM with zero
void V9_ClearVRAM();