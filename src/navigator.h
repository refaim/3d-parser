#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <GL/glu.h>

class Navigator
{
private:

public:
	Navigator() {}
};

/**************************************************************/

class Rotator: public Navigator
{
private:
	static const float ANGLE_STEP;

	inline void setCoord(GLfloat aX, GLfloat aY, GLfloat aZ);

	GLfloat angle[3];
public:
	enum EDir
	{
		dLeft = -1,
		dRight = 1
	};
    enum ECoord
    {
        cX = 0,
        cY,
        cZ
    };

	Rotator();
	void setRotation(ECoord aC, EDir aDir);
	void rotate() const;
    void setDefault();
};

/**************************************************************/

class SclTransformer: public Navigator
{
private:
    static const float SCALING_STEP;

    GLfloat val;

public:
    enum EDir
	{
		dPlus = 1,
		dMinus = -1
	};

    SclTransformer();
    void setScaling(EDir aDir);
	void scale() const;
    void setDefault();
};

#endif	// NAVIGATOR_H