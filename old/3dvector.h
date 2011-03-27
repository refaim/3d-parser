// 3DVector.h: interface for the C3DVector_templ class.
// Template
// Copyright (C) Y2k6 Yurij Borisoff & Sergey Melman
// Ver 2.12
// Last modified 31.03.2006
// Last modified 18.05.2006 (Y) - GREAT bugfix
// Last Modifed 12.06.2006 by Sergey GetPitch GetYaw
//  [10/14/2006 Sergey] GetX() GetY() GetZ()
//  [2/13/2009 SERGEY] bool LessThen (const C3DVector_templ<V_TYPE> &vector)
//  [2/16/2009 SERGEY] X() Y() Z()
//  [2/24/2009 SERGEY] убил нах френды
//  [2/24/2009 SERGEY] добавил union
//  [12/15/2010 SERG] добавил операторы == и !=
//////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>

// Modifed 31.03.2006 by Melman Sergey
//#include <fstream>

const int X = 0;
const int Y = 1;
const int Z = 2;


// ВЕКТОР-СТОЛБЕЦ !!!
template <class V_TYPE>
    class C3DVector_templ;

typedef C3DVector_templ<double> C3DVector;
typedef C3DVector               C3DPoint;
typedef C3DVector_templ<float>  C3DVectorF;
typedef C3DVectorF              C3DPointF;

template <class V_TYPE>
    class C3DVector_templ
{
public:
    C3DVector_templ( V_TYPE x = 0, V_TYPE y = 0, V_TYPE z = 0) :
      m_x(x), m_y(y), m_z(z){}

    C3DVector_templ( const C3DVector_templ<V_TYPE> &vector ) :
      m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z) {}

    // Присваивание
    C3DVector_templ<V_TYPE>
        &operator = ( const C3DVector_templ<V_TYPE> &vector );

    // взятие переменной X, Y, Z
    V_TYPE &operator [] (int i);

    // взятие переменной X, Y, Z
    const V_TYPE &
        operator [] (int i) const;

    // RECASTING
    operator C3DVector()
    {
        return C3DVector( (double)m_x, (double)m_y, (double)m_z );
    };
    // RECASTING
    operator C3DVectorF()
    {
        return C3DVectorF( (float)m_x, (float)m_y, (float)m_z );
    };


    // Cложение векторов с присваиванием
    const C3DVector_templ<V_TYPE> &
        operator += ( const C3DVector_templ<V_TYPE> & vector );

    // Вычитание вектора с приствоением
    const C3DVector_templ<V_TYPE> &
        operator -= ( const C3DVector_templ<V_TYPE> & vector );

    // Умножение на число с присваиванием
    const C3DVector_templ<V_TYPE> &
        operator *= ( V_TYPE a );

    // Деление на число с присваиванием
    const C3DVector_templ<V_TYPE> &
        operator /= ( V_TYPE a );

    //// возвращает самого себя
    //const C3DVector_templ<V_TYPE>
    //  & operator + ( void );

    // возвращает обратный вектор
    const C3DVector_templ<V_TYPE>
        operator - ( void ) const;

    // сделать вектор единичным
    const C3DVector_templ<V_TYPE> &
        Normalize();

    // установка вектора
    const C3DVector_templ<V_TYPE> &
        Set( V_TYPE x, V_TYPE y, V_TYPE z );

    // установка вектора и сделать вектор единичным
    const C3DVector_templ<V_TYPE> &
        SetAndNormalize( V_TYPE x, V_TYPE y, V_TYPE z );

    // Доступ к массиву вектора (новое)
    V_TYPE *
        operator *(void);

    // Доступ к массиву вектора (новое)
    const V_TYPE *
        operator *(void) const;

    //// длина вектора
    V_TYPE
        operator !( void ) const;

    //  [12/15/2010 SERG]
    //// оператор равно
    bool
        operator ==( const C3DVector_templ<V_TYPE> & a  ) const
    {
        return (m_x == a.m_x) && (m_y == a.m_y) && (m_z == a.m_z);
    }
    //  [12/15/2010 SERG]
    //// оператор НЕ равно
    bool
        operator !=( const C3DVector_templ<V_TYPE> & a  ) const
    {
        return !(*this == a);
    }


    // дает один из перпендикулярных
    C3DVector_templ<V_TYPE>
        GetAnyPerpendicular() const;


    // Усечение вектора по величинам
    const C3DVector_templ<V_TYPE>
        & Truncate( V_TYPE lower_bound, V_TYPE upper_bound);


    // Modifed 12.06.2006 by Sergey
    V_TYPE GetPitch( void ) const;
    // Modifed 12.06.2006 by Sergey
    V_TYPE GetYaw( void ) const;

    //  [10/14/2006 Sergey]
    const V_TYPE & GetX() const {return m_x;}
    const V_TYPE & GetY() const {return m_y;}
    const V_TYPE & GetZ() const {return m_z;}
    V_TYPE & GetX() {return m_x;}
    V_TYPE & GetY() {return m_y;}
    V_TYPE & GetZ() {return m_z;}
    //  [2/16/2009 SERGEY]
    const V_TYPE & X() const {return m_x;}
    const V_TYPE & Y() const {return m_y;}
    const V_TYPE & Z() const {return m_z;}
    V_TYPE & X() {return m_x;}
    V_TYPE & Y() {return m_y;}
    V_TYPE & Z() {return m_z;}

    //  [2/13/2009 SERGEY]
    bool LessOrEqThen (const C3DVector_templ<V_TYPE> &vector) const {return (m_x <= vector.m_x) && (m_y <= vector.m_y) && (m_z <= vector.m_z);}

private:
    union   //  [2/18/2009 SERGEY]
    {
        struct
        {
            V_TYPE m_x, m_y, m_z;
        };
        V_TYPE m_coords[3];
    };
};

////////////////////////////
// inline - для C3DVector_templ<V_TYPE> //
////////////////////////////

// Присваивание
template <class V_TYPE>
inline C3DVector_templ<V_TYPE> &C3DVector_templ<V_TYPE>::operator = ( const C3DVector_templ<V_TYPE> &vector )
{
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    return *this;
}

template <class V_TYPE>
inline V_TYPE &C3DVector_templ<V_TYPE>::operator [] (int i)
{
    return * (&m_x + i);
}

template <class V_TYPE>
inline const V_TYPE &C3DVector_templ<V_TYPE>::operator [] (int i) const
{
    return * (&m_x + i);
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::operator += ( const C3DVector_templ<V_TYPE> & vector )
{
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::operator -= ( const C3DVector_templ<V_TYPE> & vector )
{
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::operator *= ( V_TYPE a )
{
    m_x *= a;
    m_y *= a;
    m_z *= a;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::operator /= ( V_TYPE a )
{
    m_x /= a;
    m_y /= a;
    m_z /= a;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> C3DVector_templ<V_TYPE>::operator - ( void ) const
{
    return C3DVector_templ<V_TYPE>( -m_x, -m_y, -m_z);
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::Normalize()
{
    *this /= !*this;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::Set( V_TYPE x, V_TYPE y, V_TYPE z )
{
    m_x = x;
    m_y = y;
    m_z = z;
    return *this;
}

template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::SetAndNormalize( V_TYPE x, V_TYPE y, V_TYPE z )
{
    Set( x, y, z);
    Normalize();
    return *this;
}
/////////////////////////////////////////
// inline - для операций над векторами //
/////////////////////////////////////////

// умножение вектора на число
template <class V_TYPE>
inline C3DVector_templ<V_TYPE> operator * ( const C3DVector_templ<V_TYPE> & v, V_TYPE m )
{
    return C3DVector_templ<V_TYPE>( v.X () * m, v.Y () * m, v.Z () * m );
}

// скалярное умножение векторов
template <class V_TYPE>
inline V_TYPE operator * ( const C3DVector_templ<V_TYPE> &vector1, const C3DVector_templ<V_TYPE> &vector2 )
{
    return vector1.X () * vector2.X () + vector1.Y () * vector2.Y () + vector1.Z () * vector2.Z ();
}

// векторное умножение векторов
template <class V_TYPE> inline
C3DVector_templ<V_TYPE> operator ^ ( const C3DVector_templ<V_TYPE> &vector1, const C3DVector_templ<V_TYPE> &vector2 )
{
    return C3DVector_templ<V_TYPE>(
        vector1.Y () * vector2.Z () - vector1.Z () * vector2.Y (),
        vector1.Z () * vector2.X () - vector1.X () * vector2.Z (),
        vector1.X () * vector2.Y () - vector1.Y () * vector2.X () );
}


// сложение векторов
template <class V_TYPE> inline
C3DVector_templ<V_TYPE> operator+ (const C3DVector_templ<V_TYPE> &vector1, const C3DVector_templ<V_TYPE> &vector2)
{
    return C3DVector_templ<V_TYPE>( vector1.X () + vector2.X (), vector1.Y () + vector2.Y (), vector1.Z () + vector2.Z () );
}

// разность векторов
template<class V_TYPE> inline
C3DVector_templ<V_TYPE> operator - ( const C3DVector_templ<V_TYPE> &vector1, const C3DVector_templ<V_TYPE> &vector2 )
{
    return C3DVector_templ<V_TYPE>( vector1.X () - vector2.X (), vector1.Y () - vector2.Y (), vector1.Z () - vector2.Z () );
}

// длина вектора
template <class V_TYPE> inline
V_TYPE C3DVector_templ<V_TYPE>::operator !( void ) const
{
    return (V_TYPE)sqrt( (double)(m_x * m_x + m_y * m_y + m_z * m_z) );
}

template <class V_TYPE> inline
V_TYPE * C3DVector_templ<V_TYPE>::operator *(void)
{
    return m_coords;
}

template <class V_TYPE> inline
const V_TYPE * C3DVector_templ<V_TYPE>::operator *(void) const
{
    return m_coords;
}

// дает один из перпендикулярных
template <class V_TYPE> inline
C3DVector_templ<V_TYPE> C3DVector_templ<V_TYPE>::GetAnyPerpendicular() const
{
#define EPS 0.001
    if ( m_x*m_x + m_y*m_y > EPS )
    {
        return C3DVector_templ<V_TYPE>( m_y, m_x, 0.0 ).Normalize();
    }
    else if ( m_x*m_x + m_z*m_z > EPS )
    {
        return C3DVector_templ<V_TYPE>( m_z, 0.0, m_x );
    }
    else
    {
        return C3DVector_templ<V_TYPE>( 0.0, m_z, m_y );
    }
#undef EPS
}

// Усечение вектора по величинам
template <class V_TYPE>
inline const C3DVector_templ<V_TYPE> & C3DVector_templ<V_TYPE>::Truncate( V_TYPE lower_bound, V_TYPE upper_bound)
{
    V_TYPE period = upper_bound - lower_bound;
    if (m_x < lower_bound)
    {
        m_x += period;
    }

    if (m_x > upper_bound)
    {
        m_x -= period;
    }

    if (m_y < lower_bound)
    {
        m_y += period;
    }
    if (m_y > upper_bound)
    {
        m_y -= period;
    }

    if (m_z < lower_bound)
    {
        m_z += period;
    }
    if (m_z > upper_bound)
    {
        m_z -= period;
    }

    return (*this);
}

/*
// Modifed 18.05.2006 by Yuriy Borisoff
// Bugfix
template <class V_TYPE> std::ostream& operator << (std::ostream &stream, const C3DVector_templ<V_TYPE> &vector)
{
    stream << vector.X () << " " << vector.Y () << " " << vector.Z () << " ";
    return stream;
}
// Modifed 27.03.2006 by Yuriy Borisoff
// Bugfix
template <class V_TYPE> std::istream& operator >> (std::istream &stream, C3DVector_templ<V_TYPE> &vector)
{
    stream >> vector.X () >> vector.Y () >> vector.Z ();
    return stream;
}
*/

// Modifed 12.06.2006 by Sergey
template <class V_TYPE>
inline V_TYPE C3DVector_templ<V_TYPE>::GetPitch( void ) const
{
   return (V_TYPE) RAD2DEG(-atan2( m_y, sqrt( m_x * m_x + m_z * m_z ) ));
}
// Modifed 12.06.2006 by Sergey
template <class V_TYPE>
inline V_TYPE C3DVector_templ<V_TYPE>::GetYaw( void ) const
{
   return (V_TYPE)RAD2DEG(atan2( m_x, m_z ));
}

//  [3/22/2007 SERGEY]
//сравнение по длинам
template<class V_TYPE>
inline bool operator < ( const C3DVector_templ<V_TYPE> &vector1, const C3DVector_templ<V_TYPE> &vector2 )
{
    return ((!vector1) < (!vector2));
}
