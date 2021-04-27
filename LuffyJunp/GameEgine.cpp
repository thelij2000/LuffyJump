#include "GameEgine.h"

std::string GetHighScoreFromFile(std::string path)		//get high score in file 
{
	std::fstream HighScoreFile;
	std::string highscore;

	HighScoreFile.open(path, std::ios::in);
	HighScoreFile >> highscore;

	return highscore;
}

void UpdateHighScore(std::string path,		//check score and note in file
	const int& score,
	const std::string& old_high_score)
{
	int oldHighScore = 0;
	std::fstream HighScoreFile;
	std::string newHighScore;
	std::stringstream ConvertToInt(old_high_score);

	HighScoreFile.open(path, std::ios::out);

	ConvertToInt >> oldHighScore;
	if (score > oldHighScore)
	{
		oldHighScore = score;
	}
	newHighScore = std::to_string(oldHighScore);

	HighScoreFile << newHighScore;
}

int UpdateGameTimeAndScore(int& time,
	int& speed,
	int& score)
{
	if (time == TIME_MAX)					//each 1000 point increase speed
	{
		speed += SPEED_INCREASEMENT;
	}

	if (time > TIME_MAX)					//reset time value
	{
		time = 0;
	}
	if (time % 5 == 0)						//increase score each 5 time
	{
		score += SCORE_INCREASEMENT;
	}

	time += TIME_INCREASEMENT;

	return time;
}

void RenderScrollingBackground(std::vector <double>& offsetSpeed,
	LoadTexture(&gBackgroundTexture)[BACKGROUND_LAYER],
	SDL_Renderer* gRenderer)
{																	//background move by part to layer
	std::vector <double> layer_speed;
	layer_speed.push_back(LAYER_1_SPEED);
	layer_speed.push_back(LAYER_2_SPEED);

	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		offsetSpeed[i] -= layer_speed[i];
		if (offsetSpeed[i] < -gBackgroundTexture[i].GetWidth())
		{
			offsetSpeed[i] = 0;
		}
		gBackgroundTexture[i].Render(offsetSpeed[i], 0, gRenderer);
		gBackgroundTexture[i].Render(offsetSpeed[i] + gBackgroundTexture[i].GetWidth(), 0, gRenderer);
	}
}

void RenderScrollingGround(int& speed,
	const int acceleration,
	LoadTexture gGroundTexture,
	SDL_Renderer* gRenderer)					//move ground
{
	speed -= GROUND_SPEED + acceleration;
	if (speed < -gGroundTexture.GetWidth())
	{
		speed = 0;
	}
	gGroundTexture.Render(speed, 0, gRenderer);
	gGroundTexture.Render(speed + gGroundTexture.GetWidth(), 0, gRenderer);
}

void HandlePlayButton(SDL_Event* e,
	Button& PlayButton,
	bool& QuitMenu,
	bool& Play,
	Mix_Chunk* gClick)
{
	if (e->type == SDL_QUIT)
	{
		QuitMenu = true;
	}

	if (PlayButton.IsInside(e, COMMON_BUTTON))
	{
		switch (e->type)
		{
		case SDL_MOUSEMOTION:
			PlayButton.currentSprite = BUTTON_MOUSE_OVER;
			break;
		case SDL_MOUSEBUTTONDOWN:
			Play = true;
			QuitMenu = true;
			Mix_PlayChannel(-1, gClick, 0);
			PlayButton.currentSprite = BUTTON_MOUSE_OVER;
			break;
		}
	}
	else
	{
		PlayButton.currentSprite = BUTTON_MOUSE_OUT;
	}
}

void HandleHelpButton(SDL_Event* e,
	SDL_Rect(&gBackButton)[BUTTON_TOTAL],
	Button& HelpButton,
	Button& BackButton,
	LoadTexture gInstructionTexture,
	LoadTexture gBackButtonTexture,
	SDL_Renderer* gRenderer,
	bool& Quit_game,
	Mix_Chunk* gClick)
{
	if (HelpButton.IsInside(e, COMMON_BUTTON))
	{
		switch (e->type)
		{
		case SDL_MOUSEMOTION:
			HelpButton.currentSprite = BUTTON_MOUSE_OVER;
			break;
		case SDL_MOUSEBUTTONDOWN:
			HelpButton.currentSprite = BUTTON_MOUSE_OVER;
			Mix_PlayChannel(-1, gClick, NOT_REPEATITIVE);

			bool ReadDone = false;
			while (!ReadDone)
			{
				do
				{
					if (e->type == SDL_QUIT)
					{
						ReadDone = true;
						Quit_game = true;
						Close();
					}

					else if (BackButton.IsInside(e, COMMON_BUTTON))
					{
						switch (e->type)
						{
						case SDL_MOUSEMOTION:
							BackButton.currentSprite = BUTTON_MOUSE_OVER;
							break;
						case SDL_MOUSEBUTTONDOWN:
							BackButton.currentSprite = BUTTON_MOUSE_OVER;
							Mix_PlayChannel(-1, gClick, NOT_REPEATITIVE);
							ReadDone = true;
							break;
						}
					}
					else
					{
						BackButton.currentSprite = BUTTON_MOUSE_OUT;
					}

					gInstructionTexture.Render(0, 0, gRenderer);

					SDL_Rect* currentClip_Back = &gBackButton[BackButton.currentSprite];
					BackButton.Render(currentClip_Back, gRenderer, gBackButtonTexture);

					SDL_RenderPresent(gRenderer);
				} while (SDL_PollEvent(e) != 0 && e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION);
			}
			break;
		}
	}
	else
	{
		HelpButton.currentSprite = BUTTON_MOUSE_OUT;
	}
}

void HandleExitButton(SDL_Event* e,
	Button& ExitButton,
	bool& Quit,
	Mix_Chunk* gClick)
{
	if (ExitButton.IsInside(e, COMMON_BUTTON))
	{
		switch (e->type)
		{
		case SDL_MOUSEMOTION:
			ExitButton.currentSprite = BUTTON_MOUSE_OVER;
			break;
		case SDL_MOUSEBUTTONDOWN:
			Quit = true;
			ExitButton.currentSprite = BUTTON_MOUSE_OVER;
			Mix_PlayChannel(-1, gClick, NOT_REPEATITIVE);
			break;
		}
	}
	else
	{
		ExitButton.currentSprite = BUTTON_MOUSE_OUT;
	}
}

void HandleContinueButton(Button ContinueButton,
	LoadTexture gContinueButtonTexture,
	SDL_Event* e,
	SDL_Renderer* gRenderer,
	SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
	bool& Game_State,
	Mix_Chunk* gClick)
{
	bool Back_To_Game = false;
	while (!Back_To_Game)
	{
		do
		{
			if (ContinueButton.IsInside(e, SMALL_BUTTON))
			{
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					ContinueButton.currentSprite = BUTTON_MOUSE_OVER;
					break;
				case SDL_MOUSEBUTTONDOWN:
				{
					ContinueButton.currentSprite = BUTTON_MOUSE_OVER;
					Mix_PlayChannel(-1, gClick, NOT_REPEATITIVE);
					Mix_ResumeMusic();
					Game_State = true;
					Back_To_Game = true;
				}
				break;
				}
			}
			else
			{
				ContinueButton.currentSprite = BUTTON_MOUSE_OUT;
			}

			SDL_Rect* currentClip_Continue = &gContinueButton[ContinueButton.currentSprite];
			ContinueButton.Render(currentClip_Continue, gRenderer, gContinueButtonTexture);

			SDL_RenderPresent(gRenderer);
		} while (SDL_WaitEvent(e) != 0 && e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION);
	}
}

void HandlePauseButton(SDL_Event* e,
	SDL_Renderer* gRenderer,
	SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
	Button& PauseButton,
	Button ContinueButton,
	LoadTexture gContinueButtonTexture,
	bool& Game_State,
	Mix_Chunk* gClick)
{
	if (PauseButton.IsInside(e, SMALL_BUTTON))
	{
		switch (e->type)
		{
		case SDL_MOUSEMOTION:
			PauseButton.currentSprite = BUTTON_MOUSE_OVER;
			break;
		case SDL_MOUSEBUTTONDOWN:
			PauseButton.currentSprite = BUTTON_MOUSE_OVER;
			Mix_PlayChannel(-1, gClick, NOT_REPEATITIVE);
			Mix_PauseMusic();
			break;
		case SDL_MOUSEBUTTONUP:
			Game_State = false;
			HandleContinueButton(ContinueButton, gContinueButtonTexture, e, gRenderer, gContinueButton, Game_State, gClick);
			break;
		}
	}
	else
	{
		PauseButton.currentSprite = BUTTON_MOUSE_OUT;
	}
}

void GenerateEnemy(Enemy& enemy1,
	Enemy& enemy2,
	Enemy& enemy3,
	SDL_Rect(&gEnemyClips)[FLYING_FRAMES],
	SDL_Renderer* gRenderer)
{														//Load enemy
	enemy1.LoadFromFile("imgs/enemy/lucy.png", gRenderer);
	enemy2.LoadFromFile("imgs/enemy/naruto.png", gRenderer);
	enemy3.LoadFromFile("imgs/enemy/shuriken.png", gRenderer);
	{
		gEnemyClips[0].x = 0;
		gEnemyClips[0].y = 0;
		gEnemyClips[0].w = 26;
		gEnemyClips[0].h = 29;

		gEnemyClips[1].x = 26;
		gEnemyClips[1].y = 0;
		gEnemyClips[1].w = 29;
		gEnemyClips[1].h = 28;

		gEnemyClips[2].x = 84;
		gEnemyClips[2].y = 0;
		gEnemyClips[2].w = 26;
		gEnemyClips[2].h = 29;

		gEnemyClips[3].x = 55;
		gEnemyClips[3].y = 0;
		gEnemyClips[3].w = 29;
		gEnemyClips[3].h = 28;
	}
}

bool CheckColission(Character character,
	SDL_Rect* char_clip,
	Enemy enemy,
	SDL_Rect* enemy_clip)
{
	bool collide = false;

	int left_a = character.GetPosX();
	int right_a = character.GetPosX() + char_clip->w;
	int top_a = character.GetPosY();
	int bottom_a = character.GetPosY() + char_clip->h;

	if (enemy.GetType() == ON_GROUND_ENEMY)
	{

		int left_b = enemy.GetPosX();									
		int right_b = enemy.GetPosX() + enemy.GetWidth();
		int top_b = enemy.GetPosY();
		int bottom_b = enemy.GetPosY() + enemy.GetHeight();

		if (right_a >= left_b + 8 && left_a <= right_b - 8)	//Check if collission
		{
			if (bottom_a >= top_b)
			{
				collide = true;
			}
		}
	}
	else
	{

		int left_b = enemy.GetPosX();
		int right_b = enemy.GetPosX() + enemy_clip->w;
		int top_b = enemy.GetPosY();
		int bottom_b = enemy.GetPosY() + enemy_clip->h;

		if (right_a >= left_b && left_a <= right_b)		//check if collision
		{
			if (top_a <= bottom_b && top_a >= top_b)
			{
				collide = true;
			}

			if (bottom_a >= bottom_b && bottom_a <= top_b)
			{
				collide = true;
			}
		}
	}

	return collide;
}

bool CheckEnemyColission(Character character,
	Enemy enemy1,
	Enemy enemy2,
	Enemy enemy3,
	SDL_Rect* char_clip,
	SDL_Rect* enemy_clip
)
{
	if (CheckColission(character, char_clip, enemy1))
	{
		return true;
	}
	if (CheckColission(character, char_clip, enemy2))
	{
		return true;
	}
	if (CheckColission(character, char_clip, enemy3, enemy_clip))
	{
		return true;
	}
	return false;
}

void ControlCharFrame(int& frame)
{
	frame += FRAME_INCREASEMENT;
	if (frame / SLOW_FRAME_CHAR >= RUNNING_FRAMES)			//check and reset frame
	{
		frame = 0;
	}
}

void ControlEnemyFrame(int& frame)							//check and reset frame
{
	frame += FRAME_INCREASEMENT;
	if (frame / SLOW_FRAME_ENEMY >= FLYING_FRAMES)
	{
		frame = 0;
	}
}

void DrawPlayerScore(LoadTexture gTextTexture,
	LoadTexture gScoreTexture,
	SDL_Color textColor,
	SDL_Renderer* gRenderer,
	TTF_Font* gFont,
	const int& score)
{
	gTextTexture.Render(TEXT_1_POSX, TEXT_1_POSY, gRenderer);
	if (gScoreTexture.LoadFromRenderedText(std::to_string(score), gFont, textColor, gRenderer))
	{
		gScoreTexture.Render(SCORE_POSX, SCORE_POSY, gRenderer);
	}
}

void DrawPlayerHighScore(LoadTexture gTextTexture,
	LoadTexture gHighScoreTexture,
	SDL_Color textColor,
	SDL_Renderer* gRenderer,
	TTF_Font* gFont,
	const std::string& HighScore)
{
	gTextTexture.Render(TEXT_2_POSX, TEXT_2_POSY, gRenderer);
	if (gHighScoreTexture.LoadFromRenderedText(HighScore, gFont, textColor, gRenderer))
	{
		gHighScoreTexture.Render(HIGH_SCORE_POSX, HIGH_SCORE_POSY, gRenderer);
	}
}

void DrawEndGameSelection(LoadTexture gLoseTexture,
	SDL_Event* e,
	SDL_Renderer* gRenderer,
	bool& Play_Again)
{
	if (Play_Again)
	{
		bool End_Game = false;
		while (!End_Game)
		{
			while (SDL_PollEvent(e) != 0)
			{
				if (e->type == SDL_QUIT)
				{
					Play_Again = false;
				}

				if (e->type == SDL_KEYDOWN)
				{
					switch (e->key.keysym.sym)
					{
					case SDLK_SPACE:
						End_Game = true;
						break;
					case SDLK_ESCAPE:
						End_Game = true;
						Play_Again = false;
						break;
					}
				}
			}

			gLoseTexture.Render(0, 0, gRenderer);

			SDL_RenderPresent(gRenderer);
		}
	}
}
