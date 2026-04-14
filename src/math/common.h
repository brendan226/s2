/*
 * Portions adapted from cglm (https://github.com/recp/cglm)
 * Copyright (c) 2015 Recep Aslantas
 * MIT License
 */

#ifndef S2_COMMON_H
#define S2_COMMON_H

#define S2_MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define S2_MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define S2_E         2.71828182845904523536028747135266250   /* e           */
#define S2_LOG2E     1.44269504088896340735992468100189214   /* log2(e)     */
#define S2_LOG10E    0.434294481903251827651128918916605082  /* log10(e)    */
#define S2_LN2       0.693147180559945309417232121458176568  /* loge(2)     */
#define S2_LN10      2.30258509299404568401799145468436421   /* loge(10)    */
#define S2_PI        3.14159265358979323846264338327950288   /* pi          */
#define S2_PI_2      1.57079632679489661923132169163975144   /* pi/2        */
#define S2_PI_4      0.785398163397448309615660845819875721  /* pi/4        */
#define S2_1_PI      0.318309886183790671537767526745028724  /* 1/pi        */
#define S2_2_PI      0.636619772367581343075535053490057448  /* 2/pi        */
#define S2_TAU       6.283185307179586476925286766559005768  /* tau         */
#define S2_TAU_2     S2_PI                                  /* tau/2       */
#define S2_TAU_4     S2_PI_2                                /* tau/4       */
#define S2_1_TAU     0.159154943091895335768883763372514362  /* 1/tau       */
#define S2_2_TAU     0.318309886183790671537767526745028724  /* 2/tau       */
#define S2_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)  */
#define S2_SQRTTAU   2.506628274631000502415765284811045253  /* sqrt(tau)   */
#define S2_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)     */
#define S2_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)   */

#define S2_Ef         ((float)S2_E)
#define S2_LOG2Ef     ((float)S2_LOG2E)
#define S2_LOG10Ef    ((float)S2_LOG10E)
#define S2_LN2f       ((float)S2_LN2)
#define S2_LN10f      ((float)S2_LN10)
#define S2_PIf        ((float)S2_PI)
#define S2_PI_2f      ((float)S2_PI_2)
#define S2_PI_4f      ((float)S2_PI_4)
#define S2_1_PIf      ((float)S2_1_PI)
#define S2_2_PIf      ((float)S2_2_PI)
#define S2_TAUf       ((float)S2_TAU)
#define S2_TAU_2f     ((float)S2_TAU_2)
#define S2_TAU_4f     ((float)S2_TAU_4)
#define S2_1_TAUf     ((float)S2_1_TAU)
#define S2_2_TAUf     ((float)S2_2_TAU)
#define S2_2_SQRTPIf  ((float)S2_2_SQRTPI)
#define S2_2_SQRTTAUf ((float)S2_SQRTTAU)
#define S2_SQRT2f     ((float)S2_SQRT2)
#define S2_SQRT1_2f   ((float)S2_SQRT1_2)

static inline float s2_rad(float deg)
{
    return deg * S2_PIf;
}

static inline float s2_deg(float rad)
{
    return rad * 180.0f / S2_PIf;
}

#endif
