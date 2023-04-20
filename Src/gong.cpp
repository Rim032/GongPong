#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;

//Made by Rim032, 4/20/23

class GongPong : public olc::PixelGameEngine
{
public:
	GongPong()
	{
		sAppName = "GongPong";
	}
private:
	const float bar_speed = 3.5f;
	const float ball_size = 3.0f;
	const int bar_size = 20;

	short npc_score_counter = 0;
	short ply_score_counter = 0;
	short npc_mode = 1;

	olc::vf2d npc_pos = { 90.0f - bar_size / 2, 15.0f };
	olc::vf2d ply_pos = { 90.0f - bar_size / 2, 180.0f };

	olc::vf2d ball_pos = { 90.0f - ball_size / 2, 100.0f };
	olc::vf2d ball_vel = { 150.0f, 50.0f };

	void round_reset()
	{
		ply_pos = { 90.0f - bar_size / 2, 180.0f };
		npc_pos = { 90.0f - bar_size / 2, 15.0f };

		ball_pos = { 90.0f - ball_size - (rand() % 50), 100.0f - (rand() % 30)};
		ball_vel = { 150.0f + (rand() % 100), 50.0f + (rand() % 50) };
	}

	void user_controls()
	{
		if (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld)
		{
			if (ply_pos.x < (180.0f - bar_size))
			{
				ply_pos.x += bar_speed;
			}
		}

		if (ply_pos.x > 0.0f && GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld)
		{
			if (ply_pos.x > 0.0f)
			{
				ply_pos.x -= bar_speed;
			}
		}

		if (GetKey(olc::Key::R).bPressed)
		{
			round_reset();
			ply_score_counter = 0;
			npc_score_counter = 0;
			npc_mode = 1;
		}

		if (GetKey(olc::Key::M).bPressed)
		{
			if (npc_mode <= 3)
			{
				npc_mode++;
			}
			else
			{
				npc_mode = 1;
			}
		}
	}

	void primitive_npc_handle()
	{
		if (npc_mode < 4)
		{
			if (ball_pos.y <= 100.0f)
			{
				if (ball_pos.x > (npc_pos.x + (bar_size / 2)) && npc_pos.x < (180.0f - bar_size))
				{
					npc_pos.x += (bar_speed * 0.8f) * npc_mode;
				}
				else if (ball_pos.x < (npc_pos.x - (bar_size / 2)) && npc_pos.x > 0.0f)
				{
					npc_pos.x -= (bar_speed * 0.8f) * npc_mode;
				}
			}
		}
		else
		{
			npc_pos.x = ball_pos.x - (bar_size / 2); // :)
		}
	}

	void ball_world_collision_handle(float time)
	{
		ball_pos += ball_vel * time;
		if (ball_pos.x <= ball_size || ball_pos.x >= (180 - ball_size))
		{
			ball_vel.x *= -1.0f;
		}
		else if (ball_pos.y <= ball_size || ball_pos.y >= (200 - ball_size))
		{
			ball_vel.y *= -1.0f;
		}
	}

	void ball_bar_collision_handle(float time, olc::vf2d bar)
	{
		if (ball_pos.x >= bar.x - ball_size && ball_pos.x <= bar.x + (bar_size + ball_size))
		{
			if (ball_pos.y <= bar.y + (5 + ball_size) && ball_pos.y >= bar.y - ball_size)
			{
				ball_vel = { 310.0f * cos(bar.x), 110.0f * sin(bar.y) };
			}
		}
	}

	void ball_score_handle()
	{
		if (ball_pos.y >= 195.0f)
		{
			npc_score_counter++;
			round_reset();
		}
		else if(ball_pos.y <= 5.0f)
		{
			ply_score_counter++;
			round_reset();
		}
	}

	void draw_game()
	{
		FillRect(ply_pos.x, ply_pos.y, bar_size, 5, olc::WHITE);
		if (npc_mode == 4)
		{
			FillRect(npc_pos.x, npc_pos.y, bar_size, 5, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
		}
		else
		{
			FillRect(npc_pos.x, npc_pos.y, bar_size, 5, olc::WHITE);
		}

		FillCircle(ball_pos.x, ball_pos.y, ball_size, olc::WHITE);
		DrawLine(0, 103, 180, 103, olc::WHITE);

		DrawString(90, 110, to_string(ply_score_counter), olc::WHITE, 1);
		DrawString(90, 90, to_string(npc_score_counter), olc::WHITE, 1);
		DrawString(0, 0, "Diff.: " + to_string(npc_mode), olc::WHITE, 1);
	}

public:
	bool OnUserCreate() override
	{
		srand(32767);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		this_thread::sleep_for(5ms);

		ball_world_collision_handle(fElapsedTime);
		ball_bar_collision_handle(fElapsedTime, ply_pos);
		ball_bar_collision_handle(fElapsedTime, npc_pos);

		user_controls();
		primitive_npc_handle();

		ball_score_handle();
		draw_game();

		return true;
	}
};


int main()
{
	GongPong GongPong_app;
	if (GongPong_app.Construct(180, 200, 4, 4))
		GongPong_app.Start();

	return 0;
}
