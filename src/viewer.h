#ifndef VIEWER_H
#define VIEWER_H

#include <QGLWidget>
#include "aiScene.h"

class Viewer : public QGLWidget
{
    Q_OBJECT

    const aiScene *scene;
public:
    void setScene(const aiScene *aScene);

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    explicit Viewer(QWidget *parent = 0);
    ~Viewer();
};

#endif // VIEWER_H
