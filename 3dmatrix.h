// 3DMatrix.h: Transformation matrix class.
// Template
// Copyright (C) Y2k6 Yurij Borisoff & Sergey Melman
// Ver 1.01
// Last modified 16.05.2006 (Y) - fixed obsolete style
// Last modified 16.05.2006 (S) - добавлен UnProject
// Last modified 12.06.2006 (S) - добавлен тип C3DMatrixF и получение углов из матрицы вращения
// Last modified 20.06.2006 (S) - union
// Last modified  [1/30/2008 SERGEY] - Добавлены LoadProjectGL и LoadUnProjectGL
// Last modified  [1/31/2008 SERGEY] - Добавлен #define CREATEUNPROJECTGL (MTR)
// Last modified  [5/29/2008 SERGEY] - От скуки переделал вычисление обратной матрицы убрал     void AddRows(int i, int j); void MulRows(int i, T d); void SubRowsMuld( int i1, int i2, T d);
// Last modified  [6/24/2009 SERGEY] - перенес переменные класса вначало. так правиьлнее :)
// Last modified  [6/24/2009 SERGEY] - ВНИМАНИЕ !!! ПЕРЕДЕЛАЛ LoadInverseTransform создание матрицы преобразования из векторов от скуки, возможно не будет работать.
// Last modified  [6/24/2009 SERGEY] - ВНИМАНИЕ !!! ПЕРЕДЕЛАЛ operator []
// Last modified  [12/8/2010 SERG] - удалил конструктор копий и оператор = ибо они есть и по умолчанию (мне нужно было для union)
//////////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>

#include "3dvector.h"
#include "usefulmath.h"


template <class T>
class C3DMatrix_templ
{
private:
    union // Last modified 20.06.2006 (S) - union
    {
        T data[16];
        T m_H[4][4];
    };
public:
    // Конструктор по умолчанию (загружает единичную матрицу)
    C3DMatrix_templ();
/*
    // MELMAN
    // Конструктор копий
    C3DMatrix_templ(const C3DMatrix_templ<T> &matrix);
    // Присваивание
    C3DMatrix_templ<T> &operator =(const C3DMatrix_templ<T> &matrix);

*/
    // Доступ к массиву матрицы
    T *operator *(void);
    // Доступ к массиву матрицы
    const T *operator *(void) const;

    // Оператор []. Доступ к строке
    T *operator [](int i);
    const T *operator [](int i) const;

    // Умножение с присвоением (умножение слева !!!)
    const C3DMatrix_templ<T> &
        operator *= (const C3DMatrix_templ<T> &matrix);

    // Обнулить матрицу
    void Zero(void);
    // Загрузить единичную
    void LoadIdentity(void);

    // Транспонировать матрицу
    void Transpose();
    // Получить обратную матрицу
    C3DMatrix_templ Invert(void);
    // Загрузить обратную к данной
    void LoadInvert(void);
    // Загрузить матрицу перемещения (перемещение задается вектором)
    void LoadTranslate(const C3DVector_templ<T> &vec);
    // Переместить матрицу (перемещение задается вектором) // Умножить слева
    void Translate(const C3DVector_templ<T> &vec);
    // Загрузить матрицу вращения вокруг трех углов (X, Y, Z)
    void LoadRotate(T angleX, T angleY, T angleZ);
    // Вращать матрицу
    void Rotate(T angleX, T angleY, T angleZ);
    // Загрузить матрицу вращения вокруг вектора (угол в градусах)
    void LoadRotate(T angle, const C3DVector_templ<T> &vec);
    // Вращать матрицу
    void Rotate(T angle, const C3DVector_templ<T> &vec);
    // Загрузить матрицу масштабирования
    void LoadScale(const C3DVector_templ<T> &vec);
    // Масштабировать матрицу
    void Scale(const C3DVector_templ<T> &vec);
    // Установить матрицу преобразования произвольного базиса в единичный
    void LoadTransform(const C3DVector_templ<T> &e1, const C3DVector_templ<T> &e2, const C3DVector_templ<T> &e3, const C3DVector_templ<T> &o);
    // Установить матрицу преобразования единичного базиса в произвольный
    void LoadInverseTransform(const C3DVector_templ<T> &e1, const C3DVector_templ<T> &e2, const C3DVector_templ<T> &e3, const C3DVector_templ<T> &o);
    // Получить четвертую координату (w) от умножения матрицы на вектор
    T W(const C3DVector_templ<T> &vector) const;


    // Last modified 16.05.2006 (S)
    void LoadUnProject(
        const C3DMatrix_templ<T> & GL_proj,
        const C3DMatrix_templ<T> & GL_model,
        int * viewport,
        double bias = 0,
        double scale = 1,
        double depth_range0 = 0,
        double depth_range1 = 1 );

    // Last modified  [1/30/2008 SERGEY]
    // можно пользоваться #define CREATEPROJECTGL (MTR)
    // получение проекционной матрицы GL
    void LoadProjectGL (
        C3DMatrix_templ<T> cProjMtr,
        C3DMatrix_templ<T> cModelMtr,
        int piViewport[4],
        double dBias,
        double pdRange[2],
        double dScale
        );

    // Last modified 16.05.2006 (S)
    // Last modified  [1/30/2008 SERGEY]
    // получение UN-проекционной матрицы GL
    // можно пользоваться #define CREATEUNPROJECTGL (MTR)
    void LoadUnProjectGL (
        C3DMatrix_templ<T> cProjMtr,
        C3DMatrix_templ<T> cModelMtr,
        int piViewport[4],
        double dBias,
        double pdRange[2],
        double dScale
        );

    // Last modified 12.06.2006 (S)
    // Получение 3х углов из матрицы повотов
    void GetAngles (T & ax, T & ay, T &az) ;


    // Сложение
    template <class T>
        friend C3DMatrix_templ<T> operator + (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2);
    // Вычитание
    template <class T>
        friend C3DMatrix_templ<T> operator - (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2);
    // Умножение на число
    template <class T>
        friend C3DMatrix_templ<T> operator * (const C3DMatrix_templ<T> &matrix, T value);
    // Умножение на вектор
    template <class T>
        friend C3DVector_templ<T> operator * (const C3DMatrix_templ<T> &matrix, const C3DVector_templ<T> &vector);
    // Умножение матриц
    template <class T>
        friend C3DMatrix_templ<T> operator * (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2);

    // Получение квадратной матрицы
    template <class T>
        friend C3DMatrix_templ<T> Square(const C3DVector_templ<T> &vec1, const C3DVector_templ<T> &vec2);


    // Потоковые операции
    //friend std::ostream operator
    // Извлечение из потока
    template <class T> friend std::istream& operator >> (std::istream &stream, C3DMatrix_templ<T> &matrix);
    // Запись в поток
    template <class T> friend std::ostream& operator << (std::ostream &stream, C3DMatrix_templ<T> &matrix);

};

//***************************Typedefs*****************************//
typedef C3DMatrix_templ<double> C3DMatrix;
typedef C3DMatrix_templ<float> C3DMatrixF;
//************************Implemenatation************************//

template <class T> C3DMatrix_templ<T>::C3DMatrix_templ()
{
    LoadIdentity();
}
/*
// MELMAN
// Конструктор копий
template <class T> C3DMatrix_templ<T>::C3DMatrix_templ(const C3DMatrix_templ<T> &matrix)
{
    (*this) = matrix;;
}

// Присваивание
template <class T> C3DMatrix_templ<T> &C3DMatrix_templ<T>::operator =(const C3DMatrix_templ<T> &matrix)
{
    //data[0] = matrix.data[0]; data[1] = matrix.data[1]; data[2] = matrix.data[2]; data[3] = matrix.data[3];
    //data[4] = matrix.data[4]; data[5] = matrix.data[5]; data[6] = matrix.data[6]; data[7] = matrix.data[7];
    //data[8] = matrix.data[8]; data[9] = matrix.data[9]; data[10] = matrix.data[10]; data[11] = matrix.data[11];
    //data[12] = matrix.data[12]; data[13] = matrix.data[13]; data[14] = matrix.data[14]; data[15] = matrix.data[15];
    memcpy( data, matrix.data, sizeof ( data ) );
    return (*this);
}

*/


// Доступ к массиву матрицы
template <class T> inline T *C3DMatrix_templ<T>::operator *(void)
{
    return data;
}

// Доступ к массиву матрицы
template <class T> inline const T *C3DMatrix_templ<T>::operator *(void) const
{
    return data;
}

// Оператор []. Доступ к строке
template <class T> inline T *C3DMatrix_templ<T>::operator [](int i)
{
    //return data + i * 4;
    return m_H[i];
}

template <class T> inline const T *C3DMatrix_templ<T>::operator [](int i) const
{
//  return data + i * 4;
    return m_H[i];
}


// Обнулить матрицу
template <class T> inline void C3DMatrix_templ<T>::Zero(void)
{
    data[0] = data[1] = data[2] = data[3] =
    data[4] = data[5] = data[6] = data[7] =
    data[8] = data[9] = data[10] = data[11] =
    data[12] = data[13] = data[14] = data[15] = T(0);
}

// Загрузить единичную
template <class T> inline void C3DMatrix_templ<T>::LoadIdentity(void)
{
    Zero();
    data[0] = data[5] = data[10] = data[15] = T(1.0);
}

// Транспонировать матрицу
template <class T> inline void C3DMatrix_templ<T>::Transpose()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < i; j++)
        {
            SWAP(data[i * 4 + j], data[j * 4 + i]);
        }
    }
}
// Загрузить матрицу перемещения (перемещение задается вектором)
template <class T> void C3DMatrix_templ<T>::LoadTranslate(const C3DVector_templ<T> &vec)
{
    LoadIdentity();
    data[3] = vec[X];
    data[7] = vec[Y];
    data[11] = vec[Z];
}

// Переместить матрицу (перемещение задается вектором)
template <class T> void C3DMatrix_templ<T>::Translate(const C3DVector_templ<T> &vec)
{
    C3DMatrix_templ<T> matr;
    matr.LoadTranslate(vec);
    (*this) *= matr;
}

// Загрузить матрицу вращения вокруг трех углов (X, Y, Z)
template <class T> void C3DMatrix_templ<T>::LoadRotate(T angleX, T angleY, T angleZ)
{
    this->LoadRotate(angleZ, C3DVector(0, 0, 1));
    this->Rotate(angleY, C3DVector(0, 1, 0));
    this->Rotate(angleX, C3DVector(1, 0, 0));
}

// Вращать матрицу
template <class T> void C3DMatrix_templ<T>::Rotate(T angleX, T angleY, T angleZ)
{
    C3DMatrix_templ<T> matr;
    matr.LoadRotate(angleX, angleY, angleZ);
    (*this) *= matr;
}

// Загрузить матрицу вращения вокруг вектора (угол в градусах)
template <class T> void C3DMatrix_templ<T>::LoadRotate(T angle, const C3DVector_templ<T> &vec)
{
    C3DVector_templ<T> vec_(vec);
    vec_.Normalize();

    C3DMatrix_templ<T> sMatrix;
    C3DMatrix_templ<T> uMatrix = Square(vec_, vec_);
    C3DMatrix_templ<T> uMatrix_ = (sMatrix - uMatrix) *  (T)cos(DEG2RAD(angle));
    sMatrix.Zero();
    sMatrix.data[1] = -vec_[Z];
    sMatrix.data[2] = vec_[Y];
    sMatrix.data[4] = vec_[Z];
    sMatrix.data[6] = -vec_[X];
    sMatrix.data[8] = -vec_[Y];
    sMatrix.data[9] = vec_[X];

    sMatrix = sMatrix * (T)sin(DEG2RAD(angle));

    (*this) = uMatrix + uMatrix_ + sMatrix;
    this->data[15] = T(1.0);
}

// Вращать матрицу
template <class T> void C3DMatrix_templ<T>::Rotate(T angle, const C3DVector_templ<T> &vec)
{
    C3DMatrix_templ<T> matr;
    matr.LoadRotate(angle, vec);
    (*this) *= matr;
}


// Загрузить матрицу масштабирования
template <class T> void C3DMatrix_templ<T>::LoadScale(const C3DVector_templ<T> &vec)
{
    LoadIdentity();
    data[0] = vec[X];
    data[5] = vec[Y];
    data[10] = vec[Z];
}

// Масштабировать матрицу
template <class T> void C3DMatrix_templ<T>::Scale(const C3DVector_templ<T> &vec)
{
    C3DMatrix_templ<T> matr;
    matr.LoadScale(vec);
    (*this) *= matr;
}

// Установить матрицу преобразования произвольного базиса в единичный
// Modified 23.03.2006 (Yurij Borisoff)
// Bugfix
template <class T> void C3DMatrix_templ<T>::LoadTransform(const C3DVector_templ<T> &e1, const C3DVector_templ<T> &e2, const C3DVector_templ<T> &e3, const C3DVector_templ<T> &o)
{
    LoadInverseTransform(e1, e2, e3, o);
    LoadInvert();
}
// Установить матрицу преобразования единичного базиса в произвольный
template <class T> void C3DMatrix_templ<T>::LoadInverseTransform(const C3DVector_templ<T> &e1, const C3DVector_templ<T> &e2, const C3DVector_templ<T> &e3, const C3DVector_templ<T> &o)
{
    m_H[0][0] = e1[X];
    m_H[0][1] = e2[X];
    m_H[0][2] = e3[X];
    m_H[0][3] = o[X];

    m_H[1][0] = e1[Y];
    m_H[1][1] = e2[Y];
    m_H[1][2] = e3[Y];
    m_H[1][3] = o[Y];

    m_H[2][0] = e1[Z];
    m_H[2][1] = e2[Z];
    m_H[2][2] = e3[Z];
    m_H[2][3] = o[Z];

    m_H[3][0] = T(0);
    m_H[3][1] = T(0);
    m_H[3][2] = T(0);
    m_H[3][3] = T(1.0);


//  data[0 * 4 + 0] = e1[X];
//  data[0 * 4 + 1] = e2[X];
//  data[0 * 4 + 2] = e3[X];
//  data[0 * 4 + 3] = o[X];
//  data[1 * 4 + 0] = e1[Y];
//  data[1 * 4 + 1] = e2[Y];
//  data[1 * 4 + 2] = e3[Y];
//  data[1 * 4 + 3] = o[Y];
//  data[2 * 4 + 0] = e1[Z];
//  data[2 * 4 + 1] = e2[Z];
//  data[2 * 4 + 2] = e3[Z];
//  data[2 * 4 + 3] = o[Z];
//  data[3 * 4 + 0] = T(0);
//  data[3 * 4 + 1] = T(0);
//  data[3 * 4 + 2] = T(0);
//  data[3 * 4 + 3] = T(1.0);
}

// Сложение
template <class T> C3DMatrix_templ<T> operator + (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2)
{
    C3DMatrix_templ<T> mat;
    for (int i = 0; i < 16; i++)
    {
        mat.data[i] = matrix1.data[i] + matrix2.data[i];
    }
    return mat;
}
// Вычитание
template <class T> C3DMatrix_templ<T> operator - (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2)
{
    C3DMatrix_templ<T> mat;
    for (int i = 0; i < 16; i++)
    {
        mat.data[i] = matrix1.data[i] - matrix2.data[i];
    }
    return mat;
}
// Умножение на число
template <class T> C3DMatrix_templ<T> operator * (const C3DMatrix_templ<T> &matrix, T value)
{
    C3DMatrix_templ<T> mat;
    for (int i = 0; i < 16; i++)
    {
        mat.data[i] = matrix.data[i] * value;
    }
    return mat;
}

// Умножение матрицы на вектор
template <class T> inline C3DVector_templ<T> operator * (const C3DMatrix_templ<T> &matrix, const C3DVector_templ<T> &vector)
{
    return C3DVector_templ<T>(
            matrix.data[0] * vector[X] + matrix.data[1] * vector[Y] + matrix.data[2] * vector[Z] + matrix.data[3],
            matrix.data[4] * vector[X] + matrix.data[5] * vector[Y] + matrix.data[6] * vector[Z] + matrix.data[7],
            matrix.data[8] * vector[X] + matrix.data[9] * vector[Y] + matrix.data[10] * vector[Z] + matrix.data[11]
        );
}

// Получить четвертую координату (w) от умножения матрицы на вектор
template <class T> T C3DMatrix_templ<T>::W(const C3DVector_templ<T> &vector) const
{
    return ( (data[12] * vector[X] + data[13] * vector[Y] + data[14] * vector[Z] + data[15]) );
}

// Умножение матриц
template <class T>  C3DMatrix_templ<T> operator * (const C3DMatrix_templ<T> &matrix1, const C3DMatrix_templ<T> &matrix2)
{
    C3DMatrix_templ<T> matrix;
    matrix.Zero();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                matrix.data[i * 4 + j] += matrix1.data[i * 4 + k] * matrix2.data[k * 4 + j];
            }
        }
    }
    return matrix;
}

// Умножение с присвоением (слева!!!)
template <class T> const C3DMatrix_templ<T> &C3DMatrix_templ<T>::operator *= (const C3DMatrix_templ<T> &matrix)
{
    (*this) = matrix * (*this);
    return *this;
}

template <class T> C3DMatrix_templ<T> Square(const C3DVector_templ<T> &vec1, const C3DVector_templ<T> &vec2)
{
    C3DMatrix_templ<T> matrix;
    matrix.data[0] = vec1[X] * vec2[X];
    matrix.data[1] = vec1[X] * vec2[Y];
    matrix.data[2] = vec1[X] * vec2[Z];
    matrix.data[4] = vec1[Y] * vec2[X];
    matrix.data[5] = vec1[Y] * vec2[Y];
    matrix.data[6] = vec1[Y] * vec2[Z];
    matrix.data[8] = vec1[Z] * vec2[X];
    matrix.data[9] = vec1[Z] * vec2[Y];
    matrix.data[10] = vec1[Z] * vec2[Z];
    matrix.data[15] = 0;

    return matrix;
}

// Получить обратную матрицу
// Last modified  [5/29/2008 SERGEY]
template <class T> C3DMatrix_templ<T> C3DMatrix_templ<T>::Invert(void)
{
    const T _EPS_ = (T)1e-4; // малая.
    C3DMatrix_templ<T> I, H(*this);// загружаем единичную и оригинал
    for ( int j = 0; j < 4; j++ )
    {
        T d = H[j][j];
        if (-_EPS_ < d && d < _EPS_) // d = ~0.0 - если элемент ~ 0 то нужно сложением строк от этого избавиться
        {
            int k, kMax = j;
            for (k = j + 1; k < 4 ; k++) // найти строку с максимальным по модулю элементом
            {
                if ( abs (H[k][j]) > abs (H[kMax][j]) )
                {
                    kMax = k;
                }
            }
            d = H[kMax][j];
            if (-_EPS_ < d && d < _EPS_) // d == ~0.0 - сингулярность
            {
                return C3DMatrix_templ<T>();  // сингулярная матрица - возвращаем единичную
            }
            else // d != ~0.0
            {
                //сложить две строки j и k для обеих матрицх
                H[j][0] += H[kMax][0];
                H[j][1] += H[kMax][1];
                H[j][2] += H[kMax][2];
                H[j][3] += H[kMax][3];
                I[j][0] += I[kMax][0];
                I[j][1] += I[kMax][1];
                I[j][2] += I[kMax][2];
                I[j][3] += I[kMax][3];
            }
        }
        // умножить j - ю строку на d для обеих матриц
        H[j][0] /= d;
        H[j][1] /= d;
        H[j][2] /= d;
        H[j][3] /= d;
        I[j][0] /= d;
        I[j][1] /= d;
        I[j][2] /= d;
        I[j][3] /= d;
        for ( int i = 0; i < 4; i++ )
        {
            if ( i != j )
            {
                d = H[i][j];
                // вычесть из i-ой строки j-ю умноженную на d для обеих матриц
                H[i][0] -= H[j][0] * d;
                H[i][1] -= H[j][1] * d;
                H[i][2] -= H[j][2] * d;
                H[i][3] -= H[j][3] * d;
                I[i][0] -= I[j][0] * d;
                I[i][1] -= I[j][1] * d;
                I[i][2] -= I[j][2] * d;
                I[i][3] -= I[j][3] * d;
            }
        }
    }
    return I;
}

// Загрузить обратную к данной
template <class T> void C3DMatrix_templ<T>::LoadInvert(void)
{
    (*this) = Invert();
}

template <class T> std::ostream& operator << (std::ostream &stream, C3DMatrix_templ<T> &matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            stream << matrix.data[i * 4 + j] << ' ';
        }
        stream << '\n';
    }
    return stream;
}

template <class T> std::istream& operator >> (std::istream &stream, C3DMatrix_templ<T> &matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            stream >> matrix.data[i * 4 + j];
        }
    }
    return stream;
}

// Last modified 16.05.2006 (S)
template <class T> void C3DMatrix_templ<T>::LoadUnProject(
    const C3DMatrix_templ<T> & GL_proj,
    const C3DMatrix_templ<T> & GL_model,
    int *viewport,
    double bias = 0,
    double scale = 1,
    double depth_range0 = 0,
    double depth_range1 = 1 )
{
    C3DMatrix proj(GL_proj), model(GL_model);
    proj.Transpose(); // транспонирум т.к. GL дает транспонированные матрицы
    model.Transpose();
    C3DMatrix UnScr; // матрица: обратные экранные преобразования.

    UnScr.LoadTranslate(
        C3DVector(
        -1.0 - viewport[ 0 ],
        -1.0 - viewport[ 1 ],
        -( depth_range1 + depth_range0 ) / ( depth_range1 - depth_range0 ) - 2.0 * bias / ( scale * ( depth_range1 - depth_range0 ) ) )
        );

    UnScr.Scale(
        C3DVector(
        2.0 / ( viewport[ 2 ] - viewport[ 0 ] ),
        2.0 / ( viewport[ 3 ] - viewport[ 1 ] ),
        2.0 / ( scale * ( depth_range1 - depth_range0 ) ) ) );

    (*this) = ( ( proj * model ).Invert() * UnScr ); // конечная матрица преобразования:
}
// Last modified 12.06.2006 (S)
// Получение 3х углов из матрицы повотов
template <class T> void C3DMatrix_templ<T>::GetAngles( T & ax, T & ay, T &az )
{
    ax = (T)RAD2DEG( atan2( -m_H[1][2], m_H[2][2] ) );
    ay = (T)RAD2DEG( asin ( m_H[0][2] ) );
    az = (T)RAD2DEG( atan2( -m_H[0][1], m_H[0][0] ) );
}

// Last modified  [1/30/2008 SERGEY]
#define C3DMATRIX_CREATE_PROJECT_GL(MTR) \
    int piViewport[4];\
    glGetIntegerv (GL_VIEWPORT, piViewport);\
    double dBias;\
    glGetDoublev (GL_DEPTH_BIAS, &dBias);\
    double pdRange[2];\
    glGetDoublev (GL_DEPTH_RANGE, pdRange);\
    double dScale;\
    glGetDoublev (GL_DEPTH_SCALE, &dScale);\
    C3DMatrix cProjMtr, cModelMtr;\
    glGetDoublev( GL_PROJECTION_MATRIX, *cProjMtr );\
    glGetDoublev( GL_MODELVIEW_MATRIX, *cModelMtr );\
    MTR.LoadProjectGL (cProjMtr, cModelMtr, piViewport, dBias, pdRange, dScale);\

// Last modified  [1/30/2008 SERGEY]
#define C3DMATRIX_CREATE_UNPROJECT_GL(MTR) C3DMATRIX_CREATE_PROJECT_GL(MTR); MTR.LoadInvert ();
// Last modified  [1/30/2008 SERGEY]
template <class T> void C3DMatrix_templ<T>::LoadProjectGL (
    C3DMatrix_templ<T> cProjMtr,
    C3DMatrix_templ<T> cModelMtr,
    int piViewport[4],
    double dBias,
    double pdRange[2],
    double dScale
    )
{
    cProjMtr.Transpose ();
    cModelMtr.Transpose ();
    C3DMatrix cScaleMtr, cTrMtr;
    cScaleMtr.LoadScale (C3DVector ((piViewport[2] - piViewport[0])/2.0, (piViewport[3] - piViewport[1])/2.0, (pdRange[1] - pdRange[0])*dScale/2.0));
    cTrMtr.LoadTranslate (C3DVector ((piViewport[2] + piViewport[0])/2.0, (piViewport[3] + piViewport[1])/2.0, dBias + (pdRange[1] + pdRange[0])/2.0));
    (*this) = cTrMtr * cScaleMtr * cProjMtr * cModelMtr;
}
// Last modified  [1/30/2008 SERGEY]
template <class T> void C3DMatrix_templ<T>::LoadUnProjectGL (
    C3DMatrix_templ<T> cProjMtr,
    C3DMatrix_templ<T> cModelMtr,
    int piViewport[4],
    double dBias,
    double pdRange[2],
    double dScale
    )
{
    LoadProjectGL (cProjMtr, cModelMtr, piViewport, dBias, pdRange, dScale);
    LoadInvert ();
}
