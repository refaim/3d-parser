//  [12/23/2009 SERGEY] - revision
#include <windows.h>

#include "3ds.h"
#include "3dsftk.h"
#include "3drender.h"

void FindAllRefsToMe (SObjTree & tree, const C3DRender & rend)
{
    tree.children.clear ();
    for (int i = (int)rend.m_objects.size () - 1; i >=0; --i)
    {
        if (tree.obj_index == rend.m_objects[i].parent_index) // найти тех кто на меня ссылается
        {
            SObjTree child;
            child.obj_index = i;
            tree.children.push_back (child);
        }
    }
    for (int i = (int)tree.children.size () - 1; i >=0; --i)
    {
        FindAllRefsToMe (tree.children[i], rend);
    }
}

int Load3DS (const char * cpcName, C3DRender & rend)
{
    file3ds *file= NULL;
    database3ds *db = NULL;
    CLEAR_ERROR;
    file = OpenFile3ds(cpcName, "r");

    if (file == NULL) return __LINE__;
    rend.Clear ();
    rend.name = cpcName;


    InitDatabase3ds(&db);
    if (ftkerr3ds) return __LINE__;
    CreateDatabase3ds(file, db);
    if (ftkerr3ds) return __LINE__;

    rend.Protect (); // закрыть объект ренд

    ulong3ds nbrmat = GetMaterialCount3ds(db);
    if (ftkerr3ds) return __LINE__;
    rend.m_materials.resize (nbrmat+1); // 0 материал будет дефолтный!
    rend.m_materials[0].name = "_SMelm_def";

    rend.m_materials[0].ambient.r = 0;
    rend.m_materials[0].ambient.g = 0;
    rend.m_materials[0].ambient.b = 0;
    rend.m_materials[0].ambient.a = 0;

    rend.m_materials[0].diffuse.r = 1;
    rend.m_materials[0].diffuse.g = 1;
    rend.m_materials[0].diffuse.b = 1;
    rend.m_materials[0].diffuse.a = 1;

    rend.m_materials[0].specular.r = 0;
    rend.m_materials[0].specular.g = 0;
    rend.m_materials[0].specular.b = 0;
    rend.m_materials[0].specular.a = 0;

    rend.m_materials[0].emission.r = 0;
    rend.m_materials[0].emission.g = 0;
    rend.m_materials[0].emission.b = 0;

    rend.m_materials[0].shininess = 0;

    rend.m_materials[0].shinstrength = 0;

    rend.m_materials[0].selfillumpct = 0;

    rend.m_materials[0].texture.map.is_loaded = false;
    rend.m_materials[0].texture.map.name = "";

    rend.m_materials[0].reflect.map.is_loaded = false;
    rend.m_materials[0].reflect.map.name = "";

    for (ulong3ds i = 0; i < nbrmat; ++i)
    {
        material3ds *mat=NULL;
        GetMaterialByIndex3ds(db, i, &mat);
        if (ftkerr3ds) return __LINE__;
        S3DMaterial &_mat = rend.m_materials[i+1];

        _mat.name = mat->name;

        _mat.ambient.r = mat->ambient.r;
        _mat.ambient.g = mat->ambient.g;
        _mat.ambient.b = mat->ambient.b;
        _mat.ambient.a = 1;

        _mat.diffuse.r = mat->diffuse.r;
        _mat.diffuse.g = mat->diffuse.g;
        _mat.diffuse.b = mat->diffuse.b;
        _mat.diffuse.a = 1;

        _mat.specular.r = mat->specular.r * mat->shinstrength;
        _mat.specular.g = mat->specular.g * mat->shinstrength;
        _mat.specular.b = mat->specular.b * mat->shinstrength;
        _mat.specular.a = 1;

        _mat.shininess = mat->shininess;

        _mat.shinstrength = mat->shinstrength;

        _mat.selfillumpct = mat->selfillumpct;

        _mat.emission.r = 0.01f * _mat.selfillumpct;
        _mat.emission.g = 0.01f * _mat.selfillumpct;
        _mat.emission.b = 0.01f * _mat.selfillumpct;
        _mat.emission.a = 1;

        _mat.texture.map.is_loaded = false;
        _mat.texture.map.name = mat->texture.map.name;

        _mat.reflect.map.is_loaded = false;
        _mat.reflect.map.name = mat->reflect.map.name;
        ReleaseMaterial3ds(&mat);
    }

    ulong3ds nbrmesh = GetMeshCount3ds(db);
    if (ftkerr3ds) return __LINE__;
    rend.m_objects.resize (nbrmesh+1);
    // нужен объект типа ROOT нулевой
    rend.m_objects[0].name = "";
    rend.m_objects[0].parent_name = "__NO__PARENT_MELMAN__";
    rend.m_objects[0].parent_index = -1;
    for (ulong3ds i = 0; i < nbrmesh; ++i)
    {
        mesh3ds *mesh=NULL;
        GetMeshByIndex3ds(db, i, &mesh);
        if (ftkerr3ds) return __LINE__;
        S3DObject & _obj = rend.m_objects[i+1];
        // скопировать имя
        _obj.name = mesh->name;
        _obj.parent_index = 0;
        // скопирвоать вершины
        _obj.vertexarray.resize (mesh->nvertices);
        for (unsigned int k = 0; k < mesh->nvertices; ++k)
        {
            _obj.vertexarray[k].Set (mesh->vertexarray[k].x, mesh->vertexarray[k].y, mesh->vertexarray[k].z);
        }
        // скопировать текстурные координаты
        _obj.textarray.resize (mesh->ntextverts);
        for (unsigned int k = 0; k < mesh->ntextverts; ++k)
        {
            _obj.textarray[k].u = mesh->textarray[k].u;
            _obj.textarray[k].v = mesh->textarray[k].v;
        }
        // задать локальную сист. координат.
        _obj.localCS.H.LoadIdentity ();
        C3DVectorF X1(mesh->locmatrix[0 * 3 + 0], mesh->locmatrix[0 * 3 + 1], mesh->locmatrix[0 * 3 + 2]);
        C3DVectorF X2(mesh->locmatrix[1 * 3 + 0], mesh->locmatrix[1 * 3 + 1], mesh->locmatrix[1 * 3 + 2]);
        C3DVectorF X3(mesh->locmatrix[2 * 3 + 0], mesh->locmatrix[2 * 3 + 1], mesh->locmatrix[2 * 3 + 2]);
        C3DVectorF O (mesh->locmatrix[3 * 3 + 0], mesh->locmatrix[3 * 3 + 1], mesh->locmatrix[3 * 3 + 2]);
        _obj.localCS.H_from_zero.LoadInverseTransform (X1, X2, X3, O); // загрузить матрицу трансформации из 0!
        _obj.localCS.H_from_zero.Transpose (); // транспонировать для OpenGL - нужно ли???
        _obj.localCS.H_to_zero = _obj.localCS.H_from_zero.Invert (); // матрица возврата в 0
        _obj.localCS.pivot.Set (0,0,0); //
        // распределить меши
        if (mesh->nmats > 0)
        {
            _obj.meshes.resize (mesh->nmats);// количество подмешей.
            for (unsigned int k = 0; k < mesh->nmats; ++k)
            {
                const objmat3ds & omat = mesh->matarray[k];
                _obj.meshes[k].matname = omat.name; // скопировать имя и найти локальный индекс материала используемый подмешем.
                _obj.meshes[k].MatIndex = rend.GetMaterialIndex (omat.name); // если не будет найдено имя материала то будем использовать 0 как дефолтный.
                // скопировать фейсы подмеша
                _obj.meshes[k].facearray.resize (omat.nfaces);
                for (unsigned int j = 0; j < omat.nfaces; ++j)
                {
                    _obj.meshes[k].facearray[j].V1 = mesh->facearray[omat.faceindex[j]].v1;
                    _obj.meshes[k].facearray[j].V2 = mesh->facearray[omat.faceindex[j]].v2;
                    _obj.meshes[k].facearray[j].V3 = mesh->facearray[omat.faceindex[j]].v3;
                }
            }
        }
        else
        { // так бывает если нет ни одного материала и нет разбивки на подмеши, нужно все засунуть в 1 меш.
            _obj.meshes.resize (1);// количество подмешей.
            _obj.meshes[0].matname = "_SMelm_def"; // дефолтный материал
            _obj.meshes[0].MatIndex = 0; //
            // скопировать фейсы подмеша
            _obj.meshes[0].facearray.resize (mesh->nfaces);
            for (unsigned int j = 0; j < mesh->nfaces; ++j)
            {
                _obj.meshes[0].facearray[j].V1 = mesh->facearray[j].v1;
                _obj.meshes[0].facearray[j].V2 = mesh->facearray[j].v2;
                _obj.meshes[0].facearray[j].V3 = mesh->facearray[j].v3;
            }
        }
        RelMeshObj3ds(&mesh);
    }

    ulong3ds nbrobj = GetObjectNodeCount3ds(db);
    if (ftkerr3ds) return __LINE__;
    for (ulong3ds i = 0; i < nbrobj; ++i)
    {
        kfmesh3ds *omo=NULL;
        GetObjectMotionByIndex3ds (db, i, &omo);
        if (ftkerr3ds) return __LINE__;
        std::string oname = omo->name;
        if (oname == "$$$DUMMY") // блядский пустой узел
        {
            // его нужно внести в список объектов
            S3DObject dymmy;
            dymmy.name = oname + "." + omo->instance;
            dymmy.localCS.pivot.Set (omo->pivot.x, omo->pivot.y, omo->pivot.z);
            dymmy.localCS.H.LoadIdentity ();
            dymmy.localCS.H_from_zero.LoadIdentity ();
            dymmy.localCS.H_to_zero.LoadIdentity ();
            dymmy.parent_name = omo->parent;
            dymmy.parent_index = rend.GetObjectIndex (omo->parent);
            rend.m_objects.push_back (dymmy);
        }
        else
        {
            int oi = rend.GetObjectIndex (omo->name);
            if (oi != -1) // такого быть не должно иначе чушь!
            {
                rend.m_objects[oi].parent_name = omo->parent;
                rend.m_objects[oi].parent_index = rend.GetObjectIndex (omo->parent);
                rend.m_objects[oi].localCS.pivot.Set (omo->pivot.x, omo->pivot.y, omo->pivot.z);
            }
        }
        ReleaseObjectMotion3ds (&omo);
    }
    ReleaseDatabase3ds(&db);
    CloseFile3ds(file);
    CloseAllFiles3ds ();

    // рассчитать нормали для всех объектов
    rend.CalcNormals ();
    rend.LoadTextures ();
    // структура считана и заполнена.
    // теперь нужно дерево иерархии заполнить как-нибудь....
    rend.m_tree.obj_index = 0;
    FindAllRefsToMe (rend.m_tree, rend);
    rend.UnProtect ();
    return 0;
}
