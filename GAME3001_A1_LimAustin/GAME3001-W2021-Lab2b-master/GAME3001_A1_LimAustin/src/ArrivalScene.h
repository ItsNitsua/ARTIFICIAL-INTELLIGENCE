#pragma once
#ifndef __ARRIVAL_SCENE__
#define __ARRIVAL_SCENE__

#include "Button.h"
#include "Label.h"
#include "Plane.h"
#include "Player.h"
#include "Scene.h"
#include "SpaceShip.h"
#include "Target.h"

class ArrivalScene : public Scene
{
public:
	ArrivalScene();
	~ArrivalScene();

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

	Label* m_pArrivalLabel;

	SpaceShip* m_pArrivalShip;
	Target* m_pArrivalTarget;
};

#endif 
