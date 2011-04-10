#ifndef VIEWER_H
#define VIEWER_H

#include <QGLWidget>

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    explicit Viewer(QWidget *parent = 0);
};

#endif // VIEWER_H
