#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QMap>
#include <QPair>

#include <GL/gl.h>

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

/**************************************************************/

typedef QPair<Rotator::ECoord, Rotator::EDir> PairRot;
typedef QMap<int, PairRot> MapKPRot;

typedef QMap<int, SclTransformer::EDir> MapKPScl;

class NavigationEvent: public QObject
{
    Q_OBJECT

private:
    static const int TMR_INTERVAL = 25;

    QGLWidget *widGl;
    Rotator *rot;
    SclTransformer *scl;

    QTimer tmr;

    MapKPRot mapKPRot;
    MapKPScl mapKPScl;

private slots:
    void repeatEvent();
    
protected:
    bool eventFilter(QObject *aObj, QEvent *aEvent);

public:
    NavigationEvent(QGLWidget *aParent, Rotator *aRot, SclTransformer *aScl);
};

#endif  // NAVIGATOR_H
