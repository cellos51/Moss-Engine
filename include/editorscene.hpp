#pragma once

#include "scene.hpp"

#include "global.hpp"
#include "openglwindow.hpp"
#include "entity.hpp"
#include "math.hpp"
#include "level.hpp"

class EditorScene : public Scene
{
public:
	EditorScene(OpenGLWindow& window);
	void onStart() override;
	void onEnd() override;
	void update() override;
	void fixedUpdate() override;
	void render(OpenGLWindow& window) override;
private:
	OpenGLWindow& window;

	// camra offset used for panning
	Vector2 offsetCam;
	Vector2 offsetMouse;

	// literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it) done lmao (oct 26 '23)
	LevelData level;

	std::vector<Entity> collisionView;
	unsigned int collisionTexture = 0;
	bool viewCollision = false;
	bool editingPlayer = false;
	bool editingLights = false;

	ui::Button tileSet;
	ui::Panel topBar;
	ui::Panel sideBar;
	ui::Button layer1;
	ui::Button layer2;
	ui::Button layer3;
	ui::Button layer4;
	ui::Button collisionButton;
	ui::Button playerButton;
	ui::Button lightsButton;
	ui::Button saveButton;
	ui::Button saveAsButton;
	ui::Button loadButton;
	ui::Button importButton;
	ui::Slider brightness;
	ui::Slider radius;
	ui::Slider red;
	ui::Slider green;
	ui::Slider blue;

	unsigned int font = 0;
	unsigned int activeLayer = 1;
	std::string currentFile;

	Entity selector = Entity(Vector2(0, TILE_SIZE * 2), Vector2(TILE_SIZE, TILE_SIZE));
	Entity cursor = Entity(Vector2(TILE_SIZE, TILE_SIZE));
	Light lightCursor;
	Entity playerSpawn = Entity(Vector2(TILE_SIZE, TILE_SIZE));
};

