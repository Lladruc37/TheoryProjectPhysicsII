#ifndef __MODULE_H__
#define __MODULE_H__

class Application;
class Collider;

class Module
{
private:
	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool startEnabled = true) : App(parent), enabled(startEnabled)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(Collider* c1, Collider* c2)
	{}
};

#endif // !__MODULE_H__