#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 160, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	


}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
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
		TheGame::Instance()->changeSceneState(ARRIVAL_SCENE);
		TheGame::Instance()->changeSceneState(COLLISION_SCENE);
	}

}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	SoundManager::Instance().load("../Assets/audio/MarioPaint.ogg", "paint", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(1);
	SoundManager::Instance().playMusic("paint", -1);
	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press 1: Seeking||Press 2: Fleeing||Press 3: Arrival||Press 4: Avoidance", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 10.0f);
	addChild(m_pInstructionsLabel);

	m_pSubLabel = new Label("Caution for Sound", "Consolas");
	m_pSubLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 40.0f);
	addChild(m_pSubLabel);

}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
