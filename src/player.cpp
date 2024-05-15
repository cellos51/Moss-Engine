#include "player.hpp"

#include "event.hpp"
#include "Windows.h"
#include "console.hpp"

#include <cmath>

Player::Player()
{
	size = Vector2(24, 48);

	luminosity = Color4(0.0f, 0.0f, 0.0f, 0.0f);
	layer = 5;
	texturePos.w = 64;
	texturePos.h = 64;
	offset.w = 64;
	offset.h = 64;

	offset.x -= (offset.w - size.x) / 2;
	offset.y -= offset.h - size.y;

	footStep[0] = Mix_LoadWAV("assets/audio/walk1.wav");
	footStep[1] = Mix_LoadWAV("assets/audio/walk2.wav");
	footStep[2] = Mix_LoadWAV("assets/audio/walk3.wav");
	footStep[3] = Mix_LoadWAV("assets/audio/walk4.wav");
	footStep[4] = Mix_LoadWAV("assets/audio/walk5.wav");
	landing[0] = Mix_LoadWAV("assets/audio/land1.wav");
	landing[1] = Mix_LoadWAV("assets/audio/land2.wav");
	landing[2] = Mix_LoadWAV("assets/audio/land3.wav");
	jump[0] = Mix_LoadWAV("assets/audio/jump1.wav");
	jump[1] = Mix_LoadWAV("assets/audio/jump2.wav");
	slide = Mix_LoadWAV("assets/audio/sliding.wav");
	Mix_PlayChannel(6, slide, -1);
	Mix_Volume(6, 0);
}

Player::~Player()
{
	Mix_FreeChunk(footStep[0]);
	Mix_FreeChunk(footStep[1]);
	Mix_FreeChunk(footStep[2]);
	Mix_FreeChunk(footStep[3]);
	Mix_FreeChunk(footStep[4]);
	Mix_FreeChunk(landing[0]);
	Mix_FreeChunk(landing[1]);
	Mix_FreeChunk(landing[2]);
	Mix_FreeChunk(jump[0]);
	Mix_FreeChunk(jump[1]);
	Mix_FreeChunk(slide);
}

bool Player::onWall() const 
{
	return (walljumpTimeLeft == coyoteTime || walljumpTimeRight == coyoteTime);
}

void Player::update(double deltaTime)
{
	if (OnGround == true)
	{
		if (hitGround == false)
		{
			Mix_Volume(5, cachedVelocity.y * 2);
			Mix_PlayChannel(5, landing[rand() % 3], 0);
			hitGround = true;
		}

		if (std::abs(velocity.x) < 0.1)
		{
			elapsedTime += deltaTime / 200.0f;
			texturePos.x = (mirror + int(elapsedTime) % 8) * 64;
			texturePos.y = 64;
			resetAnimation = true;
			rotation = 0;
		}
		else
		{
			rotation = 0;

			if (resetAnimation == true)
			{
				resetAnimation = false;
				elapsedTime = 0;
			}

			elapsedTime += deltaTime / 100.0f * std::abs(velocity.x);
			texturePos.x = (mirror + int(elapsedTime) % 12) * 64;
			texturePos.y = 0;

			if (texturePos.x == 5 * 64 || texturePos.x == 10 * 64)
			{
				if (stepped == false)
				{
					stepped = true;
					int channel = rand() % 5;
					Mix_Volume(channel, std::abs(velocity.x) * 2);
					Mix_PlayChannel(channel, footStep[channel], 0);
				}
			}
			else
			{
				stepped = false;
			}
		}
	}
	else
	{
		cachedVelocity.y = velocity.y;

		hitGround = false;

		texturePos.y = 64;
		if (walljumpTimeLeft == coyoteTime || walljumpTimeRight == coyoteTime)
		{
			rotation = 0.0f;
			if (velocity.y < 0)
			{
				texturePos.x = (mirror + 10) * 64;
			}
			else
			{
				texturePos.x = (mirror + 11) * 64;
			}		
		}
		else if (velocity.y < 0 )
		{
			texturePos.x = (mirror + 8) * 64;
			rotation = std::lerp(rotation, -velocity.x, deltaTime * 0.005);
		}
		else
		{
			texturePos.x = (mirror + 9) * 64;
			rotation = std::lerp(rotation, velocity.x, deltaTime * 0.005);
		}
		resetAnimation = true;
	}

	//rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.05); // cool velocity based rotation but it doesn't look too good with pixel art
}

void Player::fixedUpdate(double deltaTime)
{
	bool jumpButton = Event::KeyPressed(SDLK_SPACE) || Event::ButtonPressed(SDL_CONTROLLER_BUTTON_A);

	float moveAxis = 0;
	moveAxis += (Event::KeyPressed(SDLK_a) == true) ? 1.0f : 0.0f;
	moveAxis += (Event::KeyPressed(SDLK_d) == true) ? -1.0f : 0.0f;
	moveAxis = (Event::JoyAxis(SDL_CONTROLLER_AXIS_LEFTX) != 0.0f) ? Event::JoyAxis(SDL_CONTROLLER_AXIS_LEFTX) : moveAxis;

	// movement

	float moveDir = 0.0f;

	float speed = 0;

	if (OnGround == true)
	{
		walljumpTimeLeft = 0.0f;
		walljumpTimeRight = 0.0f;

		airTime = coyoteTime;

		speed = groundSpeed;
		dragX = 0.04f;

		if (moveAxis == 0.0f)
		{
			dragX = 0.3f;
		}
	}
	else
	{
		airTime -= deltaTime;

		speed = airSpeed;
		dragX = 0.03f;
	}


	if (moveAxis < 0)
	{
		moveDir += speed * -moveAxis;

		if (velocity.x < 0 && OnGround == true) // this lets you stop on a DIME (not really a dime but you get the idea)
		{
			dragX = 0.3f;
		}
	}

	if (moveAxis > 0)
	{
		moveDir -= speed * moveAxis;

		if (velocity.x > 0 && OnGround == true)
		{
			dragX = 0.3f;
		}
	}

	// jumping and wall jumping

	dragY = 0.01f;
	cachedVelocity.x -= (cachedVelocity.x > 0) ? deltaTime / 100 : 0.0f;
	walljumpTimeLeft -= (walljumpTimeLeft > 0) ? deltaTime : 0.0f;
	walljumpTimeRight -= (walljumpTimeRight > 0) ? deltaTime : 0.0f;

	if (((leftTouch == true && moveDir < 0.0f) || (rightTouch == true && moveDir > 0.0f)) && OnGround == false)
	{
		if (velocity.y > 0.0f)
		{
			dragY = 0.1f;
		}

		Mix_Volume(6, std::abs(velocity.y) * 20);

		if (leftTouch == true)
		{
			walljumpTimeLeft = coyoteTime;
		}
		else if (rightTouch == true)
		{
			walljumpTimeRight = coyoteTime;
		}
	}
	else
	{
		Mix_Volume(6, 0);
		cachedVelocity.x = std::abs(velocity.x);
	}

	if (jumpButton == true && pressingJump == false)
	{
		pressingJump = true;
		jumpTime = jumpBuffer;
	}
	else
	{
		jumpTime -= (jumpTime > 0) ? deltaTime : 0.0f;

		if (jumpButton == false)
		{
			pressingJump = false;
		}
	}

	if (jumpTime > 0 && (airTime > 0 || walljumpTimeLeft > 0 || walljumpTimeRight > 0))
	{
		if (walljumpTimeLeft > 0 && walljumpTimeLeft > walljumpTimeRight)
		{
			velocity.x += 5 + cachedVelocity.x / 2;
			velocity.y -= jumpForce;

			Mix_Volume(5, 15);
			Mix_PlayChannel(5, jump[rand() % 2], 0);
		}
		else if (walljumpTimeRight > 0 && walljumpTimeRight > walljumpTimeLeft)
		{
			velocity.x -= 5 + cachedVelocity.x / 2;
			velocity.y -= jumpForce;

			Mix_Volume(5, 15);
			Mix_PlayChannel(5, jump[rand() % 2], 0);
		}
		else if (airTime > 0)
		{
			velocity.y -= jumpForce;

			Mix_Volume(5, 15);
			Mix_PlayChannel(5, jump[rand() % 2], 0);
		}
		walljumpTimeLeft = 0.0f;
		walljumpTimeRight = 0.0f;
		airTime = 0.0f;
		jumpTime = 0.0f;
	}
	else if (jumpButton == false && !OnGround && velocity.y < 0)
	{
		velocity.y = std::lerp(velocity.y, 0.0f, deltaTime * 0.05);
	}
	// end of jumping

	velocity.x += moveDir * deltaTime;

	// animation
	if (moveAxis < 0)
	{
		texturePos.w = 64;
		mirror = 0;
	}
	else if (moveAxis > 0)
	{
		texturePos.w = -64;
		mirror = 1;
	}

	PhysicsEntity::physics(phys, deltaTime);
}

NetPlayer::NetPlayer()
{
	size = Vector2(24, 48);

	luminosity = Color4(0.0f, 0.0f, 0.0f, 0.0f);
	layer = 5;
	texturePos.w = 64;
	texturePos.h = 64;
	offset.w = 64;
	offset.h = 64;

	offset.x -= (offset.w - size.x) / 2;
	offset.y -= offset.h - size.y;

	/*footStep[0] = Mix_LoadWAV("assets/audio/walk1.wav");
	footStep[1] = Mix_LoadWAV("assets/audio/walk2.wav");
	footStep[2] = Mix_LoadWAV("assets/audio/walk3.wav");
	footStep[3] = Mix_LoadWAV("assets/audio/walk4.wav");
	footStep[4] = Mix_LoadWAV("assets/audio/walk5.wav");
	landing[0] = Mix_LoadWAV("assets/audio/land1.wav");
	landing[1] = Mix_LoadWAV("assets/audio/land2.wav");
	landing[2] = Mix_LoadWAV("assets/audio/land3.wav");
	jump[0] = Mix_LoadWAV("assets/audio/jump1.wav");
	jump[1] = Mix_LoadWAV("assets/audio/jump2.wav");
	slide = Mix_LoadWAV("assets/audio/sliding.wav");
	Mix_PlayChannel(6, slide, -1);
	Mix_Volume(6, 0);*/
}

NetPlayer::~NetPlayer()
{
	/*Mix_FreeChunk(footStep[0]);
	Mix_FreeChunk(footStep[1]);
	Mix_FreeChunk(footStep[2]);
	Mix_FreeChunk(footStep[3]);
	Mix_FreeChunk(footStep[4]);
	Mix_FreeChunk(landing[0]);
	Mix_FreeChunk(landing[1]);
	Mix_FreeChunk(landing[2]);
	Mix_FreeChunk(jump[0]);
	Mix_FreeChunk(jump[1]);
	Mix_FreeChunk(slide);*/
}

void NetPlayer::update()
{
	transform = Vector2(std::lerp(transform.x, targetPos.x, Time::deltaTime() * lerpSpeed), std::lerp(transform.y, targetPos.y, Time::deltaTime() * lerpSpeed));

	// animation
	if (velocity.x > 0)
	{
		texturePos.w = 64;
		mirror = 0;
	}
	else if (velocity.x < 0)
	{
		texturePos.w = -64;
		mirror = 1;
	}

	if (OnGround == true)
	{
		/*if (hitGround == false)
		{
			Mix_Volume(5, velocity.y * 2);
			Mix_PlayChannel(5, landing[rand() % 3], 0);
			hitGround = true;
		}*/

		if (std::abs(velocity.x) < 0.1)
		{
			elapsedTime += Time::deltaTime() / 200.0f;
			texturePos.x = (mirror + int(elapsedTime) % 8) * 64;
			texturePos.y = 64;
			resetAnimation = true;
			rotation = 0;
		}
		else
		{
			rotation = 0;

			if (resetAnimation == true)
			{
				resetAnimation = false;
				elapsedTime = 0;
			}

			elapsedTime += Time::deltaTime() / 100.0f * std::abs(velocity.x);
			texturePos.x = (mirror + int(elapsedTime) % 12) * 64;
			texturePos.y = 0;

			/*if (texturePos.x == 5 * 64 || texturePos.x == 10 * 64)
			{
				if (stepped == false)
				{
					stepped = true;
					int channel = rand() % 5;
					Mix_Volume(channel, std::abs(velocity.x) * 2);
					Mix_PlayChannel(channel, footStep[channel], 0);
				}
			}
			else
			{
				stepped = false;
			}*/
		}
	}
	else
	{
		hitGround = false;

		texturePos.y = 64;
		if (walljumping == true)
		{
			rotation = 0.0f;
			if (velocity.y < 0)
			{
				texturePos.x = (mirror + 10) * 64;
			}
			else
			{
				texturePos.x = (mirror + 11) * 64;
			}
		}
		else if (velocity.y < 0)
		{
			texturePos.x = (mirror + 8) * 64;
			rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.005);
		}
		else
		{
			texturePos.x = (mirror + 9) * 64;
			rotation = std::lerp(rotation, velocity.x, Time::deltaTime() * 0.005);
		}
		resetAnimation = true;
	}
}