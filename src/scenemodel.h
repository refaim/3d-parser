#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include <string>
#include <exception>
#include "assimp.hpp"
#include "aiScene.h"

class ImportError: std::exception {
    const char *msg;
public:
    ImportError(const ImportError &ie): msg(ie.what()) {}
    ImportError(const char *amsg);
    virtual const char *what() const throw();
};

class SceneModel
{
    Assimp::Importer importer; // Lifetime of importer determines lifetime of scene

    void getBoundingBoxForNode(const aiNode *nd, aiVector3D &min, aiVector3D &max, aiMatrix4x4 &trafo);
    std::string filename;
public:
    const aiScene *scene;
    aiVector3D scene_min, scene_max, scene_center;

    const aiScene *operator->() { return scene; }
    void GetExtensionList(std::string &out) const;
    void loadScene(const std::string &filename);
    void getBoundingBox(aiVector3D &min, aiVector3D &max);
    const std::string &getFilename();


    bool isLoaded() const;
    SceneModel(const aiScene *aScene = 0);
};

#endif // SCENEMODEL_H
