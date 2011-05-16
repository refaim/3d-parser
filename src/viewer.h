#ifndef VIEWER_H
#define VIEWER_H

#include <string>
#include <QGLWidget>
#include <QMap>
#include <QKeyEvent>
#include <QSet>
#include "navigator.h"
#include "scenemodel.h"


class Viewer : public QGLWidget
{
    Q_OBJECT

    void recursive_render(const aiScene *sc, const aiNode* nd);
    void apply_material(const aiMaterial *mtl);
    void loadTextures();

    void freeTextures();
    GLuint sceneList;
    QMap<std::string, GLuint> textureIds;

	Rotator rot;
    SclTransformer scl;

public:
    SceneModel sceneModel;

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    void loadScene(const std::string &filename);
    ~Viewer();

	void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    explicit Viewer(QWidget *parent = 0);
};

#endif // VIEWER_H
