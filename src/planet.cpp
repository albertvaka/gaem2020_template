#include "planet.h"

#include "text.h"
#include "assets.h"
#include "window.h"
#include "asteroid.h"
#include "collide.h"

const float size = 50.0;

Planet::Planet(int id, float orbit_radius, float orbit_offset, float mass, float health, float rps):
	id(id), orbit_radius(orbit_radius), mass(mass),
	health(health), CircleEntity(vec(), size),
	rps(rps), curr_angle(orbit_offset)
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
			health -= asteroid->mass;
			asteroid->alive = false;
		}
	}
}

void Planet::Draw() const
{
	float scale = size / 160;
	Window::Draw(Assets::backgroundTexture, pos)
		.withOrigin(Assets::backgroundTexture->w / 2, Assets::backgroundTexture->h / 2)
		.withScale(scale);

	Text txt_health(Assets::font_30);
	txt_health.setString(std::to_string(int(health)));
	Window::Draw(txt_health, pos + vec(0, size / 2.0 + 10.0)) 
		.withOrigin(txt_health.getSize().x, 0)
		.withScale(0.5f);

	if (Debug::Draw) {
		DrawBounds(100, 100, 255);
	}
}
