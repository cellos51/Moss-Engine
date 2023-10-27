#pragma once

#include "scene.hpp"

class EditorScene : public Scene
{
public:
	EditorScene(OpenGLWindow& window);
	void onStart() override;
	void onEnd() override;
	void update() override;
	void render(OpenGLWindow& window) override;
};

