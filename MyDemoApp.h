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

class MyApp : public Application
{
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
protected:
	Texture* mDiffuseTexture;
	shader* mStaticShader;
	shader* mSkinnedShader;
	std::vector<Mesh> mGPUMeshes;
	std::vector<Mesh> mCPUMeshes;
	Transform mModel;
};

#endif // !_MyApp_
