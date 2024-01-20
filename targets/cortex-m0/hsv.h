#ifndef FASTLED_HSV_H
#define FASTLED_HSV_H

#include "fastled.h"

/// Pre-defined hue values for CHSV objects
typedef enum {
    HUE_RED = 0,       ///< Red (0°)
    HUE_ORANGE = 32,   ///< Orange (45°)
    HUE_YELLOW = 64,   ///< Yellow (90°)
    HUE_GREEN = 96,    ///< Green (135°)
    HUE_AQUA = 128,    ///< Aqua (180°)
    HUE_BLUE = 160,    ///< Blue (225°)
    HUE_PURPLE = 192,  ///< Purple (270°)
    HUE_PINK = 224     ///< Pink (315°)
} HSVHue;

/**
 * Original source from vendor/fastled/src/hsv2rgb.cpp
 *
 * Edited for types and formatting in our project.
 */
typedef struct {
  union {
    struct {
      union {
        /// Color hue.
        /// This is an 8-bit value representing an angle around
        /// the color wheel. Where 0 is 0°, and 255 is 358°.
        uint8_t hue;
        uint8_t h;  ///< @copydoc hue
      };
      union {
        /// Color saturation.
        /// This is an 8-bit value representing a percentage.
        uint8_t saturation;
        uint8_t sat;  ///< @copydoc saturation
        uint8_t s;    ///< @copydoc saturation
      };
      union {
        /// Color value (brightness).
        /// This is an 8-bit value representing a percentage.
        uint8_t value;
        uint8_t val;  ///< @copydoc value
        uint8_t v;    ///< @copydoc value
      };
    };
    /// Access the hue, saturation, and value data as an array.
    /// Where:
    /// * `raw[0]` is the hue
    /// * `raw[1]` is the saturation
    /// * `raw[2]` is the value
    uint8_t raw[3];
  };
} hsv_t;

typedef struct {
  union {
    struct {
      union {
        uint8_t r;    ///< Red channel value
        uint8_t red;  ///< @copydoc r
      };
      union {
        uint8_t g;      ///< Green channel value
        uint8_t green;  ///< @copydoc g
      };
      union {
        uint8_t b;     ///< Blue channel value
        uint8_t blue;  ///< @copydoc b
      };
    };
    /// Access the red, green, and blue data as an array.
    /// Where:
    /// * `raw[0]` is the red value
    /// * `raw[1]` is the green value
    /// * `raw[2]` is the blue value
    uint8_t raw[3];
  };
} rgb_t;

/**
 * Copied from fastled library.
 */
/// Recover approximate HSV values from RGB. 
/// These values are *approximate*, not exact. Why is this "only" an approximation? 
/// Because not all RGB colors have HSV equivalents!  For example, there
/// is no HSV value that will ever convert to RGB(255,255,0) using
/// the code provided in this library.   So if you try to
/// convert RGB(255,255,0) "back" to HSV, you'll necessarily get
/// only an approximation.  Emphasis has been placed on getting
/// the "hue" as close as usefully possible, but even that's a bit
/// of a challenge.  The 8-bit HSV and 8-bit RGB color spaces
/// are not a "bijection".
///
/// Nevertheless, this function does a pretty good job, particularly
/// at recovering the 'hue' from fully saturated RGB colors that
/// originally came from HSV rainbow colors.  So if you start
/// with CHSV(hue_in,255,255), and convert that to RGB, and then
/// convert it back to HSV using this function, the resulting output
/// hue will either exactly the same, or very close (+/-1).
/// The more desaturated the original RGB color is, the rougher the
/// approximation, and the less accurate the results.
/// @note This function is a long-term work in progress; expect
/// results to change slightly over time as this function is
/// refined and improved.
/// @par
/// @note This function is most accurate when the input is an
/// RGB color that came from a fully-saturated HSV color to start
/// with.  E.g. CHSV( hue, 255, 255) -> CRGB -> CHSV will give
/// best results.
/// @par
/// @note This function is not nearly as fast as HSV-to-RGB.
/// It is provided for those situations when the need for this
/// function cannot be avoided, or when extremely high performance
/// is not needed.
/// @see https://en.wikipedia.org/wiki/Bijection
/// @param rgb an RGB value to convert
/// @returns the approximate HSV equivalent of the RGB value
void hsv_from_rgb_approximate(hsv_t *hsv, const rgb_t *rgb);

/**
 * This is an approximattion of the HSV to RGB conversion.
 *
 * Unlike the hsv_to_rgb_rainbow implementation, this will get closer to the
 * expected color, based on the provided HSV value.
 *
 * The rainbow implementation seems to do fine on full, single channel colors,
 * (e.g., red, green and blue), but it's not even close for secondary or
 * tertierary colors (e.g., cyan, magenta, etc.).
 *
 * This implementation is based on algorithms found here:
 * https://www.rapidtables.com/convert/color/rgb-to-hsv.html
 */
void hsv_to_rgb_approximate(const hsv_t *hsv, rgb_t *rgb);

void hsv_to_rgb_fast(const hsv_t *hsv, rgb_t *rgb);

void hsv_to_rgb_rainbow(const hsv_t *hsv, rgb_t *rgb);

#endif // FASTLED_HSV_H
