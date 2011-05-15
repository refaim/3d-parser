#include "viewer.h"
#include <GL/glu.h>

Viewer::Viewer(QWidget *parent) : QGLWidget(parent), sceneList(0) {}

void Viewer::initializeGL()
{
// some init functions..
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    // XXX docs say all polygons are emitted CCW, but tests show that some aren't.
    //if(getenv("MODEL_IS_BROKEN"))
    //    glFrontFace(GL_CW);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

void color4_to_float4(const struct aiColor4D *c, float f[4])
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

void apply_material(const aiMaterial *mtl)
{
    float c[4];

    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse, specular, ambient, emission;
    float shininess, strength;
    int two_sided, wireframe;
    unsigned int max;

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
    if (ret1 == AI_SUCCESS)
    {
        max = 1;
        ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
        if (ret2 == AI_SUCCESS)
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
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
        glDisable(GL_CULL_FACE);
    else
        glEnable(GL_CULL_FACE);
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

        if (mesh->mNormals == NULL)
            glDisable(GL_LIGHTING);
        else
            glEnable(GL_LIGHTING);

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
                if(mesh->mColors[0] != NULL)
                        glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                if(mesh->mNormals != NULL)
                        glNormal3fv(&mesh->mNormals[index].x);
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
    //glRotatef(angle,0.f,1.f,0.f);

    // scale the whole asset to fit into our view frustum
    tmp = sceneModel.scene_max.x - sceneModel.scene_min.x;
    tmp = std::max(sceneModel.scene_max.y - sceneModel.scene_min.y,tmp);
    tmp = std::max(sceneModel.scene_max.z - sceneModel.scene_min.z,tmp);
    tmp = 1.f / tmp;
    glScalef(tmp, tmp, tmp);

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
    const double aspectRatio = (double) w / h, fieldOfView = 45.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, 1.0, 1000.0);  /* last two: Znear and Zfar */
    glViewport(0, 0, w, h);
}

void Viewer::loadScene(const std::string &filename)
{
    if (sceneList)
    {
        glDeleteLists(sceneList, 1);
        sceneList = 0;
    }
    sceneModel.loadScene(filename);
    // now we need redraw our brand new scene
    repaint();
}

