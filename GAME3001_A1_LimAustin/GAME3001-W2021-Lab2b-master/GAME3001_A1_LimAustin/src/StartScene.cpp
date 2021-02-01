#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 150, 255, 255, 255);
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

}

void StartScene::start()
{
	const SDL_Color pink = { 255, 153, 255, 255 };
	m_pStartLabel = new Label("Austin Lim", "Lazy", 90, pink, glm::vec2(400.0f, 60.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pShip);
	
	m_pInstructionsLabel = new Label("ID:101251873", "Lazy", 40, pink, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

