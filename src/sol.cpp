#include "sol.h"

#include "assets.h"
#include "asteroid.h"
#include "window.h"


const float collider_radius = 80.f;
const float sprite_scale = 1.2f;

Sol::Sol(vec pos)
  : CircleEntity(pos, collider_radius)
  , solAnim(AnimLib::BLACKHOLE)
{
}

void Sol::Update(float dt)
{
  solAnim.Update(dt);
  for (auto asteroid : Asteroid::GetAll()) {
    if (asteroid->bounds().Distance(bounds()) < 0) {
      vec ast_dir = asteroid->velocity.Normalized().RotatedAroundOrigin(-M_PI/10);
      asteroid->velocity += ast_dir * 100;
      asteroid->max_speed_mult = 1.5f;
    }
  }
}

void Sol::Draw() const
{
	const GPU_Rect& animRect = solAnim.GetCurrentRect();
	Window::Draw(Assets::solTexture, pos)
		.withRect(animRect)
		.withScale(sprite_scale)
		.withOrigin(vec(animRect.w, animRect.h) / 2);
}
