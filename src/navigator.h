#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <GL/glu.h>

class Navigator
{
private:

public:
    Navigator() {}
};

class Rotator: public Navigator
{
private:
    static const float ANGLE_STEP;

    inline void setCoord(GLfloat aX, GLfloat aY, GLfloat aZ);

    GLfloat angleX, angleY, angleZ;
public:
    enum EDir
    {
        dLeft = -1,
        dRight = 1
    };

    Rotator();

    void setRotateY(EDir aDir);
    void setRotateX(EDir aDir);
    void setRotateZ(EDir aDir);

    void Rotate() const;
};

#endif  // NAVIGATOR_H
