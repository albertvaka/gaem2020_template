#pragma once

#include "camera.h"
#include "assets.h"
#include "window.h"
#include "text.h"
#include "vec.h"

struct RotoText : Text
{
	bool shown = false;
	float timer = -1;

	const float DURATION = 2.2f;
	const float INTRO_DURATION = 0.3f;

	RotoText() : Text(Assets::font_30, Assets::font_30_outline)
	{
		setFillColor(255,255,255);
		setOutlineColor(0,0,0);
	}

	void ShowMessage(const std::string& msg)
	{
		setString(msg);
		timer = 0.01f;
		shown = true;
	}

	void Update(float dt) {
		if (timer >= 0) {
			timer += dt;
		}
	}

	void Draw(vec pos, float scale = 1.f) {
		if (timer < 0) {
			return;
		}

		float messageTime = timer;
		/*if (messageTime > DURATION) {
			messageTime = (DURATION + INTRO_DURATION) - messageTime;
			if (messageTime < 0) {
				timer = -1.f;
				return;
			}
		}*/

		float ang = sin(messageTime * 2) * 10;
		float sc = 1;
		if (messageTime < INTRO_DURATION)
		{
			sc = 1 + (INTRO_DURATION - messageTime) * 10;
			ang -= (INTRO_DURATION - messageTime) * 600;
		}

		if (messageTime < INTRO_DURATION) {
			sc = messageTime / INTRO_DURATION;
		}

		Window::Draw(*this, pos)
			.withOrigin(getSize() / 2.f)
			.withScale(2*sc * scale)
			.withRotation(ang);
	}
};


