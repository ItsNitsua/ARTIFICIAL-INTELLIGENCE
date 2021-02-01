#include "CollisionScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

CollisionScene::CollisionScene()
{
	CollisionScene::start();
}

CollisionScene::~CollisionScene()
= default;

void CollisionScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 100, 50, 160, 255);
}

void CollisionScene::update()
{
	m_pCollisionShip->setMaxSpeed(1);
	updateDisplayList();
	//CollisionManager::AABBCheck(m_pCollisionShip, m_pObstacle);
	if (CollisionManager::lineRectCheck(m_pCollisionShip->m_leftWhisker.Start(), m_pCollisionShip->m_leftWhisker.End(), // left whisk
		(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5, m_pObstacle->getHeight() * 0.5)), 200.0f, 100.0f))
	{
		std::cout << "left line touched" << std::endl;
		m_pCollisionShip->turnRight();
		
	}
	else if (CollisionManager::lineRectCheck(m_pCollisionShip->m_rightWhisker.Start(), m_pCollisionShip->m_rightWhisker.End(), // right whisk
		(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5, m_pObstacle->getHeight() * 0.5)), 200.0f, 100.0f))
	{
		std::cout << "right line touched" << std::endl;
		m_pCollisionShip->turnLeft();
		
	
	}
}

void CollisionScene::clean()
{
	removeAllChildren();
}

void CollisionScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) // goes to Avoidance
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round1.wav", "round1", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("round1", 0);
		TheGame::Instance()->changeSceneState(SEEKING_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2)) // goes to fleeing
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round2.wav", "round2", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("round2", 0);
		TheGame::Instance()->changeSceneState(FLEEING_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3)) // goes to Arrival
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round3.wav", "round3", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("round3", 0);
		TheGame::Instance()->changeSceneState(ARRIVAL_SCENE);
	}
}

void CollisionScene::start()
{
	// Set GUI Title
	m_guiTitle = "Collision Scene";
	

	SoundManager::Instance().setSoundVolume(2);
	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);

	m_pCollisionTarget = new Target();
	m_pCollisionTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pCollisionTarget);

	/* Seeking Label */
	m_pCollisionLabel = new Label("Collision", "Consolas");
	m_pCollisionLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 10.0f);
	addChild(m_pCollisionLabel);
	
	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pObstacle);
	
	// instantiating spaceship
	m_pCollisionShip = new SpaceShip();
	m_pCollisionShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pCollisionShip->setEnabled(true);
	m_pCollisionShip->setDestination(m_pCollisionTarget->getTransform()->position);

	m_pCollisionShip->setWhisker(m_pCollisionShip->getTransform()->position, (m_pCollisionShip->getTransform()->position + m_pCollisionShip->getOrientation() * 100.0f));
	
	addChild(m_pCollisionShip);
}

void CollisionScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1: Austin Lim", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pCollisionShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pCollisionShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pCollisionShip->getRotation();
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pCollisionShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pCollisionShip->setTurnRate(turn_rate);
	}

	if (ImGui::Button("Start"))
	{
		m_pCollisionShip->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_pCollisionShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pCollisionShip->setEnabled(false);
		m_pCollisionShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pCollisionShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pCollisionShip->getRotation();
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pCollisionTarget->getTransform()->position.x, m_pCollisionTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pCollisionTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pCollisionShip->setDestination(m_pCollisionTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
