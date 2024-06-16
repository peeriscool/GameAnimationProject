#define _CRT_SECURE_NO_WARNINGS
#include "MyDemoApp.h"
#include "GLTFLoader.h"
#include "Uniform.h"
//#include "quat.h"
#define PI 3.141592653589793
void MyApp::Initialize() {
    cgltf_data* gltf = LoadGLTFFile("Resources/CesiumMilkTruck_Embeded.gltf");
    // cgltf_data* gltf = LoadGLTFFile("Resources/Woman.gltf");

    if (!gltf) {
        std::cerr << "Failed to load GLTF file." << std::endl;
        return;
    }

    Transform mModel;
    mCPUMeshes = LoadMeshes(gltf);
    FreeGLTFFile(gltf);

    if (mCPUMeshes.empty()) {
        std::cerr << "No meshes loaded." << std::endl;
        return;
    }

    std::cout << "Number of meshes: " << mCPUMeshes.size() << std::endl;

    mStaticShader = new shader("Resources/staticVert.shader", "Resources/litFrag.shader");
    mDiffuseTexture = new Texture("Resources/CesiumMilkTruck.jpg");

  /*  if (!mDiffuseTexture->IsLoaded()) {
        std::cerr << "Failed to load texture." << std::endl;
        return;
    }*/

    mModel.position = vec3(0, 0, 0);
    mModel.scale = vec3(0.5, 0.5, 0.5);
    mModel.rotation.x = 1.1f;
    mStaticShader->Bind();
}

void MyApp::Render(float inAspectRatio) {
   
    //TODO Convert MousX and y to Camera pitch yaw 

    mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    mat4 view = lookAt(vec3(mouseX, mouseY, 7), vec3(0, 4, 0), vec3(0, 1, 0));
    mat4 model = transformToMat4(mModel);

    mStaticShader->Bind();
    Uniform<mat4>::Set(mStaticShader->GetUniform("model"), model);
    Uniform<mat4>::Set(mStaticShader->GetUniform("view"), view);
    Uniform<mat4>::Set(mStaticShader->GetUniform("projection"), projection);
    Uniform<vec3>::Set(mStaticShader->GetUniform("light"), vec3(1, 1, 1));

    mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);

    for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
        mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
        mCPUMeshes[i].Draw();
        mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
    }
}
void MyApp::Update(float deltaTime) 
{

   // std::cout << mouseX<< std::endl;
//    quat rotationSpeed = quat(1,0.01f,1,0); // Adjust as needed

    // Apply the incremental rotation to the model's current rotation
 //   mModel.rotation = rotationSpeed * mModel.rotation;

   // mModel.rotation.y += 0.001;

    // // = vec3(0, deltaTime, 0);
   // mCPUMeshes[0].UpdateOpenGLBuffers();
   // mCPUMeshes[0].GetPosition();
}
void MyApp::Shutdown(){}

//void MyApp::Initialize() {
//	//cgltf_data* gltf = LoadGLTFFile("Resources/CesiumMilkTruck_Embeded.gltf"); 
//	cgltf_data* gltf = LoadGLTFFile("Resources/Woman.gltf");
//	Transform mModel;
//	mCPUMeshes = LoadMeshes(gltf);
//	FreeGLTFFile(gltf);
//	
//	std::cout << gltf->nodes << std::endl;
//	
//	mStaticShader = new shader("Resources/staticVert.shader", "Resources/litFrag.shader");
//	mDiffuseTexture = new Texture("Resources/CesiumMilkTruck.jpg");
//	
//	mModel.position = vec3(2, 0, 0);
//
//	mStaticShader->Bind();
//
//}
//
//void MyApp::Update(float deltaTime) {
//	
//}
//
//void MyApp::Render(float inAspectRatio) {
//	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
//	mat4 view = lookAt(vec3(0, 4, 7), vec3(0, 4, 0), vec3(0, 1, 0));
//	mat4 model;
//
//	model = transformToMat4(mModel);
//	mStaticShader->Bind();
//	Uniform<mat4>::Set(mStaticShader->GetUniform("model"), model);
//	Uniform<mat4>::Set(mStaticShader->GetUniform("view"), view);
//	Uniform<mat4>::Set(mStaticShader->GetUniform("projection"), projection);
//	Uniform<vec3>::Set(mStaticShader->GetUniform("light"), vec3(1, 1, 1));
//
//	mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);
//
//	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
//		mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
//		mCPUMeshes[i].Draw();
//		mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
//	}
//}
//
//void MyApp::Shutdown() {
////	delete mRestPoseVisual;
////	delete mCurrentPoseVisual;
////	delete mBindPoseVisual;
////	mClips.clear();
//}