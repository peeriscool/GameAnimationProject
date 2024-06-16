#ifndef _H_APPLICATION_
#define _H_APPLICATION_

class Application //abstract app class
{
private:
	int mouseX;
	int mouseY;

	Application(const Application&);
	Application& operator=(const Application&);
public:
	inline Application(){}
	inline virtual ~Application(){}
	inline virtual void Initialize(){}
	inline virtual void Update(float inDeltatime){} //argument to Update frame
	inline virtual void Render(float inAspectRatio) {} //aspect ratio of window
	inline virtual void Shutdown(){}
	inline virtual void UpdateMousePosition(int x, int y) // Load Mouse location from winMain
	{
		mouseX = x;
		mouseY = y;
	}
};

#endif
