
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/// Define a LIB8TION member function as static inline with an "unused" attribute
#define LIB8STATIC __attribute__ ((unused)) static inline
/// Define a LIB8TION member function as always static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline

///////////////////////////////////////////////////////////////////////
///
/// @defgroup FractionalTypes Fixed-Point Fractional Types. 
/// Types for storing fractional data. 
///
/// * ::sfract7 should be interpreted as signed 128ths.
/// * ::fract8 should be interpreted as unsigned 256ths.
/// * ::sfract15 should be interpreted as signed 32768ths.
/// * ::fract16 should be interpreted as unsigned 65536ths.
///
/// Example: if a fract8 has the value "64", that should be interpreted
///          as 64/256ths, or one-quarter.
///
/// accumXY types should be interpreted as X bits of integer,
///         and Y bits of fraction.  
/// E.g., ::accum88 has 8 bits of int, 8 bits of fraction
///
/// @{

/// ANSI: unsigned short _Fract. 
/// Range is 0 to 0.99609375 in steps of 0.00390625.  
/// Should be interpreted as unsigned 256ths.
typedef uint8_t fract8;

/// ANSI: signed short _Fract. 
/// Range is -0.9921875 to 0.9921875 in steps of 0.0078125.  
/// Should be interpreted as signed 128ths.
typedef int8_t sfract7;

/// ANSI: unsigned _Fract.
/// Range is 0 to 0.99998474121 in steps of 0.00001525878.  
/// Should be interpreted as unsigned 65536ths.
typedef uint16_t fract16;

/// ANSI: signed _Fract.
/// Range is -0.99996948242 to 0.99996948242 in steps of 0.00003051757.  
/// Should be interpreted as signed 32768ths.
typedef int16_t sfract15;

// typedef uint16_t  accum88;    ///< ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
// typedef int16_t   saccum78;   ///< ANSI: signed   short _Accum. 7 bits int, 8 bits fraction
// typedef uint32_t  accum1616;  ///< ANSI: signed         _Accum. 16 bits int, 16 bits fraction
// typedef int32_t   saccum1516; ///< ANSI: signed         _Accum. 15 bits int, 16 bits fraction
// typedef uint16_t  accum124;   ///< no direct ANSI counterpart. 12 bits int, 4 bits fraction
// typedef int32_t   saccum114;  ///< no direct ANSI counterpart. 1 bit int, 14 bits fraction

/// Scale one byte by a second one, which is treated as
/// the numerator of a fraction whose denominator is 256. 
///
/// In other words, it computes i * (scale / 256)
/// @param i input value to scale
/// @param scale scale factor, in n/256 units
/// @returns scaled value
/// @note Takes 4 clocks on AVR with MUL, 2 clocks on ARM
LIB8STATIC_ALWAYS_INLINE uint8_t scale8( uint8_t i, fract8 scale)
{
#if SCALE8_C == 1
#if (FASTLED_SCALE8_FIXED == 1)
    return (((uint16_t)i) * (1+(uint16_t)(scale))) >> 8;
#else
    return ((uint16_t)i * (uint16_t)(scale) ) >> 8;
#endif
#elif SCALE8_AVRASM == 1
#if defined(LIB8_ATTINY)
#if (FASTLED_SCALE8_FIXED == 1)
    uint8_t work=i;
#else
    uint8_t work=0;
#endif
    uint8_t cnt=0x80;
    asm volatile(
#if (FASTLED_SCALE8_FIXED == 1)
        "  inc %[scale]                 \n\t"
        "  breq DONE_%=                 \n\t"
        "  clr %[work]                  \n\t"
#endif
        "LOOP_%=:                       \n\t"
        /*"  sbrc %[scale], 0             \n\t"
        "  add %[work], %[i]            \n\t"
        "  ror %[work]                  \n\t"
        "  lsr %[scale]                 \n\t"
        "  clc                          \n\t"*/
        "  sbrc %[scale], 0             \n\t"
        "  add %[work], %[i]            \n\t"
        "  ror %[work]                  \n\t"
        "  lsr %[scale]                 \n\t"
        "  lsr %[cnt]                   \n\t"
        "brcc LOOP_%=                   \n\t"
        "DONE_%=:                       \n\t"
        : [work] "+r" (work), [cnt] "+r" (cnt)
        : [scale] "r" (scale), [i] "r" (i)
        :
    );
    return work;
#else
    asm volatile(
#if (FASTLED_SCALE8_FIXED==1)
        // Multiply 8-bit i * 8-bit scale, giving 16-bit r1,r0
        "mul %0, %1          \n\t"
        // Add i to r0, possibly setting the carry flag
        "add r0, %0         \n\t"
        // load the immediate 0 into i (note, this does _not_ touch any flags)
        "ldi %0, 0x00       \n\t"
        // walk and chew gum at the same time
        "adc %0, r1          \n\t"
#else
        /* Multiply 8-bit i * 8-bit scale, giving 16-bit r1,r0 */
        "mul %0, %1          \n\t"
        /* Move the high 8-bits of the product (r1) back to i */
        "mov %0, r1          \n\t"
        /* Restore r1 to "0"; it's expected to always be that */
#endif
        "clr __zero_reg__    \n\t"

        : "+a" (i)      /* writes to i */
        : "a"  (scale)  /* uses scale */
        : "r0", "r1"    /* clobbers r0, r1 */
    );
    /* Return the result */
    return i;
#endif
#else
#error "No implementation for scale8 available."
#endif
}

/// This version of scale8_video() does not clean up the R1 register on AVR. 
/// If you are doing several "scale8_video()'s" in a row, use this, and
/// then explicitly call cleanup_R1().
/// @warning You **MUST** call cleanup_R1() after using this function!
/// @param i input value to scale
/// @param scale scale factor, in n/256 units
/// @returns scaled value
/// @see scale8_video()
LIB8STATIC_ALWAYS_INLINE uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
#if SCALE8_C == 1 || defined(LIB8_ATTINY)
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
#elif SCALE8_AVRASM == 1
    uint8_t j=0;
    asm volatile(
        "  tst %[i]\n\t"
        "  breq L_%=\n\t"
        "  mul %[i], %[scale]\n\t"
        "  mov %[j], r1\n\t"
        "  breq L_%=\n\t"
        "  subi %[j], 0xFF\n\t"
        "L_%=: \n\t"
        : [j] "+a" (j)
        : [i] "a" (i), [scale] "a" (scale)
        : "r0", "r1"
    );
    return j;
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // asm volatile(
    //      "      tst %0           \n"
    //      "      breq L_%=        \n"
    //      "      mul %0, %1       \n"
    //      "      mov %0, r1       \n"
    //      "      add %0, %2       \n"
    //      "      clr __zero_reg__ \n"
    //      "L_%=:                  \n"
    //      : "+a" (i)
    //      : "a" (scale), "a" (nonzeroscale)
    //      : "r0", "r1");
    // // Return the result
    // return i;
#else
#error "No implementation for scale8_video_LEAVING_R1_DIRTY available."
#endif
}

/// The "video" version of scale8() guarantees that the output will
/// be only be zero if one or both of the inputs are zero. 
/// If both inputs are non-zero, the output is guaranteed to be non-zero.  
/// This makes for better "video"/LED dimming, at the cost of
/// several additional cycles.
/// @param i input value to scale
/// @param scale scale factor, in n/256 units
/// @returns scaled value
/// @see scale8()
LIB8STATIC_ALWAYS_INLINE uint8_t scale8_video( uint8_t i, fract8 scale)
{
#if SCALE8_C == 1 || defined(LIB8_ATTINY)
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
#elif SCALE8_AVRASM == 1
    uint8_t j=0;
    asm volatile(
        "  tst %[i]\n\t"
        "  breq L_%=\n\t"
        "  mul %[i], %[scale]\n\t"
        "  mov %[j], r1\n\t"
        "  clr __zero_reg__\n\t"
        "  cpse %[scale], r1\n\t"
        "  subi %[j], 0xFF\n\t"
        "L_%=: \n\t"
        : [j] "+a" (j)
        : [i] "a" (i), [scale] "a" (scale)
        : "r0", "r1"
    );
    return j;
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // asm volatile(
    //      "      tst %0           \n"
    //      "      breq L_%=        \n"
    //      "      mul %0, %1       \n"
    //      "      mov %0, r1       \n"
    //      "      add %0, %2       \n"
    //      "      clr __zero_reg__ \n"
    //      "L_%=:                  \n"
    //      : "+a" (i)
    //      : "a" (scale), "a" (nonzeroscale)
    //      : "r0", "r1");
    // // Return the result
    // return i;
#else
#error "No implementation for scale8_video available."
#endif
}

/// @defgroup Math Basic Math Operations
/// Fast, efficient 8-bit math functions specifically
/// designed for high-performance LED programming.
///
/// Because of the AVR (Arduino) and ARM assembly language
/// implementations provided, using these functions often
/// results in smaller and faster code than the equivalent
/// program using plain "C" arithmetic and logic.
/// @{

/// Add one byte to another, saturating at 0xFF
/// @param i first byte to add
/// @param j second byte to add
/// @returns the sum of i + j, capped at 0xFF
LIB8STATIC_ALWAYS_INLINE uint8_t qadd8( uint8_t i, uint8_t j)
{
#if QADD8_C == 1
    unsigned int t = i + j;
    if( t > 255) t = 255;
    return t;
#elif QADD8_AVRASM == 1
    asm volatile(
        /* First, add j to i, conditioning the C flag */
        "add %0, %1    \n\t"

        /* Now test the C flag.
        If C is clear, we branch around a load of 0xFF into i.
        If C is set, we go ahead and load 0xFF into i.
        */
        "brcc L_%=     \n\t"
        "ldi %0, 0xFF  \n\t"
        "L_%=: "
        : "+a" (i)
        : "a"  (j)
    );
    return i;
#elif QADD8_ARM_DSP_ASM == 1
    asm volatile( "uqadd8 %0, %0, %1" : "+r" (i) : "r" (j));
    return i;
#else
#error "No implementation for qadd8 available."
#endif
}

/// Square root for 16-bit integers. 
/// About three times faster and five times smaller
/// than Arduino's general `sqrt` on AVR.
LIB8STATIC uint8_t sqrt16(uint16_t x)
{
    if( x <= 1) {
        return x;
    }

    uint8_t low = 1; // lower bound
    uint8_t hi, mid;

    if( x > 7904) {
        hi = 255;
    } else {
        hi = (x >> 5) + 8; // initial estimate for upper bound
    }

    do {
        mid = (low + hi) >> 1;
        if ((uint16_t)(mid * mid) > x) {
            hi = mid - 1;
        } else {
            if( mid == 255) {
                return 255;
            }
            low = mid + 1;
        }
    } while (hi >= low);

    return low - 1;
}

/// Subtract one byte from another, saturating at 0x00
/// @param i byte to subtract from
/// @param j byte to subtract
/// @returns i - j with a floor of 0
LIB8STATIC_ALWAYS_INLINE uint8_t qsub8( uint8_t i, uint8_t j)
{
#if QSUB8_C == 1
    int t = i - j;
    if( t < 0) t = 0;
    return t;
#elif QSUB8_AVRASM == 1

    asm volatile(
        /* First, subtract j from i, conditioning the C flag */
        "sub %0, %1    \n\t"

        /* Now test the C flag.
        If C is clear, we branch around a load of 0x00 into i.
        If C is set, we go ahead and load 0x00 into i.
        */
        "brcc L_%=     \n\t"
        "ldi %0, 0x00  \n\t"
        "L_%=: "
        : "+a" (i)
        : "a"  (j)
    );
    return i;
#else
#error "No implementation for qsub8 available."
#endif
}
