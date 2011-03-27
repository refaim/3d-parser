#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

//  [12/23/2009 SERGEY] - revision
// !!!!!!!!
// Внимание
// !!!!!!!!

// В связи с тем что <glh/glh_extensions.h> реализован через жопу, то нужно чтобы
// строка #define GLH_EXT_SINGLE_FILE была только в одном CPP из всего проекта(солюшина)
// проблема возникла когда <glh/glh_extensions.h> включался в 3DRendr и в GLRoutine
// а эти две lib использовались в одном проекте. Линкер ругался на redefinition
// чтобы реить проблему я в 3DRender закоментыл строку #define GLH_EXT_SINGLE_FILE

// Инициализация расширения
#define GLH_EXT_SINGLE_FILE
#pragma warning (disable:4996)
#include <glh/glh_extensions.h>

#include "3drender.h"
#include "TextureManager.h"


#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")

class CGlExtInit
{
public:
    ~CGlExtInit ()
    {
        if (glActiveTextureARB)
        {
            glh_shutdown_extensions();
        }

    }
    static void Init ()
    {
        if (!glActiveTextureARB)
        {
            // пытаемся инициализировать GlExt
            if ( wglGetCurrentContext () == 0 ) //  [10/3/2006 SERGEY]
            {
                return;
            }
            if (!glh_init_extensions("GL_ARB_multitexture GL_EXT_texture_env_add"))
            {
                return;
            }
        }
    }
};
// при убивании этого объекта деинициализируется GlExtentions
// этот объект убивается при окончании работы всей либы
static CGlExtInit g_GlExtInit;

C3DRender::C3DRender () : MyDRAW_NORMALS_MUL(1), is_protected (false)
{
}

C3DRender::~C3DRender ()
{
    FreeTextures ();
}

void C3DRender::Clear ()
{
    FreeTextures ();
    name = "";
    m_materials.clear ();
    m_objects.clear ();
    m_tree.obj_index = -1;
    m_tree.children.clear ();
    MyDRAW_NORMALS_MUL = 1.0f;
    is_protected = false;
}

// дает индекс объекта по имени, если нет такого, то -1
int C3DRender::GetObjectIndex  (const char * cpcName) const
{
    for (int i = (int)m_objects.size () - 1; i >= 0; --i)
        if (m_objects[i].name == cpcName)
            return i;
    return -1;
}

// дает индекс материала по имени, если нет такого материала то вернуть 0 - дефолтный
int C3DRender::GetMaterialIndex (const char * cpcName) const
{
    for (int i = (int)m_materials.size () - 1; i >= 0; --i)
        if (m_materials[i].name == cpcName)
            return i;
    return 0;
}

// рассчитать нормали для всех объектов
void C3DRender::CalcNormals ()
{
    for (int oind = 0; oind < (int)m_objects.size (); ++oind)
    {
        S3DObject & obj = m_objects[oind];
        std::vector <std::vector <C3DVectorF>> all_norm;
        obj.normalarray.resize (obj.vertexarray.size ());
        all_norm.resize (obj.vertexarray.size ());
        // переберем все треугольники
        for ( int i = 0; i < (int)obj.meshes.size(); ++i )
        {
            for ( int j = 0; j < (int)obj.meshes[i].facearray.size (); ++j)
            {
                SFace & face = obj.meshes[i].facearray[j];
                C3DPointF & V1 = obj.vertexarray[face.V1];
                C3DPointF & V2 = obj.vertexarray[face.V2];
                C3DPointF & V3 = obj.vertexarray[face.V3];
                C3DVectorF vnorm (((V2 - V1) ^ (V3 - V1)).Normalize ());
                all_norm[face.V1].push_back( vnorm );
                all_norm[face.V2].push_back( vnorm );
                all_norm[face.V3].push_back( vnorm );
            }
        }
        // просуммируем и нормализуем все векторы для каждой вершины
        for ( int i = 0; i < (int)obj.normalarray.size (); ++i )
        {
            C3DVectorF fin_norm;
            for ( int j = 0; j < (int)all_norm[ i ].size(); ++j )
            {
                fin_norm += all_norm[i][j];
            }
            fin_norm.Normalize();
            obj.normalarray[i] = fin_norm;
        }
    }
}

bool C3DRender::IsEmpty () const
{
    return m_objects.empty ();
}

float C3DRender::FindMaxScale () const
{
    float fMax = 0;
    for (int i = 0; i < (int)m_objects.size (); ++i)
    {
        const S3DObject & obj = m_objects[i];
        for (int j = 0; j < (int)obj.vertexarray.size (); ++j)
        {
            C3DVectorF v = obj.vertexarray[j];
            float x = fabsf (v.X ());
            if (fMax < x) fMax = x;
            float y = fabsf (v.Y ());
            if (fMax < y) fMax = y;
            float z = fabsf (v.Z ());
            if (fMax < z) fMax = z;
        }
    }
    return fMax;
}

C3DMatrixF & C3DRender::GetObjectMatrix(int i)
{
    static C3DMatrixF m;
    if (i >= 0 && i < (int)m_objects.size ())
    {
        return m_objects[i].localCS.H;
    }
    else
    {
        return m;
    }

}

C3DMatrixF & C3DRender::GetObjectMatrix(const char * cpcName)
{
    return GetObjectMatrix (GetObjectIndex (cpcName));
}

void C3DRender::LoadTextures ()
{
    for (int i = 0; i < (int)m_materials.size (); ++i)
    {
        S3DMaterial & cMBlock = m_materials[i];
        if (!cMBlock.texture.map.is_loaded)
        {
            int rez = TEXMANAGER.LoadTexture (cMBlock.texture.map.name.c_str ());
            cMBlock.texture.map.is_loaded = (rez != -1);
        }
        if (!cMBlock.reflect.map.is_loaded)
        {
            int rez = TEXMANAGER.LoadTexture (cMBlock.reflect.map.name.c_str ());
            cMBlock.reflect.map.is_loaded = (rez != -1);
        }
    }
}

void C3DRender::FreeTextures ()
{
    // убить текстуры
    for (int i = 0; i < (int)m_materials.size (); ++i)
    {
        S3DMaterial & cMBlock = m_materials[i];
        if (cMBlock.texture.map.is_loaded)
        {
            TEXMANAGER.FreeTexture (cMBlock.texture.map.name);
            cMBlock.texture.map.is_loaded = false;
        }
        if (cMBlock.reflect.map.is_loaded)
        {
            TEXMANAGER.FreeTexture (cMBlock.reflect.map.name);
            cMBlock.reflect.map.is_loaded = false;
        }
    }
}

void C3DRender::SetMaterial (DWORD num, DWORD type) const
{
    // получить индекс материала и включить его
    if ((type & DRAW_MATERIAL_BIT) && (num != -1)) // если он нужен
    {
        const S3DMaterial & mblock = m_materials[num];
        glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   &mblock.ambient.r);
        glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   &mblock.diffuse.r);
        glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  &mblock.specular.r);
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,  &mblock.emission.r);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mblock.shininess * 100.0f);

        if ( (type & DRAW_TEXTURE_DIFFUSE_1_BIT) && (mblock.texture.map.is_loaded)) // если установлен бит диффузных текстур1
        {
            glActiveTextureARB (GL_TEXTURE0_ARB);   //Настройки первого тексурного блока
            glEnable (GL_TEXTURE_2D);
            TEXMANAGER.Bind (mblock.texture.map.name);
            glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//Режим умножения
            //glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);//Режим замещения
        }
        else
        {
            glActiveTextureARB (GL_TEXTURE0_ARB);   //Настройки первого тексурного блока
            glDisable (GL_TEXTURE_2D);
        }
        if ((type & DRAW_TEXTURE_REFLECTION_BIT) && (mblock.reflect.map.is_loaded)) // если установлен бит рефлектион текстур
        {
            glActiveTextureARB (GL_TEXTURE1_ARB);   //Настройки второго текстурного блока
            glEnable (GL_TEXTURE_2D);
            TEXMANAGER.Bind (mblock.reflect.map.name);
            //glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//Режим умножения
            glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);//Режим замещения
            glEnable (GL_TEXTURE_GEN_S);
            glEnable (GL_TEXTURE_GEN_T);
            glTexGenf (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGenf (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        }
        else
        {
            glActiveTextureARB (GL_TEXTURE1_ARB);   //Настройки второго текстурного блока
            glDisable (GL_TEXTURE_2D);
        }
    }
    else // иначе включаем просто белый цвет
    {
        glEnable (GL_COLOR_MATERIAL);
        glColor3ub (255, 255, 255);
    }
}

void C3DRender::DrawTree (unsigned long type, const SObjTree & tree) const
{
    glPushMatrix ();
    {
        DrawObj (type, tree.obj_index);
        for (int i = (int)tree.children.size () - 1; i >= 0; --i)
        {
            DrawTree (type, tree.children[i]);
        }
    }
    glPopMatrix ();
}

void C3DRender::DrawObj (unsigned long type, int obj_index) const
{
    const S3DObject & obj = m_objects[obj_index];

    // если матрица не 0, то использовать ее
    // ПыС: в некоторых объектах есть пустрой родитель
    // так вот иногда у этого родителя матрица равна 0
    // а иногда нет. Хуй знает что это значит.
    // я решил проверять на 0 и если она не 0, то использовать.
    if(!(obj.localCS.H_from_zero*C3DVectorF(1,1,1)) > 0.001f)
    {
        // установить матрицу объекта
        glMultMatrixf (*obj.localCS.H_from_zero);
        glMultMatrixf (*obj.localCS.H);
        glMultMatrixf (*obj.localCS.H_to_zero);
    }

    if (obj.vertexarray.empty ())  // и если список вершин пуст то выйти
    {
        return;
    }
    // Устанавливаем указатели массивов
    glVertexPointer (3, GL_FLOAT, 0, &*obj.vertexarray.begin());
    glNormalPointer (GL_FLOAT, 0, &*obj.normalarray.begin());
    if (!obj.textarray.empty () && // если есть текстурные координаты
        (type & DRAW_MATERIAL_BIT)) // и используются материалы
    {
        glEnableClientState (GL_TEXTURE_COORD_ARRAY); // включим массив текстурных координат
        glTexCoordPointer (2, GL_FLOAT, 0, &*obj.textarray.begin());
    }
    else
    {
        glDisableClientState (GL_TEXTURE_COORD_ARRAY);
    }

    // рисуем фэйсы
    for (int i = 0; i < (int)obj.meshes.size (); ++i)
    {
        if ((!obj.meshes[i].facearray.empty ()) && (type & DRAW_FACES_BIT))
        {
            SetMaterial (obj.meshes[i].MatIndex, type);
            // рисуем
            glPushMatrix ();
            {
                glMultMatrixf (*obj.localCS.H_from_zero);
                glTranslatef (-obj.localCS.pivot.X (), -obj.localCS.pivot.Y (), -obj.localCS.pivot.Z ());
                glMultMatrixf (*obj.localCS.H_to_zero);
                glDrawElements (GL_TRIANGLES, (int)obj.meshes[i].facearray.size () * 3,
                    GL_UNSIGNED_SHORT, &*obj.meshes[i].facearray.begin ());
            }
            glPopMatrix ();

        }
    }
    // рисуем нормали
    if (type & DRAW_NORMALS_BIT)
    {
        glPushAttrib (GL_ENABLE_BIT|GL_LIGHTING_BIT);
        {
            glDisable (GL_LIGHTING);
            glActiveTextureARB (GL_TEXTURE0_ARB);   //Отключить - Настройки второго текстурного блока
            glDisable (GL_TEXTURE_2D);
            glActiveTextureARB (GL_TEXTURE1_ARB);   //Отключить - Настройки второго текстурного блока
            glDisable (GL_TEXTURE_2D);
            glEnable (GL_COLOR_MATERIAL);
            glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
            glColor3f (1, 1, 1);
            glBegin (GL_LINES);
            {
                for ( int i = 0; i < (int)obj.vertexarray.size (); ++i )
                {
                    glVertex3fv (*obj.vertexarray[i]);
                    glVertex3fv (*(obj.vertexarray[i] + obj.normalarray[i] * MyDRAW_NORMALS_MUL));
                }
            }
            glEnd ();
        }
        glPopAttrib ();
    }
}

void C3DRender::Draw (unsigned long type)
{
    CGlExtInit::Init ();
    if (IsEmpty () || is_protected)
    {
        return;
    }
    //у нас есть набор мешей и материалов все что нужно сделать это нарисовать
    //все меши включая нужный материал
    if (type & (DRAW_FACES_BIT | DRAW_WIREFRAME_BIT | DRAW_NORMALS_BIT)) // если рисование ваще нужно
    {
        BeginDraw (type);
        DrawTree (type, m_tree);
        EndDraw ();
    }
}

void C3DRender::BeginDraw (DWORD type) const
{
    glPushAttrib (GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT|GL_LIGHTING_BIT|GL_TEXTURE_BIT|GL_POLYGON_BIT);
    glPushClientAttrib (GL_CLIENT_VERTEX_ARRAY_BIT);

    if (type & DRAW_ENABLE_LIGHT)
    {
        glEnable (GL_LIGHTING);
    }
    else
    {
        glDisable (GL_LIGHTING);
    }

    if (type & DRAW_MATERIAL_BIT)
    {
        glDisable (GL_COLOR_MATERIAL);
    }
    else
    {
        glEnable (GL_COLOR_MATERIAL);
    }

    glDisable (GL_CULL_FACE);

    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GREATER, 0);

    // при необходимости устаномим вэйрефрейм
    glPolygonMode(GL_FRONT_AND_BACK, (type & DRAW_WIREFRAME_BIT) ? GL_LINE : GL_FILL);

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
}

void C3DRender::EndDraw () const
{
    glPopClientAttrib ();
    glPopAttrib ();
}
