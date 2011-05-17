#include "viewer.h"

#include <QMessageBox>
#include <QDebug>
#include <QtCore>
#include <QMap>
#include <QtGui>

Viewer::Viewer(QWidget *parent) : QGLWidget(parent), sceneList(0) {}

void Viewer::initializeGL()
{
// some init functions..
    loadTextures();
    glClearColor(1.f, 1.f, 1.f, 0.f);
    glClearDepth(1.f);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glDepthFunc(GL_LEQUAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    // XXX docs say all polygons are emitted CCW, but tests show that some aren't.
    //if(getenv("MODEL_IS_BROKEN"))
    //    glFrontFace(GL_CW);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
    f[0] = c->r;
    f[1] = c->g;
    f[2] = c->b;
    f[3] = c->a;
}

void set_float4(float f[4], float a, float b, float c, float d)
{
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;
}

void Viewer::apply_material(const aiMaterial *mtl)
{
    float c[4];
    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse, specular, ambient, emission;
    float shininess, strength;
    int two_sided, wireframe;
    unsigned int max, texIndex = 0;

    aiString texPath;
    if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
    {
        //bind texture
        unsigned int texId = textureIds[texPath.data];
        glBindTexture(GL_TEXTURE_2D, texId);
    }

    set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
    if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        color4_to_float4(&diffuse, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
        color4_to_float4(&specular, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

    set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
        color4_to_float4(&ambient, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
        color4_to_float4(&emission, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

    max = 1;
    ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
    max = 1;
    ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
    if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
    else
    {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    }

    max = 1;
    if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
        fill_mode = wireframe ? GL_LINE : GL_FILL;
    else
        fill_mode = GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

    max = 1;
    if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

}

inline void setEnable(GLenum cap, bool cond)
{
    if(cond)
        glEnable(cap);
    else
        glDisable(cap);
}

void Viewer::recursive_render (const aiScene *sc, const aiNode* nd)
{
    aiMatrix4x4 m = nd->mTransformation;

    // update transform
    m.Transpose();
    glPushMatrix();
    glMultMatrixf((float*)&m);

    // draw all meshes assigned to this node
    for (unsigned int n = 0; n < nd->mNumMeshes; ++n)
    {
        const aiMesh* mesh = sceneModel->mMeshes[nd->mMeshes[n]];

        apply_material(sc->mMaterials[mesh->mMaterialIndex]);

        setEnable(GL_LIGHTING, mesh->mNormals != NULL);
        setEnable(GL_COLOR_MATERIAL, mesh->mColors[0] != NULL);

        for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
        {
            const struct aiFace* face = &mesh->mFaces[t];
            GLenum faceMode;

            switch (face->mNumIndices)
            {
                case 1: faceMode = GL_POINTS; break;
                case 2: faceMode = GL_LINES; break;
                case 3: faceMode = GL_TRIANGLES; break;
                default: faceMode = GL_POLYGON; break;
            }

            glBegin(faceMode);

            for (unsigned int i = 0; i < face->mNumIndices; i++)
            {
                int index = face->mIndices[i];
                if (mesh->mColors[0] != NULL)
                    glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                if (mesh->mNormals != NULL)
                {
                    if (mesh->HasTextureCoords(0))
                    {
                        glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
                    }
                    glNormal3fv(&mesh->mNormals[index].x);
                }
                glVertex3fv(&mesh->mVertices[index].x);
            }

            glEnd();
        }
    }

    // draw all children
    for (unsigned int n = 0; n < nd->mNumChildren; ++n)
        recursive_render(sc, nd->mChildren[n]);

    glPopMatrix();
}


void Viewer::paintGL()
{
    float tmp;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!sceneModel.isLoaded())
    {
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

    // rotate it around the y axis
    /*glRotatef(rot.getAngle(),0.f,1.f,0.f);*/
    rot.Rotate();

    {
        using namespace std; // workaround for VC9 strange behaviour
        // scale the whole asset to fit into our view frustum
        tmp = sceneModel.scene_max.x - sceneModel.scene_min.x;
        tmp = max(sceneModel.scene_max.y - sceneModel.scene_min.y, tmp);
        tmp = max(sceneModel.scene_max.z - sceneModel.scene_min.z, tmp);
        tmp = 1.f / tmp;
        glScalef(tmp, tmp, tmp);
    }

    // center the model
    glTranslatef( -sceneModel.scene_center.x, -sceneModel.scene_center.y, -sceneModel.scene_center.z );

    // if the display list has not been made yet, create a new one and
    // fill it with scene contents
    if (sceneList == 0)
    {
        sceneList = glGenLists(1);
        glNewList(sceneList, GL_COMPILE);
        // now begin at the root node of the imported data and traverse
        // the scenegraph by multiplying subsequent local transforms
        // together on GL's matrix stack.
        recursive_render(sceneModel.scene, sceneModel->mRootNode);
        glEndList();
    }

    glCallList(sceneList);

    //do_motion();
}

void Viewer::resizeGL(int w, int h)
{
    if (!h) h = 1;
    const double aspectRatio = (double) w / h, fieldOfView = 45.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, 1.0, 1000.0);  /* last two: Znear and Zfar */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Viewer::loadScene(const std::string &filename)
{
    if (sceneList)
    {
        glDeleteLists(sceneList, 1);
        sceneList = 0;
    }
    sceneModel.loadScene(filename);
    loadTextures();
    // now we need redraw our brand new scene
    updateGL();
}

void Viewer::loadTextures()
{
    if (!sceneModel.isLoaded())
    {
        return;
    }
    if (sceneModel->HasTextures())
    {
        QMessageBox::critical(this, "Feature not implemented", "Can't import embedded textures");
        return;
    }
    freeTextures(); // Makes no harm if there is no textures loaded;
    for (unsigned int m = 0; m < sceneModel->mNumMaterials; ++m)
    {
        int texIndex = 0;
        GLuint texid;
        aiReturn texFound = AI_SUCCESS;
        aiString path;

        while (texFound == AI_SUCCESS)
        {
            texFound = sceneModel->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
            QString texturepath = QDir::toNativeSeparators(QDir::cleanPath(
                QFileInfo(QString::fromStdString(sceneModel.getFilename()))
                    .absoluteDir()
                    .filePath(path.data)
                    .replace('\\', '/') // Example with spider shows that damn .obj contains windows native separators
                ));
            QPixmap px(texturepath);
            texid = textureIds[path.data] = bindTexture(px);

            glBindTexture(GL_TEXTURE_2D, texid);
            // some strange options goes here
            texIndex++;
        }
    }
}

Viewer::~Viewer()
{
    freeTextures();
}

void Viewer::freeTextures()
{
    for (QMap<std::string, GLuint>::const_iterator i = textureIds.begin(); i != textureIds.end(); ++i)
    {
        deleteTexture(i.value());
    }
    textureIds.clear();
}

void Viewer::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key())
    {
        case Qt::Key_Left:
            rot.setRotateY(Rotator::dLeft);
            break;
        case Qt::Key_Right:
            rot.setRotateY(Rotator::dRight);
            break;
        case Qt::Key_Up:
            rot.setRotateX(Rotator::dLeft);
            break;
        case Qt::Key_Down:
            rot.setRotateX(Rotator::dRight);
            break;
        case Qt::Key_PageUp:
            rot.setRotateZ(Rotator::dRight);
            break;
        case Qt::Key_PageDown:
            rot.setRotateZ(Rotator::dLeft);
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            rot.showFace();
            break;
    }
    updateGL();
}
