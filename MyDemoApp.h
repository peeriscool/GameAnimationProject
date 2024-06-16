#ifndef _MyApp_
#define _MyApp_

#include "Application.h"
#include "DebugDraw.h"
#include <vector>
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "globals.h"

class MyApp : public Application
{
	struct AnimationInstance 
	{
		Pose mAnimatedPose;
		std::vector <mat4> mPosePalette;
		unsigned int mClip;
		float mPlayback;
		Transform mModel;

		inline AnimationInstance() : mClip(0), mPlayback(0.0f) { }
	};
private:
	int mouseX;
	int mouseY;

public:
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
	MyApp() : mouseX(0), mouseY(0) {} 
	void UpdateMousePosition(int x, int y) // Load Mouse location from winMain
	{
		mouseX = x;
		mouseY = y;
	}
	void UpdateCamera();
protected:
	
	Texture* mDiffuseTexture;
	Texture* TruckDiffuseTexture;
	shader* mStaticShader;
	shader* mSkinnedShader;
	std::vector<Mesh> mGPUMeshes;
	std::vector<Mesh> mCPUMeshes;
	Transform mModel;

	//animation data
	Pose mRestPose;
	Skeleton mSkeleton;
	Pose mCurrentPose;
	std::vector<Clip> mClips;
	unsigned int mCurrentClip;
	float mPlaybackTime;
	DebugDraw* mBindPoseVisual;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mCurrentPoseVisual;
	AnimationInstance mGPUAnimInfo;
	AnimationInstance mCPUAnimInfo;
};

#endif // !_MyApp_
