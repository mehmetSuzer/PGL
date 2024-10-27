
#ifndef __CORE_H__
#define __CORE_H__

#include <math.h>
#include <stdbool.h>
#include "pglmdef.h"

#define PGLM_E           ((f64)2.71828182845904523536028747135266250)  // e         
#define PGLM_LOG2E       ((f64)1.44269504088896340735992468100189214)  // log2(e)   
#define PGLM_LOG10E      ((f64)0.43429448190325182765112891891660508)  // log10(e)  
#define PGLM_LN2         ((f64)0.69314718055994530941723212145817657)  // loge(2)   
#define PGLM_LN10        ((f64)2.30258509299404568401799145468436421)  // loge(10)  
#define PGLM_PI          ((f64)3.14159265358979323846264338327950288)  // pi        
#define PGLM_PI_2        ((f64)1.57079632679489661923132169163975144)  // pi/2      
#define PGLM_PI_3        ((f64)1.04719755119659774615421446109316763)  // pi/3
#define PGLM_PI_4        ((f64)0.78539816339744830961566084581987572)  // pi/4      
#define PGLM_1_PI        ((f64)0.31830988618379067153776752674502872)  // 1/pi      
#define PGLM_2_PI        ((f64)0.63661977236758134307553505349005745)  // 2/pi      
#define PGLM_3_PI        ((f64)0.95492965855137201461330258023508617)  // 3/pi
#define PGLM_4_PI        ((f64)1.27323954473516268615107010698011490)  // 4/pi
#define PGLM_SQRTPI      ((f64)1.77245385090551602729816748334114518)  // sqrt(pi)
#define PGLM_1_SQRTPI    ((f64)0.56418958354775628694807945156077259)  // 1/sqrt(pi)
#define PGLM_2_SQRTPI    ((f64)1.12837916709551257389615890312154517)  // 2/sqrt(pi)
#define PGLM_3_SQRTPI    ((f64)1.69256875064326886084423835468231776)  // 3/sqrt(pi)
#define PGLM_SQRT2       ((f64)1.41421356237309504880168872420969808)  // sqrt(2)   
#define PGLM_SQRT3       ((f64)1.73205080756887729352744634150587237)  // sqrt(3)
#define PGLM_1_SQRT2     ((f64)0.70710678118654752440084436210484904)  // 1/sqrt(2) 
#define PGLM_1_SQRT3     ((f64)0.57735026918962576450914878050195746)  // 1/sqrt(3)
#define PGLM_SQRT3_2     ((f64)0.86602540378443864676372317075293618)  // sqrt(3)/2
#define PGLM_1_2SQRT3    ((f64)0.28867513459481288225457439025097873)  // 1/(2*sqrt(3))

#define PGLM_Ef          ((f32)PGLM_E)           // e         
#define PGLM_LOG2Ef      ((f32)PGLM_LOG2E)       // log2(e)   
#define PGLM_LOG10Ef     ((f32)PGLM_LOG10E)      // log10(e)  
#define PGLM_LN2f        ((f32)PGLM_LN2)         // loge(2)   
#define PGLM_LN10f       ((f32)PGLM_LN10)        // loge(10)  
#define PGLM_PIf         ((f32)PGLM_PI)          // pi        
#define PGLM_PI_2f       ((f32)PGLM_PI_2)        // pi/2      
#define PGLM_PI_3f       ((f32)PGLM_PI_3)        // pi/3
#define PGLM_PI_4f       ((f32)PGLM_PI_4)        // pi/4      
#define PGLM_1_PIf       ((f32)PGLM_1_PI)        // 1/pi      
#define PGLM_2_PIf       ((f32)PGLM_2_PI)        // 2/pi      
#define PGLM_3_PIf       ((f32)PGLM_3_PI)        // 3/pi
#define PGLM_4_PIf       ((f32)PGLM_4_PI)        // 4/pi
#define PGLM_SQRTPIf     ((f32)PGLM_SQRTPI)      // sqrt(pi)
#define PGLM_1_SQRTPIf   ((f32)PGLM_1_SQRTPI)    // 1/sqrt(pi)
#define PGLM_2_SQRTPIf   ((f32)PGLM_2_SQRTPI)    // 2/sqrt(pi)
#define PGLM_3_SQRTPIf   ((f32)PGLM_3_SQRTPI)    // 3/sqrt(pi)
#define PGLM_SQRT2f      ((f32)PGLM_SQRT2)       // sqrt(2)   
#define PGLM_SQRT3f      ((f32)PGLM_SQRT3)       // sqrt(3)
#define PGLM_1_SQRT2f    ((f32)PGLM_1_SQRT2)     // 1/sqrt(2) 
#define PGLM_1_SQRT3f    ((f32)PGLM_1_SQRT3)     // 1/sqrt(3)
#define PGLM_SQRT3_2f    ((f32)PGLM_SQRT3_2)     // sqrt(3)/2
#define PGLM_1_2SQRT3f   ((f32)PGLM_1_2SQRT3)    // 1/(2*sqrt(3))

#define smaller(x, y)       (((x) < (y)) ? (x) :  (y))
#define greater(x, y)       (((x) > (y)) ? (x) :  (y))
#define abs(x)              (((x) >= 0)  ? (x) : -(x))
#define clamp(x, min, max)  (((x) < (min)) ? (min) : ((x) > (max)) ? (max) : (x))
#define interp(x, y, a)     ((y) + (a) * ((x) - (y)))

#define swap(ptr1, ptr2)    { const typeof(*(ptr1)) temp = *(ptr1); *(ptr1) = *(ptr2); *(ptr2) = temp; }

#endif // __CORE_H__
