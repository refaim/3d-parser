#include "scenemodel.h"
#include "aiScene.h"
#include "aiPostProcess.h"

void SceneModel::loadScene(const std::string &aFilename)
{
    if (!aFilename.length()) // Dialog canceled
        return;

    const aiScene *newScene = importer.ReadFile(aFilename, aiProcess_GenNormals);
    if (!newScene)
    {
        throw ImportError(importer.GetErrorString());
    }
    scene = newScene;
    filename = aFilename;

    getBoundingBox(scene_min, scene_max);
    scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
    scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
    scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
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

bool SceneModel::isLoaded() const
{
    return scene;
}

void SceneModel::getBoundingBox(aiVector3D &min, aiVector3D &max)
{
    aiMatrix4x4 trafo;

    min.x = min.y = min.z = 1e10f;
    max.x = max.y = max.z = -1e10f;

    getBoundingBoxForNode(scene->mRootNode, min, max, trafo);
}

void SceneModel::getBoundingBoxForNode(const aiNode *nd, aiVector3D &min, aiVector3D &max, aiMatrix4x4 &trafo)
{
    aiMatrix4x4 prev;

    prev = trafo;
    trafo *= nd->mTransformation;

    for (unsigned int n = 0; n < nd->mNumMeshes; ++n)
    {
        const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        for (unsigned int t = 0; t < mesh->mNumVertices; ++t)
        {
            aiVector3D tmp = mesh->mVertices[t];
            tmp *= trafo;

            min.x = std::min(min.x, tmp.x);
            min.y = std::min(min.y, tmp.y);
            min.z = std::min(min.z, tmp.z);

            max.x = std::max(max.x, tmp.x);
            max.y = std::max(max.y, tmp.y);
            max.z = std::max(max.z, tmp.z);
        }
    }

    for (unsigned int n = 0; n < nd->mNumChildren; ++n)
    {
        getBoundingBoxForNode(nd->mChildren[n], min, max, trafo);
    }

    trafo = prev;
}

const std::string & SceneModel::getFilename()
{
    return filename;
}
