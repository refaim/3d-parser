#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>

class Viewer : public QGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void paintGL();
    void resizeGL();

public:
    explicit Viewer(QWidget *parent = 0);
};

#endif // GLVIEWER_H
