#pragma once

#include <memory>

#include "openglwindow.hpp"

class Scene // the base class for a scene 
{
public:
	virtual void onStart() = 0;
	virtual void onEnd() = 0;
	virtual void update() {};
	virtual void fixedUpdate() {};
	virtual void render(OpenGLWindow& window) {};
};

class SceneManager
{
public:
	SceneManager();
	SceneManager(std::shared_ptr<Scene> scene);
	void openScene(std::shared_ptr<Scene> scene);
	void closeScene();
	void update();
	void fixedUpdate();
	void render(OpenGLWindow& window);
private:
	std::shared_ptr<Scene> activeScene;
};
