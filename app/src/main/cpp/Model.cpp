#include "NDKHelper.h"

#include "Model.h"

using namespace std;
using namespace Assimp;

Model::Model(const string &path) : path(path) {
    std::vector<uint8_t> buf;
    if (!ndk_helper::JNIHelper::GetInstance()->ReadFile(path.c_str(), &buf)) {
        LOGI("[ %s ] failed reading file: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    importer = new Importer();
    const aiScene *scene = importer->ReadFileFromMemory(buf.data(), buf.size(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGI("[ %s ] failed importing model file: %s", __FUNCTION__, path.c_str());
        throw exception();
    }
}

Model::~Model() {
    delete importer;
}