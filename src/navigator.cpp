#include "viewer.h" //Must be changed to #include "navigator.h"

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
    angle[cX] = angle[cY] = angle[cZ] = 0.f;
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

/**************************************************************/

NavigationEvent::NavigationEvent(QGLWidget *aParent, Rotator *aRot, SclTransformer *aScl):
    widGl(aParent),
    rot(aRot),
    scl(aScl)
{
    connect(&tmr, SIGNAL(timeout()), this, SLOT(repeatEvent()));
    tmr.setInterval(TMR_INTERVAL);
}

bool NavigationEvent::eventFilter(QObject *aObj, QEvent *aEvent)
{
    if (aEvent->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(aEvent);
        switch (keyEvent->key())
        {
            case Qt::Key_Left:
                mapKPRot[(int)Qt::Key_Left] = PairRot(Rotator::cY, Rotator::dLeft);  break;
            case Qt::Key_Right:
                mapKPRot[(int)Qt::Key_Right] = PairRot(Rotator::cY, Rotator::dRight); break;
            case Qt::Key_Up:
                mapKPRot[(int)Qt::Key_Up] = PairRot(Rotator::cX, Rotator::dLeft);  break;
            case Qt::Key_Down:
                mapKPRot[(int)Qt::Key_Down] = PairRot(Rotator::cX, Rotator::dRight); break;
            case Qt::Key_PageUp:
                mapKPRot[(int)Qt::Key_PageUp] = PairRot(Rotator::cZ, Rotator::dRight); break;
            case Qt::Key_PageDown:
                mapKPRot[(int)Qt::Key_PageDown] = PairRot(Rotator::cZ, Rotator::dLeft);  break;

            case Qt::Key_Minus: 
                mapKPScl[Qt::Key_Minus] = SclTransformer::dMinus; break;
            case Qt::Key_Plus:  
                mapKPScl[Qt::Key_Plus] = SclTransformer::dPlus; break;

            default:                
                return false;
        }
        if (!tmr.isActive()) tmr.start();
        return true;
    }
    else if (aEvent->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(aEvent);
        MapKPRot::iterator itRot = mapKPRot.find((int)keyEvent->key());
        MapKPScl::iterator itScl = mapKPScl.find((int)keyEvent->key());
        if (itRot != mapKPRot.end()) mapKPRot.erase(itRot);
        if (itScl != mapKPScl.end()) mapKPScl.erase(itScl);
        return true;
    }
    return QObject::eventFilter(aObj, aEvent);
}

void NavigationEvent::repeatEvent()
{
    if (mapKPRot.isEmpty() && mapKPScl.isEmpty())
    {
        tmr.stop();
        return;
    }
    for (MapKPRot::const_iterator it = mapKPRot.begin(); it != mapKPRot.end(); ++it)
        rot->setRotation(it.value().first, it.value().second);

    for (MapKPScl::const_iterator it = mapKPScl.begin(); it != mapKPScl.end(); ++it)
        scl->setScaling(it.value());

    widGl->updateGL();
}
