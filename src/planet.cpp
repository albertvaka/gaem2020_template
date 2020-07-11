#include "planet.h"

#include "text.h"
#include "assets.h"
#include "window.h"
#include "asteroid.h"
#include "collide.h"

const float size = 50.0;

Planet::Planet(float orbit_radius, float orbit_offset, float mass, float rps)
	: orbit_radius(orbit_radius)
	, mass(mass)
	, health(INITIAL_HEALTH)
	, CircleEntity(vec(), size)
	, rps(rps)
	, curr_angle(orbit_offset)
{
	Debug::out << "Planet created";
}

void Planet::Update(float dt) 
{
	this->curr_angle += rps * dt;
	pos = vec(Window::GAME_WIDTH/2, Window::GAME_HEIGHT/2)
		+ vec::FromAngle(Mates::DegsToRads(curr_angle), orbit_radius);

	auto asteroids = Asteroid::GetAll();

	for (auto asteroid : asteroids) {
		if (Collide(this, asteroid)) {
			health -= 10*asteroid->size;
			asteroid->alive = false;
		}
	}
}

void Planet::Draw() const
{
	GPU_Image* texture = Assets::planetTextures[planetTexture];
	float scale = size / texture->base_h*2;
	
	Window::Draw(texture, pos)
		.withOrigin(texture->w / 2, texture->h / 2)
		.withScale(scale);

	Text txt_health(Assets::font_30);
	txt_health.setString(std::to_string(int(ceil(health))));
	Window::Draw(txt_health, pos + vec(0, size / 2.0 + 10.0)) 
		.withOrigin(txt_health.getSize().x, 0)
		.withScale(0.5f);

	if (Debug::Draw) {
		DrawBounds(100, 100, 255);
	}
}
