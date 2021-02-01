#include "SeekingScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

SeekingScene::SeekingScene()
{
	SeekingScene::start();
}

SeekingScene::~SeekingScene()
= default;

void SeekingScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 160, 255);
}

void SeekingScene::update()
{
	updateDisplayList();

	
}

void SeekingScene::clean()
{
	removeAllChildren();
}

void SeekingScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
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
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4)) // goes to Avoidance
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round4.wav", "round4", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("round4", 0);
		TheGame::Instance()->changeSceneState(COLLISION_SCENE);
	}
}

void SeekingScene::start()
{
	// Set GUI Title
	m_guiTitle = "Seeking Scene";

	m_pSeekingTarget = new Target();
	m_pSeekingTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pSeekingTarget);

	/* Seeking Label */
	m_pSeekingLabel = new Label("Seeking", "Consolas");
	m_pSeekingLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 10.0f);
	addChild(m_pSeekingLabel);
	
	// instantiating spaceship
	m_pSeekingShip = new SpaceShip();
	m_pSeekingShip->getTransform()->position = glm::vec2(-100.0f, 300.0f);
	m_pSeekingShip->setEnabled(true);
	m_pSeekingShip->setDestination(m_pSeekingTarget->getTransform()->position);
	addChild(m_pSeekingShip);
}

void SeekingScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1: Austin Lim", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pSeekingShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pSeekingShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSeekingShip->getRotation();
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSeekingShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSeekingShip->setTurnRate(turn_rate);
	}

	if (ImGui::Button("Start"))
	{
		m_pSeekingShip->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_pSeekingShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pSeekingShip->setEnabled(false);
		m_pSeekingShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSeekingShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pSeekingShip->getRotation();
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pSeekingTarget->getTransform()->position.x, m_pSeekingTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pSeekingTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSeekingShip->setDestination(m_pSeekingTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
