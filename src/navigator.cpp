#include "viewer.h"	//Must be changed to #include "navigator.h"

/**************************************************************/

const float Rotator::ANGLE_STEP = 10.f;

Rotator::Rotator()
{
    setDefault();
}

void Rotator::setRotation(ECoord aC, EDir aDir)
{
	angle[aC] += (int)aDir * ANGLE_STEP; 
}

void Rotator::rotate() const
{
	glRotatef(angle[cX], 1.f, 0.f, 0.f);
    glRotatef(angle[cY], 0.f, 1.f, 0.f);
    glRotatef(angle[cZ], 0.f, 0.f, 1.f);
}

void Rotator::setDefault()
{
    angle[cX] = angle[cX] = angle[cX] = 0.f;
}

/**************************************************************/

const float SclTransformer::SCALING_STEP = 0.1f;

SclTransformer::SclTransformer():
    val(0.f)
{
    //empty
}

void SclTransformer::setScaling(EDir aDir)
{
    val += (int)aDir * SCALING_STEP; 
}

void SclTransformer::scale() const
{
    glTranslatef(0.f, 0.f, val);
}

void SclTransformer::setDefault()
{
    val = 0.f;
}