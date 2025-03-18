

#ifndef LightShowLED_palettes_h
#define LightShowLED_palettes_h


#include "FastLED.h"

// Christmas

DEFINE_GRADIENT_PALETTE(red_gp){
  0, 173, 20, 10,  // red
  43, 0, 0, 0,
  73, 173, 20, 10,  // red
  137, 0, 0, 0,
  183, 173, 20, 10,  // red
  232, 0, 0, 0,
  255, 173, 20, 10,  // red
};

DEFINE_GRADIENT_PALETTE(white_gp){
  0, 200, 200, 200,  // white
  43, 0, 0, 0,
  73, 200, 200, 200,  // white
  137, 0, 0, 0,
  183, 200, 200, 200,  // white
  232, 0, 0, 0,
  255, 200, 200, 200,  // white
};

DEFINE_GRADIENT_PALETTE(blue_gp){
  0, 0, 0, 200,  // blue
  43, 0, 0, 0,
  73, 200, 200, 200,  // blue
  137, 0, 0, 0,
  183, 200, 200, 200,  // blue
  232, 0, 0, 0,
  255, 200, 200, 200,  // blue
};

DEFINE_GRADIENT_PALETTE(green_gp){
  0, 53, 173, 1,  // green
  43, 0, 0, 0,
  73, 53, 173, 1,  // green
  137, 0, 0, 0,
  183, 53, 173, 1,  // green
  232, 0, 0, 0,
  255, 53, 173, 1,  // green
};

// Halloween


DEFINE_GRADIENT_PALETTE(orange_gp){
  0, 173, 53, 1,  //orange
  43, 0, 0, 0,
  73, 173, 53, 1,  //orange
  137, 0, 0, 0,
  183, 173, 53, 1,  //orange
  232, 0, 0, 0,
  255, 173, 53, 1,  //orange
};

DEFINE_GRADIENT_PALETTE(purple_gp){
  0, 37, 1, 80,  // purple
  43, 0, 0, 0,
  73, 37, 1, 80,  // purple
  137, 0, 0, 0,
  183, 37, 1, 80,  // purple
  232, 0, 0, 0,
  255, 37, 1, 80,  // purple
};

DEFINE_GRADIENT_PALETTE(hgreen_gp){
  0, 13, 60, 6,  // green
  43, 0, 0, 0,
  73, 13, 60, 6,  // green
  137, 0, 0, 0,
  183, 13, 60, 6,  // green
  232, 0, 0, 0,
  255, 13, 60, 6,  // green
};

DEFINE_GRADIENT_PALETTE(rwb_gp){
  0, 173, 20, 10,  // red
  43, 0, 0, 0,
  73, 200, 200, 200,  // white
  137, 0, 0, 0,
  183, 0, 0, 200,  // blue
  232, 0, 0, 0,
  255, 173, 20, 10,  // red
};

DEFINE_GRADIENT_PALETTE(christmas_gp){
  0, 255, 0, 0,       // Bright red
  43, 0, 0, 0,        // Black (off)
  73, 255, 200, 150,  // Warm white
  137, 0, 0, 0,       // Black (off)
  183, 0, 255, 0,     // Bright green
  232, 0, 0, 0,       // Black (off)
  255, 255, 0, 0      // Bright red
};

DEFINE_GRADIENT_PALETTE(warm_white_gp){
  0, 255, 200, 150,    // Warm white
  23, 0, 0, 0,         // Black (off)
  73, 255, 200, 150,   // Warm white
  117, 0, 0, 0,        // Black (off)
  183, 255, 200, 150,  // Warm white
  212, 0, 0, 0,        // Black (off)
  255, 255, 200, 150,  // Warm white
};

DEFINE_GRADIENT_PALETTE(warm_white2_gp){
  0, 225, 160, 36,    // Warm white
  23, 0, 0, 0,        // Black (off)
  73, 225, 160, 36,   // Warm white
  117, 0, 0, 0,       // Black (off)
  183, 225, 160, 36,  // Warm white
  212, 0, 0, 0,       // Black (off)
  255, 225, 160, 36,  // Warm white
};

DEFINE_GRADIENT_PALETTE(blue_vivid_gp){
  0, 0, 0, 254,    // Warm white
  23, 0, 0, 0,     // Black (off)
  73, 0, 0, 254,   // Warm white
  117, 0, 0, 0,    // Black (off)
  183, 0, 0, 254,  // Warm white
  212, 0, 0, 0,    // Black (off)
  255, 0, 0, 254,  // Warm white
};

CRGBPalette16 halloween_palettes[] = { orange_gp, purple_gp, hgreen_gp };
CRGBPalette16 rwb_palettes[] = { rwb_gp };
CRGBPalette16 white_palettes[] = { warm_white2_gp };
CRGBPalette16 blue_palettes[] = { blue_vivid_gp };
CRGBPalette16 xmas_palettes[] = { christmas_gp };

uint8_t xmas_palette_count = sizeof(xmas_palettes) / sizeof(xmas_palettes[0]);
uint8_t halloween_palette_count = sizeof(halloween_palettes) / sizeof(halloween_palettes[0]);
uint8_t rwb_palette_count = sizeof(rwb_palettes) / sizeof(rwb_palettes[0]);
uint8_t white_palette_count = sizeof(white_palettes) / sizeof(white_palettes[0]);
uint8_t blue_palette_count = sizeof(blue_palettes) / sizeof(blue_palettes[0]);
#endif