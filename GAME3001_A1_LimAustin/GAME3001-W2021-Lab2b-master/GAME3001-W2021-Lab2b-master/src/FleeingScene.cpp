#include "FleeingScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Util.h"

FleeingScene::FleeingScene()
{
	FleeingScene::start();
}

FleeingScene::~FleeingScene()
= default;

void FleeingScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 160, 160, 255); // background
}

void FleeingScene::update()
{
	updateDisplayList();
	if (CollisionManager::AABBCheck(m_pFleeingShip, m_pFleeingTarget))
	{
		m_pFleeingShip->moveBack();
	}
	

}

void FleeingScene::clean()
{
	removeAllChildren();
}

void FleeingScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) // goes to seeking
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round1.wav", "round1", SOUND_SFX);
		SoundManager::Instance().setMusicVolume(5);
		SoundManager::Instance().playMusic("round1", 0);
		TheGame::Instance()->changeSceneState(SEEKING_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3)) // goes to Arrival
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round3.wav", "round3", SOUND_SFX);
		SoundManager::Instance().setMusicVolume(5);
		SoundManager::Instance().playMusic("round3", 0);
		TheGame::Instance()->changeSceneState(ARRIVAL_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4)) // goes to Avoidance
	{
		SoundManager::Instance().load("../Assets/audio/mkt-round4.wav", "round4", SOUND_SFX);
		SoundManager::Instance().setMusicVolume(5);
		SoundManager::Instance().playMusic("round4", 0);
		TheGame::Instance()->changeSceneState(COLLISION_SCENE);
	}

}

void FleeingScene::start()
{
	// Set GUI Title
	m_guiTitle = "Fleeing Scene";

	/* Seeking Label */
	m_pFleeingLabel = new Label("Fleeing", "Consolas");
	m_pFleeingLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 10.0f);
	addChild(m_pFleeingLabel);

	// create target
	m_pFleeingTarget = new Target();
	m_pFleeingTarget->getTransform()->position = glm::vec2(450.0f, 300.0f);
	addChild(m_pFleeingTarget);

	// create ship
	m_pFleeingShip = new SpaceShip();
	m_pFleeingShip->getTransform()->position = glm::vec2(350.0f, 300.0f);
	m_pFleeingShip->setEnabled(true);
	m_pFleeingShip->setDestination(m_pFleeingTarget->getTransform()->position);
	addChild(m_pFleeingShip);

}

void FleeingScene::GUI_Function() const
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
		m_pFleeingShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pFleeingShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pFleeingShip->getRotation();
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pFleeingShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pFleeingShip->setTurnRate(turn_rate);
	}

	if (ImGui::Button("Start"))
	{
		m_pFleeingShip->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_pFleeingShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pFleeingShip->setEnabled(false);
		m_pFleeingShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pFleeingShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pFleeingShip->getRotation();
	}
	ImGui::Separator();
	static float targetPosition[2] = { m_pFleeingTarget->getTransform()->position.x, m_pFleeingTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pFleeingTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pFleeingShip->setDestination(m_pFleeingTarget->getTransform()->position);
	}
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
