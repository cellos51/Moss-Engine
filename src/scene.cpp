#include "scene.hpp"

SceneManager::SceneManager() : activeScene(nullptr)
{

}

SceneManager::SceneManager(std::shared_ptr<Scene> scene)
{
	openScene(scene);
}


void SceneManager::openScene(std::shared_ptr<Scene> scene)
{
	
	if (activeScene)
	{
		activeScene->onEnd();

		activeScene = scene;

		activeScene->onStart();
	}
	else
	{
		activeScene = scene;
		activeScene->onStart();
	}
}

void SceneManager::closeScene()
{
	if (activeScene)
	{
		activeScene->onEnd();
		activeScene = nullptr;
	}
}

void SceneManager::update()
{
	if (activeScene)
	{
		activeScene->update();
	}
}

void SceneManager::render(OpenGLWindow& window)
{
	if (activeScene)
	{
		activeScene->render(window);
	}
}