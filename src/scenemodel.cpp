#include "scenemodel.h"
#include "assimp.hpp"
#include "aiScene.h"
#include "aiPostProcess.h"

void SceneModel::loadScene(const std::string &filename)
{
    if (!filename.length()) // Dialog canceled
        return;

    const aiScene *newScene = importer.ReadFile(filename, aiProcess_GenNormals);
    if (!scene)
    {
        throw ImportError(importer.GetErrorString());
    }
    scene = newScene;
}

ImportError::ImportError(const char *amsg): msg(amsg) {}

const char * ImportError::what() const throw()
{
    return msg;
}

SceneModel::SceneModel(const aiScene *aScene): scene(aScene) {}

void SceneModel::GetExtensionList(std::string &out) const
{
    importer.GetExtensionList(out);
}
