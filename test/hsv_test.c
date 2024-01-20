#include "minunit.h"

#include "hsv.h"
#include "hsv_test.h"

char *test_hsv_size(void) {
  char *message;
  hsv_t hsv = {0};
  muAssertLongEq(sizeof(hsv), 3L, "hsv size");

  rgb_t rgb = {0};
  muAssertLongEq(sizeof(rgb), 3L, "rgb size");
  return NULL;
}

char *test_hsv_to_and_from_rainbow(void) {
  rgb_t rgb = {0};
  rgb_t expected_rgb = {0};
  hsv_t hsv = {0};

  // Red
  rgb = (rgb_t){.r = 0xff, .g = 0x00, .b = 0x00};
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.hue, 0, "red.h");
  muAssertIntEq(hsv.sat, 255, "red.s");
  muAssertIntEq(hsv.val, 255, "red.v");

  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0xff, "rgb.r");
  muAssertIntEq(rgb.g, 0x00, "rgb.g");
  muAssertIntEq(rgb.b, 0x00, "rgb.b");

  // Green
  rgb = (rgb_t){.r = 0x00, .g = 0xff, .b = 0x00};
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.h, 96, "hsv.h");
  muAssertIntEq(hsv.s, 255, "hsv.s");
  muAssertIntEq(hsv.v, 255, "hsv.v");

  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0x00, "rgb.r");
  muAssertIntEq(rgb.g, 0xff, "rgb.g");
  muAssertIntEq(rgb.b, 0x00, "rgb.b");

  // Light Green
  rgb = (rgb_t){.r = 0x00, .g = 0x0f, .b = 0x00};
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.h, 96, "hsv.h");
  muAssertIntEq(hsv.s, 255, "hsv.s");
  muAssertIntEq(hsv.v, 61, "hsv.v");

  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0x00, "rgb.r");
  muAssertIntEq(rgb.g, 0x0f, "rgb.g");
  muAssertIntEq(rgb.b, 0x00, "rgb.b");

  // Yellow
  rgb = (rgb_t){.r = 0x7f, .g = 0x7f, .b = 0x00};
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.h, 63, "hsv.h");
  muAssertIntEq(hsv.s, 255, "hsv.s");
  muAssertIntEq(hsv.v, 254, "hsv.v");

  // This round trip takes red from 127 to 169, and green from 127 to 166
  // This is pretty far from the color we started with!
  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0xa9, "rgb.r");
  muAssertIntEq(rgb.g, 0xa6, "rgb.g");
  muAssertIntEq(rgb.b, 0x00, "rgb.b");

  // Cyan (AND BLUE return the SAME HSV)
  rgb = (rgb_t){.r = 0, .g = 0xff, .b = 0xff};
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.h, 129, "hsv.h"); // Originally returned 160?!
  muAssertIntEq(hsv.s, 255, "hsv.s");
  muAssertIntEq(hsv.v, 255, "hsv.v");

  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0x00, "rgb.r");
  muAssertIntEq(rgb.g, 0xff, "rgb.g");
  muAssertIntEq(rgb.b, 0xff, "rgb.b");

  // Magenta (Returns wrong HSV)
  rgb = (rgb_t){.r = 0xff, .g = 0, .b = 0xff};
  printf("-----------------------\n");
  hsv_from_rgb_approximate(&hsv, &rgb);
  muAssertIntEq(hsv.h, 0, "hsv.h"); // Should be 212!
  muAssertIntEq(hsv.s, 255, "hsv.s");
  muAssertIntEq(hsv.v, 255, "hsv.v");

  // Returns wrong RGB
  hsv_to_rgb_rainbow(&hsv, &rgb);
  muAssertIntEq(rgb.r, 0xff, "rgb.r");
  muAssertIntEq(rgb.g, 0x0, "rgb.g");
  muAssertIntEq(rgb.b, 0x0, "rgb.b"); // Should be 255!

  // Dark Red
  // rgb = (rgb_t){.r = 0x80, .g = 0, .b = 0};
  //
  // Dark Blue
  // rgb = (rgb_t){.r = 0, .g = 0, .b = 0x80};
  //
  // Dark Green
  // rgb = (rgb_t){.r = 0, .g = 0x80, .b = 0};
  //
  // Dark, Ugly Yellow
  // rgb = (rgb_t){.r = 0x80, .g = 0x80, .b = 0};
  //
  // Dark Cyan
  // rgb = (rgb_t){.r = 0, .g = 0x80, .b = 0x80};
  //
  // Dark Purple
  // rgb = (rgb_t){.r = 0x80, .g = 0, .b = 0x80};
  //
  // Gray
  // rgb = (rgb_t){.r = 0xbf, .g = 0xbf, .b = 0xbf};
  //
  // Dark Gray
  // rgb = (rgb_t){.r = 0x80, .g = 0x80, .b = 0x80};
  return NULL;
}

// char *test_hsv_to_bytes(void) {
//   size_t count = 10;
//   size_t pixel_size = sizeof(grb_color);
//   grb_color grb_pixels[] = {
//     {.r = 0xff, .g = 0x00, .b = 0x00},
//     {.r = 0x00, .g = 0xff, .b = 0x00},
//     {.r = 0x00, .g = 0x00, .b = 0xff},
//     {.r = 0xff, .g = 0x00, .b = 0xff},
//     {.r = 0x00, .g = 0xff, .b = 0xff},
//     {.r = 0xff, .g = 0xff, .b = 0x00},
//     {.r = 0xff, .g = 0xff, .b = 0xff},
//     {.r = 0x00, .g = 0x00, .b = 0x00},
//     {.r = 0xbe, .g = 0x00, .b = 0x00},
//     {.r = 0x00, .g = 0xbe, .b = 0x00},
//     {.r = 0x00, .g = 0x00, .b = 0xbe},
//   };
// 
//   u8_t *pixels = (u8_t *)&grb_pixels;
//   u32_t data = 0;
// 
//   while (count-- > 0) {
//     data |= (pixels[0] << 16);
//     data |= (pixels[1] << 8);
//     data |= (pixels[2]);
//     // printf("data: 0x%06x\n", data);
//     pixels += pixel_size;
//     data = 0;
//   }
// 
//   return NULL;
// }
// 
// char *test_hsv_compare(void) {
//   hsv_t hsv_a = {.h = 0,   .s = 255, .v = 255};
//   hsv_t hsv_b = {.h = 0,   .s = 255, .v = 255};
// 
//   muAssertIntEq(hsv_compare_colors(&hsv_a, &hsv_b), 0, "hsv_compare");
//   return NULL;
// }
// 
// char *test_hsv_to_rgb_colors(void) {
//   hsv_t result;
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0, .b = 0});
//   muAssertIntEq(result.h, 0, "black.h");
//   muAssertIntEq(result.s, 0, "black.s");
//   muAssertIntEq(result.v, 0, "black.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0xff, .g = 0xff, .b = 0xff});
//   muAssertIntEq(result.h, 0, "white.h");
//   muAssertIntEq(result.s, 0, "white.s");
//   muAssertIntEq(result.v, 255, "white.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0xff, .g = 0, .b = 0});
//   muAssertIntEq(result.h, 0, "red.h");
//   muAssertIntEq(result.s, 255, "red.s");
//   muAssertIntEq(result.v, 255, "red.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0xff, .b = 0});
//   muAssertIntEq(result.h, 85, "lime.h");
//   muAssertIntEq(result.s, 255, "lime.s");
//   muAssertIntEq(result.v, 255, "lime.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0, .b = 0xff});
//   muAssertIntEq(result.h, 170, "blue.h");
//   muAssertIntEq(result.s, 255, "blue.s");
//   muAssertIntEq(result.v, 255, "blue.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0xff, .g = 0xff, .b = 0});
//   muAssertIntEq(result.h, 43, "yellow.h");
//   muAssertIntEq(result.s, 255, "yellow.s");
//   muAssertIntEq(result.v, 255, "yellow.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0xff, .b = 0xff});
//   muAssertIntEq(result.h, 128, "cyan.h");
//   muAssertIntEq(result.s, 255, "cyan.s");
//   muAssertIntEq(result.v, 255, "cyan.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0xff, .g = 0, .b = 0xff});
//   muAssertIntEq(result.h, 213, "magenta.h");
//   muAssertIntEq(result.s, 255, "magenta.s");
//   muAssertIntEq(result.v, 255, "magenta.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0xbf, .g = 0xbf, .b = 0xbf});
//   muAssertIntEq(result.h, 0, "silver.h");
//   muAssertIntEq(result.s, 0, "silver.s");
//   muAssertIntEq(result.v, 191, "silver.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0x80, .g = 0x80, .b = 0x80});
//   muAssertIntEq(result.h, 0, "gray.h");
//   muAssertIntEq(result.s, 0, "gray.s");
//   muAssertIntEq(result.v, 128, "gray.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0x80, .g = 0, .b = 0});
//   muAssertIntEq(result.h, 0, "maroon.h");
//   muAssertIntEq(result.s, 255, "maroon.s");
//   muAssertIntEq(result.v, 128, "maroon.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0x80, .g = 0x80, .b = 0});
//   muAssertIntEq(result.h, 43, "olive.h");
//   muAssertIntEq(result.s, 255, "olive.s");
//   muAssertIntEq(result.v, 128, "olive.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0x80, .b = 0});
//   muAssertIntEq(result.h, 85, "green.h");
//   muAssertIntEq(result.s, 255, "green.s");
//   muAssertIntEq(result.v, 128, "green.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0x80, .g = 0, .b = 0x80});
//   muAssertIntEq(result.h, 213, "purple.h");
//   muAssertIntEq(result.s, 255, "purple.s");
//   muAssertIntEq(result.v, 128, "purple.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0x80, .b = 0x80});
//   muAssertIntEq(result.h, 128, "teal.h");
//   muAssertIntEq(result.s, 255, "teal.s");
//   muAssertIntEq(result.v, 128, "teal.v");
// 
//   result = hsv_from_rgb((grb_color){.r = 0, .g = 0, .b = 0x80});
//   muAssertIntEq(result.h, 170, "navy.h");
//   muAssertIntEq(result.s, 255, "navy.s");
//   muAssertIntEq(result.v, 128, "navy.v");
//   return NULL;
// }
// 
// char *test_hsv_near_yellow(void) {
//   hsv_t hsv = hsv_from_rgb((grb_color){.r = 0xff, .g = 0xcc, .b = 0x66});
//   muAssertIntEq(hsv.h, 28, "hsv.h"); // 40 degrees for hue (i.e., 28)
//   muAssertIntEq(hsv.s, 153, "hsv.s"); // 60% saturation (i.e., 153)
//   muAssertIntEq(hsv.v, 255, "hsv.v"); // 100% value (i.e., 255)
// 
//   grb_color result = hsv_to_rgb(hsv);
//   muAssertIntEq(result.r, 0xff, "result.r");
//   muAssertIntEq(result.g, 0xcb, "result.g"); // Close, but not exact!
//   muAssertIntEq(result.b, 0x65, "result.b"); // Close, but not exact!
//   return NULL;
// }
// 
// char *test_hsv_yellow(void) {
//   grb_color rgb = {.r = 0xff, .g = 0xcc, .b = 0x00};
//   hsv_t hsv = hsv_from_rgb(rgb);
//   grb_color result = hsv_to_rgb(hsv);
// 
//   muAssertIntEq(result.r, 0xff, "result.r");
//   muAssertIntEq(result.g, 0xcc, "result.g");
//   muAssertIntEq(result.b, 0x00, "result.b");
//   return NULL;
// }
// 
// char *test_hsv_interpolate_rgb(void) {
//   grb_color rgb_a = {.r = 0xff, .g = 0,    .b = 0};
//   grb_color rgb_b = {.r = 0,    .g = 0xff, .b = 0};
// 
//   grb_color result = hsv_interpolate_rgb(rgb_a, rgb_b, 0.5);
//   muAssertIntEq(result.r, 255, "result.r");
//   muAssertIntEq(result.g, 252, "result.g");
//   muAssertIntEq(result.b, 0, "result.b");
// 
//   result = hsv_interpolate_rgb(rgb_a, rgb_b, 0.75);
//   muAssertIntEq(result.r, 135, "result.r");
//   muAssertIntEq(result.g, 255, "result.g");
//   muAssertIntEq(result.b, 0, "result.b");
// 
//   result = hsv_interpolate_rgb(rgb_a, rgb_b, 0.9);
//   muAssertIntEq(result.r, 57, "result.r");
//   muAssertIntEq(result.g, 255, "result.g");
//   muAssertIntEq(result.b, 0, "result.b");
// 
//   result = hsv_interpolate_rgb(rgb_a, rgb_b, 1.0);
//   muAssertIntEq(result.r, 3, "result.r");
//   muAssertIntEq(result.g, 255, "result.g");
//   muAssertIntEq(result.b, 0, "result.b");
//   return NULL;
// }
// 
// char *test_hsv_interpolate(void) {
//   grb_color rgb_a = {.r = 0xff, .g = 0,    .b = 0};
//   grb_color rgb_b = {.r = 0,    .g = 0xff, .b = 0};
//   hsv_t hsv_a = hsv_from_rgb(rgb_a);
//   hsv_t hsv_b = hsv_from_rgb(rgb_b);
// 
//   hsv_t hsv_result = hsv_interpolate(hsv_a, hsv_b, 0.5);
//   grb_color rgb_result = hsv_to_rgb(hsv_result);
//   muAssertIntEq(rgb_result.r, 255, "rgb_result.r");
//   muAssertIntEq(rgb_result.g, 252, "rgb_result.g");
//   muAssertIntEq(rgb_result.b, 0, "rgb_result.b");
// 
//   hsv_result = hsv_interpolate(hsv_a, hsv_b, 0.75);
//   rgb_result = hsv_to_rgb(hsv_result);
//   muAssertIntEq(rgb_result.r, 135, "rgb_result.r");
//   muAssertIntEq(rgb_result.g, 255, "rgb_result.g");
//   muAssertIntEq(rgb_result.b, 0, "rgb_result.b");
// 
//   hsv_result = hsv_interpolate(hsv_a, hsv_b, 0.9);
//   rgb_result = hsv_to_rgb(hsv_result);
//   muAssertIntEq(rgb_result.r, 57, "rgb_result.r");
//   muAssertIntEq(rgb_result.g, 255, "rgb_result.g");
//   muAssertIntEq(rgb_result.b, 0, "rgb_result.b");
// 
//   hsv_result = hsv_interpolate(hsv_a, hsv_b, 1.0);
//   rgb_result = hsv_to_rgb(hsv_result);
//   muAssertIntEq(rgb_result.r, 3, "rgb_result.r");
//   muAssertIntEq(rgb_result.g, 255, "rgb_result.g");
//   muAssertIntEq(rgb_result.b, 0, "rgb_result.b");
//   return NULL;
// }
// 
// char *test_hsv_simple(void) {
//   grb_color rgb;
//   hsv_t hsv;
// 
//   // Red round trip
//   rgb = (grb_color){.r = 0xff, .g = 0, .b = 0};
//   hsv = hsv_from_rgb(rgb);
//   muAssertIntEq(hsv.h, 0, "hsv.h");
//   muAssertIntEq(hsv.s, 255, "hsv.s");
//   muAssertIntEq(hsv.v, 255, "hsv.v");
// 
//   rgb = hsv_to_rgb(hsv);
//   muAssertIntEq(rgb.r, 0xff, "rgb.r");
//   muAssertIntEq(rgb.g, 0, "rgb.g");
//   muAssertIntEq(rgb.b, 0, "rgb.b");
// 
//   // Green round trip
//   rgb = (grb_color){.r = 0, .g = 0xff, .b = 0};
//   hsv = hsv_from_rgb(rgb);
//   muAssertIntEq(hsv.h, 85, "hsv.h");
//   muAssertIntEq(hsv.s, 255, "hsv.s");
//   muAssertIntEq(hsv.v, 255, "hsv.v");
// 
//   rgb = hsv_to_rgb(hsv);
//   muAssertIntEq(rgb.r, 0x03, "rgb.r"); // NOTE: Hue is quantized?
//   muAssertIntEq(rgb.g, 0xff, "rgb.g");
//   muAssertIntEq(rgb.b, 0, "rgb.b");
// 
//   // Blue round trip
//   rgb = (grb_color){.r = 0, .g = 0x0, .b = 0xff};
//   hsv = hsv_from_rgb(rgb);
//   muAssertIntEq(hsv.h, 170, "hsv.h");
//   muAssertIntEq(hsv.s, 255, "hsv.s");
//   muAssertIntEq(hsv.v, 255, "hsv.v");
// 
//   rgb = hsv_to_rgb(hsv);
//   muAssertIntEq(rgb.r, 0, "rgb.r");
//   muAssertIntEq(rgb.g, 9, "rgb.g"); // NOTE: Hue is quantized?
//   muAssertIntEq(rgb.b, 255, "rgb.b");
// 
//   // Orange round trip
//   rgb = (grb_color){.r = 0xff, .g = 0xcc, .b = 0};
//   hsv = hsv_from_rgb(rgb);
//   muAssertIntEq(hsv.h, 34, "hsv.h");
//   muAssertIntEq(hsv.s, 255, "hsv.s");
//   muAssertIntEq(hsv.v, 255, "hsv.v");
// 
//   rgb = hsv_to_rgb(hsv);
//   muAssertIntEq(rgb.r, 0xff, "rgb.r");
//   muAssertIntEq(rgb.g, 0xcc, "rgb.g");
//   muAssertIntEq(rgb.b, 0, "rgb.b");
//   return NULL;
// }
// 
