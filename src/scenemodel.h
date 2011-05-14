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
    const aiScene *scene;
public:
    void GetExtensionList(std::string &out) const;
    void loadScene(const std::string &filename);
    SceneModel(const aiScene *aScene = 0);
};

#endif // SCENEMODEL_H
