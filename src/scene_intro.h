#include "scene_main.h"

#include "input.h"

#ifdef _IMGUI
#include "imgui.h"
#endif

#include "assets.h"
#include "debug.h"
#include "collide.h"
#include "asteroid.h"
#include "sol.h"
#include "rand.h"
#include "scene_manager.h"


const float cicleTexturesTime = 2.7f;

const float cameraOffsetY = 150.f;

extern float mainClock;

struct SceneIntro : public Scene {

	Planet p1, p2, p3, p4;
	Sol sol;
	int currentLevel;
	float timer = 0.f;
	bool aiMode;

	Text startText, credits, textMode2pA, textMode2pB, textMode1pA, textMode1pB;
	RotoText rototext;

	SceneIntro(bool ai = false)
		: startText(Assets::font_30, Assets::font_30_outline)
		, credits(Assets::font_30, Assets::font_30_outline)
		, textMode2pA(Assets::font_30, Assets::font_30_outline)
		, textMode2pB(Assets::font_30, Assets::font_30_outline)
		, textMode1pA(Assets::font_30, Assets::font_30_outline)
		, textMode1pB(Assets::font_30, Assets::font_30_outline)
		, p1(200, 0, 10000, 30)
		, p2(330, 250, 10000, 18)
		, p3(455, 180, 10000, 12)
		, p4(600, 300, 10000, 9)
		, sol(vec(Window::GAME_WIDTH / 2, Window::GAME_HEIGHT / 2))
		, aiMode(ai)
	{
		Asteroid::DeleteAll();

		p1.health = -1;
		p2.health = -1;
		p3.health = -1;
		p4.health = -1;
		RotateTextures();

		credits.setString("A game by:\n\n\nGerard del Castillo\n\nAlbert Vaca Cintora\n\n\nArt by:\n\n\nLVGames, Vectorpixelstar, Helianthus Games, Ravenmore");

		rototext.ShowMessage("The Neighbours");
		rototext.Update(100);

		startText.setString("PRESS START");
		textMode2pA.setString("< 2 player mode >");
		textMode2pB.setString("<  2 player mode  >");

		textMode1pA.setString("< VS AI >");
		textMode1pB.setString("<  VS AI  >");

	}

	void RotateTextures() {
		int tex;
		do {
			tex = Random::roll(std::size(Assets::backgroundTextures));
		} while (currentLevel == tex);
		currentLevel = tex;
		
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex);
		p1.planetTexture = tex;
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex || p2.planetTexture == tex);
		p2.planetTexture = tex;
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex || p2.planetTexture == tex || p3.planetTexture == tex);
		p3.planetTexture = tex;
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex || p2.planetTexture == tex || p3.planetTexture == tex || p4.planetTexture == tex);
		p4.planetTexture = tex;
	}

	void EnterScene() override {
		Camera::SetCenter(Camera::GetCenter() + vec(0, -cameraOffsetY));
	}
	void ExitScene() override { 
		Camera::SetCenter(Camera::GetCenter() + vec(0, cameraOffsetY));
	}

	void Update(float dt) override
	{
		if (Input::IsJustPressedAnyPlayer(GameKeys::MENU_RIGHT) || Input::IsJustPressedAnyPlayer(GameKeys::MENU_LEFT)) {
			aiMode = !aiMode;
		}

		if (Input::IsPressedAnyPlayer(GameKeys::START)) {
			SceneManager::SetScene(new SceneMain(aiMode));
			return;
		}

		rototext.Update(dt*0.9f);

		for (Sol* sol : Sol::GetAll()) {
			sol->Update(dt);
		}

		for (Planet* planet : Planet::GetAll()) {
			planet->Update(dt);
		}

		timer += dt;
		if (timer >= cicleTexturesTime) {
			timer -= cicleTexturesTime;
			RotateTextures();
		}
	}

void Draw() override
	{
		Window::Clear(0, 0, 0);
		GPU_Image* bgAsset = Assets::backgroundTextures[currentLevel];
		Window::Draw(bgAsset, vec(0, -cameraOffsetY))
			.withScale(4)
			.withRect(
				0, 0,
				Window::GAME_WIDTH, Window::GAME_HEIGHT
			);
		
		for (const Planet* planet : Planet::GetAll()) {
			planet->Draw();
		}

		for (Sol* sol : Sol::GetAll()) {
			sol->Draw();
		}

		rototext.Draw(Camera::GetCenter() - vec(0, 150), 1.5f);

		if ((int(mainClock * 1000) / 400) % 2) {
			Window::Draw(startText, Camera::GetCenter() + vec(0, cameraOffsetY))
				.withOrigin(startText.getSize()/2);
		}
		
		GPU_Image* infoText;
		if ((int(mainClock * 1000) / 300) % 2) {
			infoText = aiMode ? textMode1pA : textMode2pA;
		}
		else {
			infoText = aiMode ? textMode1pB : textMode2pB;
		}
		Window::Draw(infoText, Camera::GetCenter() + vec(0, cameraOffsetY + 35))
			.withScale(0.5f)
			.withOrigin(vec(infoText->texture_w, infoText->texture_h)/2);

		vec scale(0.7f, 0.8f);
		Window::Draw(credits, vec(30, 1000-cameraOffsetY-30-(scale.y*credits.getSize().y)))
			.withScale(scale);
	}

};

