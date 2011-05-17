#include "viewer.h" //Must be changed to #include "navigator.h"

const float Rotator::ANGLE_STEP = 10.f;

Rotator::Rotator()
{
    showFace();
}

void Rotator::showFace()
{
    angleX = angleY = angleZ = 0.f;
}

void Rotator::setRotateY(EDir aDir)
{
    angleY += (int)aDir * ANGLE_STEP; 
}

void Rotator::setRotateX(EDir aDir)
{
    angleX += (int)aDir * ANGLE_STEP;
}

void Rotator::setRotateZ(EDir aDir)
{
    angleZ += (int)aDir * ANGLE_STEP;
}

void Rotator::Rotate() const
{
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);
}
