//  [10/13/2006 SERGEY] - добавил SRAND()
//  [10/13/2006 SERGEY] - добавил RND() - возвращает случ число [0, 1]

#pragma once
#ifndef __USEFULMATH_H_
#define __USEFULMATH_H_

//  [12/22/2009 SERGEY] сделал SRAND, RND - дефайнами и убил библы
//#include <stdlib.h>
//#include <time.h>
//#include <Windows.h>


const double M_PI  = 3.1415926535897932384626433832795;
const double M_PI2 = 1.5707963267948966192313216916398;
const double D2R   = M_PI / 180.0;
const double R2D   = 180.0 / M_PI;
const double EPS_ = 1e-5;   // Это у нас будет ноль

//  [12/22/2009 SERGEY]
#define SRAND() {srand ((unsigned)time (NULL));}
//  [12/22/2009 SERGEY]
#define RND(f) (f * rand () / RAND_MAX)

inline double DEG2RAD(double deg)
// Перевод градус->радиан
{
    return deg * D2R;
}

//  [3/23/2007 SERGEY]
template<typename T> inline T RAD2DEG(T rad)
// Перевод радиан->градус
{
    return rad * (T)R2D;
}


template<typename T> inline T TRUNCATE(T x, T lower, T upper)
{
    T period = upper - lower;
    T y = x;
    if (x < lower)
    {
        y = x + period;
    }
    if (x > upper)
    {
        y = x - period;
    }
    return y;
}

template<typename T> inline T SQR(T x)
{
    return x * x;
}

template<typename T> inline int SIGN(T x)
{
    return x > 0 ? 1 : -1;
}

#ifndef MIN
template<typename T> inline T MIN(T x, T y)
{
    return x > y ? y : x;
}
#endif

#ifndef MAX
template<typename T> inline T MAX(T x, T y)
{
    return x < y ? y : x;
}
#endif

template<typename T> inline void SWAP(T &x, T &y)
{
    T t_ = x;
    x = y;
    y = t_;
}

#endif //__USEFULMATH_H_
