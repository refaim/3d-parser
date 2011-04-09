#include "viewer.h"

Viewer::Viewer(QWidget *parent) :
    QGLWidget(parent)
{}

void Viewer::initializeGL()
{
// some init functions..
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(30.0);
    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
}

void Viewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glColor3f(0.80, 0.0, 0.0);
    glVertex2f(-0.4, -0.5);
    glColor3f(0.0, 0.80, 0.0);
    glVertex2f(-0.5, 0.5);
    glColor3f(0.0, 0.0, 0.80);
    glVertex2f(0.45, 0.5);
    glColor3f(0.80, 0.80, 0.0);
    glVertex2f(0.5, -0.5);
    glColor3f(0.80, 0.0, 0.0);
    glVertex2f(1.0, -0.5);
    glEnd();

    glFlush();
}

void Viewer::resizeGL()
{
    paintGL();
}
