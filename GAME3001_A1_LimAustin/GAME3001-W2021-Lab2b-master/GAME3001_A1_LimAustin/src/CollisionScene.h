#pragma once
#pragma once
#ifndef __COLLISION_SCENE__
#define __COLLISION_SCENE__

#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "Plane.h"
#include "Player.h"
#include "Scene.h"
#include "SpaceShip.h"
#include "Target.h"

class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Label* m_pCollisionLabel;
	SpaceShip* m_pCollisionShip;
	Target* m_pCollisionTarget;
	Obstacle* m_pObstacle;
};

#endif 