//  [12/23/2009 SERGEY] - revision

#pragma once

#include <vector>
#include <string>

#include "3dmatrix.h"
#include "3dvector.h"

//////////////////////////////////////////////////////////////////////////

#define DRAW_MATERIAL_BIT                   (1<<0)
#define DRAW_FACES_BIT                      (1<<1)
#define DRAW_NORMALS_BIT                    (1<<2)
#define DRAW_WIREFRAME_BIT                  (1<<3)
#define DRAW_TEXTURE_DIFFUSE_1_BIT          (1<<4)
#define DRAW_TEXTURE_REFLECTION_BIT         (1<<5)
#define DRAW_ENABLE_LIGHT                   (1<<6)

#define DRAW_DEFAULT_BITS   (DRAW_MATERIAL_BIT|DRAW_FACES_BIT|DRAW_TEXTURE_DIFFUSE_1_BIT|DRAW_TEXTURE_REFLECTION_BIT|DRAW_ENABLE_LIGHT)

struct fcolors
{
    float r;   /* RGB Color components */
    float g;
    float b;
    float a;
};

struct bitmaps
{
    bool is_loaded;
    std::string name;  /* Bitmap file name */
};          /* Bit map definition */


struct mapsets
{
    bitmaps map; /* The map settings */
};

struct S3DMaterial
{
    std::string name;      /* Name */
    fcolors ambient;     /* Ambient light color */
    fcolors diffuse;     /* Diffuse light color */
    fcolors specular;    /* Specular light color */
    fcolors emission;    /* Specular light color */
    float shininess;    /* Shininess factor */
    float shinstrength; /* Shininess strength */
    float selfillumpct;     /* Self illumination percentage */
    mapsets texture;     /* Texture map settings */
    mapsets reflect;    /* Reflection map settings */
};

struct textverts
{
    float u, v;
};

struct SFace
{
    unsigned short int V1, V2, V3;
};

struct SSingleMesh
{
    std::string matname;     /*имя материала для этой меши*/
    int MatIndex;           /*локальный номер материала этой меши*/
    std::vector<SFace> facearray;     /*Список фейсов этой меши*/
};

struct S3DLocalCS
{
    C3DMatrixF H_to_zero, H_from_zero, H;
    C3DVectorF pivot;
};

struct S3DObject
{
    std::string name;       /* Object name */
    std::string parent_name;
    int parent_index;
    std::vector<C3DVectorF> vertexarray;  /* List of vertices */
    std::vector<C3DVectorF> normalarray;  /* List of vertices */
    std::vector<textverts> textarray;
    S3DLocalCS localCS;
    std::vector<SSingleMesh> meshes;     /* List of faces */
};

typedef std::vector<S3DMaterial> VMaterials;
typedef std::vector<S3DObject> VObjects;

struct SObjTree
{
    int obj_index;
    std::vector<SObjTree> children;
};

class C3DRender
{
public:
    std::string name;
    VMaterials m_materials;
    VObjects m_objects;
    SObjTree m_tree;
    float MyDRAW_NORMALS_MUL;
    bool is_protected;
public:
    C3DRender ();
    ~C3DRender ();
public:
    // не const
    void Clear ();
    void CalcNormals ();
    void SetNormalsLen (float len) {MyDRAW_NORMALS_MUL = len;}
    C3DMatrixF & GetObjectMatrix(int i);
    C3DMatrixF & GetObjectMatrix(const char * cpcName);
    void Protect () {is_protected = true;}
    void UnProtect () {is_protected = false;}
    void LoadTextures ();
    // const
    int GetObjectIndex  (const char * cpcName) const;
    int GetMaterialIndex (const char * cpcName) const;
    bool IsEmpty () const;
    float FindMaxScale () const;
    void Draw (unsigned long type = DRAW_DEFAULT_BITS);
protected:
    // не const
    void FreeTextures ();
    // const
    void SetMaterial (unsigned long  num, unsigned long  type) const;
    void BeginDraw (unsigned long  type) const;
    void DrawTree (unsigned long type, const SObjTree & tree) const;
    void DrawObj (unsigned long type, int obj_index) const;
    void EndDraw () const;
};
