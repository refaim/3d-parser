#ifndef VIEWER_H
#define VIEWER_H

#include <QGLWidget>
#include "scenemodel.h"
#include <GL/glu.h>

class Viewer : public QGLWidget
{
    Q_OBJECT

    void recursive_render (const aiScene *sc, const aiNode* nd);
    GLuint sceneList;
public:
    SceneModel sceneModel;

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    void loadScene(const std::string &filename);

    explicit Viewer(QWidget *parent = 0);
};

#endif // VIEWER_H
