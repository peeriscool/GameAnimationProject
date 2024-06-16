#define _CRT_SECURE_NO_WARNINGS
#include "Chapter09Sample01.h"
#include "GLTFLoader.h"
void Chapter09Sample01::Initialize() {
	cgltf_data* gltf = LoadGLTFFile("Resources/Woman.gltf");
	//mCPUMeshes = LoadMeshes(gltf);
	mSkeleton = LoadSkeleton(gltf);
	mRestPose = LoadRestPose(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	mGPUMeshes = mCPUMeshes;
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].UpdateOpenGLBuffers();
	}

	mStaticShader = new shader("Resources/static.vert", "Resources/lit.frag");
	mSkinnedShader = new shader("Resources/skinned.vert", "Resources/lit.frag");
	mDiffuseTexture = new Texture("Assets/Woman.png");

	mGPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
	mCPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());

	mGPUAnimInfo.mModel.position = vec3(-2, 0, 0);
	mCPUAnimInfo.mModel.position = vec3(2, 0, 0);


	mRestPoseVisual = new DebugDraw();
	mRestPoseVisual->FromPose(mRestPose);
	mRestPoseVisual->UpdateOpenGLBuffers();

	mBindPoseVisual = new DebugDraw();
	mBindPoseVisual->FromPose(mSkeleton.GetBindPose());
	mBindPoseVisual->UpdateOpenGLBuffers();

	mCurrentClip = 0;
	mCurrentPose = mRestPose;

	mCurrentPoseVisual = new DebugDraw();
	mCurrentPoseVisual->FromPose(mCurrentPose);
	mCurrentPoseVisual->UpdateOpenGLBuffers();

	// Loading animation by name from gltf
	unsigned int numUIClips = (unsigned int)mClips.size();
	for (unsigned int i = 0; i < numUIClips; ++i) {
		if (mClips[i].GetName() == "Running") {
			mCurrentClip = i;
			break;
		}
	}
}

void Chapter09Sample01::Update(float deltaTime) {
	mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + deltaTime);
	mCurrentPoseVisual->FromPose(mCurrentPose);
}

void Chapter09Sample01::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 4, 7), vec3(0, 4, 0), vec3(0, 1, 0));
	mat4 mvp = projection * view; // No model

	mRestPoseVisual->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), mvp);
	mCurrentPoseVisual->UpdateOpenGLBuffers();
	mCurrentPoseVisual->Draw(DebugDrawMode::Lines, vec3(0, 0, 1), mvp);
	mBindPoseVisual->Draw(DebugDrawMode::Lines, vec3(0, 1, 0), mvp);
}

void Chapter09Sample01::Shutdown() {
	delete mRestPoseVisual;
	delete mCurrentPoseVisual;
	delete mBindPoseVisual;
	mClips.clear();
}