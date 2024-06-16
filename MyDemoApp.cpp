#define _CRT_SECURE_NO_WARNINGS
#include "MyDemoApp.h"
#include "GLTFLoader.h"
#include "Uniform.h"
#include <iostream>
//#include "quat.h"
quat Camquat;
float pitch = 0;
float yaw = 0;
vec3 up = vec3(0.0f, 1.0f, 0.0f);
vec3 forward = vec3(0.0f,0.0f,-1.0f);
bool FirstMouse = true;
float lastX, lastY,movSpeed;
vec3 camposition;
Transform Camera;
//forward decleration
std::vector<Mesh> TruckMesh;
int walksize = 6;
quat dir = quat(0, 1, 0, 1);
quat dir2 = quat(0, 1, 0, 0);
quat dir3 = quat(0, 1, 0, -1);
quat dir4 = quat(0, 0, 0, 1);

//quat dir3 = quat(0, -1, 0, 0);
void WalkingCycle(float deltaTime, Transform mModel);

void MyApp::Initialize() {
    //initialize camera
    Camera = Transform(vec3(0, 0, 0), quat(0, 0, 0, 0), vec3(1, 1, 1));
    movSpeed = 0.1f;
    //loading gltf meshes, skeletons and poses

   
    cgltf_data* gltf = LoadGLTFFile("Resources/Woman.gltf");
    cgltf_data* Truck = LoadGLTFFile("Resources/CesiumMilkTruck_Embeded.gltf");

    if (!gltf) {
        std::cerr << "Failed to load GLTF file." << std::endl;
        return;
    }
    //Initial positions,rotations and scales
    Transform mModel = Transform(vec3(0.5, 0.5, 11.5), quat(1, 0, 0, 1),vec3(1,1,1));
    TruckMesh = LoadMeshes(Truck);
    mCPUMeshes = LoadMeshes(gltf);
    mSkeleton = LoadSkeleton(gltf);
    mClips = LoadAnimationClips(gltf);
   // mCPUMeshes.push_back(Mesh{ /* initialization */});
  

    if (mCPUMeshes.empty()) {
        std::cerr << "No meshes loaded." << std::endl;
        return;
    }

    std::cout << "Number of meshes: " << mCPUMeshes.size() << std::endl;

    //assign shaders and Textures
    mStaticShader = new shader("Resources/staticVert.shader", "Resources/litFrag.shader");
    mDiffuseTexture = new Texture("Resources/Woman.png");
    TruckDiffuseTexture = new Texture("Resources/CesiumMilkTruck.jpg");

    if (!mDiffuseTexture->GetHandle()) 
    {
        std::cerr << "Failed to load texture." << std::endl;
        return;
    }
    mGPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
    mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
    mCPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
    mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
    

    mCPUAnimInfo.mModel.position = vec3(0, 0, 0);
    unsigned int numUIClips = (unsigned int)mClips.size();
    for (unsigned int i = 0; i < numUIClips; ++i) {
        if (mClips[i].GetName() == "Walking") {
            mCPUAnimInfo.mClip = i;
        }
        else if (mClips[i].GetName() == "Running") {
            mCPUAnimInfo.mClip = i;
        }
    }
  //  mStaticShader->Bind();
}

void MyApp::Render(float inAspectRatio) \
{
    UpdateCamera();
    forward = vec3( 1.0f, -Camquat.x, Camquat.y);
    //std::cout << "forward:" << forward.x << forward.y << forward.z << std::endl;
    mat4 projection = perspective(90.0f, inAspectRatio, 0.01f, 1000.0f);
    mat4 view = lookAt(Camera.position, Camera.position - forward, up); //cameraposition, cameraposition - camforward, camup
    mat4 model = transformToMat4(mCPUAnimInfo.mModel);

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
    mat4 modeltruck = transformToMat4(mModel); //load Truck model as mat4
    Uniform<mat4>::Set(mStaticShader->GetUniform("model"), modeltruck);

    TruckDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0); //redefine texture for next model
    for (unsigned int i = 0, size = (unsigned int)TruckMesh.size(); i < size; ++i) {

        TruckMesh[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
        TruckMesh[i].Draw();
        TruckMesh[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
    }
}
void MyApp::Update(float deltaTime) 
{
    //animation updating
    mCPUAnimInfo.mPlayback = mClips[mCPUAnimInfo.mClip].Sample(mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mPlayback + deltaTime);
    mGPUAnimInfo.mPlayback = mClips[mGPUAnimInfo.mClip].Sample(mGPUAnimInfo.mAnimatedPose, mGPUAnimInfo.mPlayback + deltaTime);

    for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
        mCPUMeshes[i].CPUSkin(mSkeleton, mCPUAnimInfo.mAnimatedPose);
    }
    
    mGPUAnimInfo.mAnimatedPose.GetMatrixPalette(mGPUAnimInfo.mPosePalette);
   



    if (mCPUAnimInfo.mModel.position.z < walksize && mCPUAnimInfo.mModel.position.x == 0) //
    {
       // mCPUAnimInfo.mModel.rotation = nlerp(mCPUAnimInfo.mModel.rotation, originalDir, deltaTime * 10);
        mCPUAnimInfo.mModel.position.z += 0.02f;
      //  mModel.position.x += 0.01;
        std::cout << "forwardX:" << mCPUAnimInfo.mModel.rotation.x << std::endl;
        std::cout << "forwardY:" << mCPUAnimInfo.mModel.rotation.y << std::endl;
        std::cout << "forwardZ:" << mCPUAnimInfo.mModel.rotation.w << std::endl;

    }
    else if (mCPUAnimInfo.mModel.position.z >= walksize && mCPUAnimInfo.mModel.position.x < walksize)
    {
        mCPUAnimInfo.mModel.rotation = nlerp(mCPUAnimInfo.mModel.rotation, dir, deltaTime*10);
        mCPUAnimInfo.mModel.position.x += 0.02f;

        std::cout << "left:" << mCPUAnimInfo.mModel.rotation.z << std::endl;
    
    }
    else if(mCPUAnimInfo.mModel.position.z > 0 && mCPUAnimInfo.mModel.position.x >= walksize)
    {
            mCPUAnimInfo.mModel.position.z -= 0.02f;
            mCPUAnimInfo.mModel.rotation = nlerp(mCPUAnimInfo.mModel.rotation, dir2, deltaTime*10);
            std::cout << "right:" << mCPUAnimInfo.mModel.rotation.z << std::endl;
           
    }
    else if(mCPUAnimInfo.mModel.position.z <= 0 && mCPUAnimInfo.mModel.position.x >0)
    {
            mCPUAnimInfo.mModel.rotation = nlerp(mCPUAnimInfo.mModel.rotation, dir3, deltaTime*10);
            mCPUAnimInfo.mModel.position.x -= 0.02f; //walk back to start
            std::cout << "back:" << mCPUAnimInfo.mModel.rotation.z << std::endl;

    }
    else
    {
        mCPUAnimInfo.mModel.position = vec3(0,0,0);
        mCPUAnimInfo.mModel.rotation = dir4;
    }
   // std::cout << mCPUAnimInfo.mModel.rotation.y << std::endl;
    //key presses
    if (keys['W'])
    {
        Camera.position.x += -movSpeed;
       // std::cout << "Y:" << camposition.y << std::endl;
    }
    if (keys['S'])
    {
        Camera.position.x += movSpeed;
       // std::cout << "Y:" << camposition.y << std::endl;
    }
    if (keys['A'])
    {
        Camera.position.z += movSpeed;
       // std::cout << "X:" << camposition.x << std::endl;
    }
    if (keys['D'])
    {
        Camera.position.z += -movSpeed;
        //std::cout << "X:" << camposition.x << std::endl;
    }
    if (keys['Q'])
    {
        Camera.position.y += -movSpeed;
       // std::cout <<"Z:" << camposition.z << std::endl;
    }
    if (keys['E'])
    {
        Camera.position.y += +movSpeed;
       //std::cout << "Z:" << camposition.z << std::endl;
    }
    
}
void MyApp::Shutdown(){}
void MyApp::UpdateCamera()
{
    float x = (float)mouseX;
    float y = (float)mouseY;

    if (FirstMouse)
    {
        lastX = x;
        lastY = y;
        FirstMouse = false;
    }
    int screenWidth = 1280;
    int screenHeight = 720;
    float normalizedX = (2.0f * mouseX) / screenWidth - 1.0f;
    float normalizedY = 1.0f - (2.0f * mouseY) / screenHeight; // Invert Y axis

    lastX = x;
    lastY = y;
    yaw = normalizedX *180.0f * 1.0f;
    pitch = normalizedY *180.0f * 1.0f;

    float pitchRadians = degreesToRadians(pitch);
    float yawRadians = degreesToRadians(yaw);
    Camquat = quat(pitchRadians, yawRadians,0.0f,0.0f);
}
void WalkingCycle(float deltaTime,Transform mModel)
{
   /* if (mModel.position.z < 3 && mModel.position.x == 0) {
        mModel.position.z += 0.03f;
        mModel.rotation = nlerp(mModel.rotation, dir, deltaTime);
        std::cout << "forward: " << mModel.position.z << std::endl;
    }
    else if (mModel.position.z >= 3 && mModel.position.x < 3) {
        mModel.rotation = nlerp(mModel.rotation, dir2, deltaTime);
        mModel.position.x += 0.03f;
        std::cout << "left: " << mModel.position.x << std::endl;
    }
    else if (mModel.position.z > 0 && mModel.position.x >= 3) {
        mModel.position.z -= 0.03f;
        mModel.rotation = nlerp(mModel.rotation, dir, deltaTime);
        std::cout << "backward: " << mModel.position.z << std::endl;
    }
    else if (mModel.position.z <= 0 && mModel.position.x > 0) {
        mModel.rotation = nlerp(mModel.rotation, dir2, deltaTime);
        mModel.position.x -= 0.03f;
        std::cout << "right: " << mModel.position.x << std::endl;
    }*/
}