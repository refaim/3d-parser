#ifndef VIEWER_H
#define VIEWER_H

#include <string>
#include <QGLWidget>
#include <QMap>
#include "scenemodel.h"
#include <GL/glu.h>

class Viewer : public QGLWidget
{
    Q_OBJECT

    void recursive_render(const aiScene *sc, const aiNode* nd);
    void apply_material(const aiMaterial *mtl);
    void loadTextures();

    void freeTextures();
    GLuint sceneList;
    QMap<std::string, GLuint> textureIds;
public:
    SceneModel sceneModel;

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    void loadScene(const std::string &filename);
    ~Viewer();

    explicit Viewer(QWidget *parent = 0);
};

#endif // VIEWER_H
