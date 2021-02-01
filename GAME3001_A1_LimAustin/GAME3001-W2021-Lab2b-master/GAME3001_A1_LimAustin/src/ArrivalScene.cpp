#include "ArrivalScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI

#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Util.h"

ArrivalScene::ArrivalScene()
{
	ArrivalScene::start();
}

ArrivalScene::~ArrivalScene()
= default;

void ArrivalScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 160, 160, 160, 255); // background
}

void ArrivalScene::update()
{
	updateDisplayList();
	
	m_pArrivalShip->m_stopOnTouch();
	
}

void ArrivalScene::clean()
{
	removeAllChildren();
}

void ArrivalScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) // goes to seeking
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4)) // goes to Avoidance
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round4.wav", "round3", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("round3", 0);
		TheGame::Instance()->changeSceneState(COLLISION_SCENE);
	}

}

void ArrivalScene::start()
{
	// Set GUI Title
	m_guiTitle = "Arrival Scene";

	/* Seeking Label */
	m_pArrivalLabel = new Label("Arrival", "Consolas");
	m_pArrivalLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 10.0f);
	addChild(m_pArrivalLabel);


	// create target
	m_pArrivalTarget = new Target();
	m_pArrivalTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pArrivalTarget);

	// create ship
	m_pArrivalShip = new SpaceShip();
	m_pArrivalShip->getTransform()->position = glm::vec2(50.0f, 300.0f);
	m_pArrivalShip->setEnabled(true);
	m_pArrivalShip->setDestination(m_pArrivalTarget->getTransform()->position);
	addChild(m_pArrivalShip);

}

void ArrivalScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Target Pos"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	static float speed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pArrivalShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pArrivalShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pArrivalShip->getRotation();
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pArrivalShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pArrivalShip->setTurnRate(turn_rate);
	}

	if (ImGui::Button("Start"))
	{
		m_pArrivalShip->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_pArrivalShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pArrivalShip->setEnabled(false);
		m_pArrivalShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pArrivalShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pArrivalShip->getRotation();
	}
	ImGui::Separator();
	static float targetPosition[2] = { m_pArrivalTarget->getTransform()->position.x, m_pArrivalTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pArrivalTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pArrivalShip->setDestination(m_pArrivalTarget->getTransform()->position);
	}
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
