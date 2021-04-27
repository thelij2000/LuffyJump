#include "Base.h"
#include "GameEgine.h"
#include "LoadTexture.h"
#include "Button.h"
#include "Character.h"
#include "Enemy.h"

const std::string LAYER[BACKGROUND_LAYER] = {
	"imgs/background/layer01.png",
	"imgs/background/layer02.png",
};

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Color textColor = { 0, 0, 0 };
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

SDL_Rect gPlayButton[BUTTON_TOTAL];
SDL_Rect gHelpButton[BUTTON_TOTAL];
SDL_Rect gExitButton[BUTTON_TOTAL];
SDL_Rect gBackButton[BUTTON_TOTAL];
SDL_Rect gPauseButton[BUTTON_TOTAL];
SDL_Rect gContinueButton[BUTTON_TOTAL];
SDL_Rect gPlayAgainButton[BUTTON_TOTAL];
SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];
SDL_Rect gCharacterJumpClips;

LoadTexture gCharacterJumpTexture;
LoadTexture gMenuTexture;
LoadTexture gInstructionTexture;
LoadTexture gBackgroundTexture[BACKGROUND_LAYER];
LoadTexture gCharacterTexture;
LoadTexture gGroundTexture;
LoadTexture gPlayButtonTexture;
LoadTexture gHelpButtonTexture;
LoadTexture gExitButtonTexture;
LoadTexture gBackButtonTexture;
LoadTexture gPauseButtonTexture;
LoadTexture gContinueButtonTexture;
LoadTexture gLoseTexture;
LoadTexture gText1Texture;
LoadTexture gScoreTexture;
LoadTexture gText2Texture;
LoadTexture gHighScoreTexture;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button HelpButton(HELP_BUTTON_POSX, HELP_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button PauseButton(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY);
Button ContinueButton(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY);

Character character;

int main(int argc, char* argv[])
{
	if (!Init())						//check initialize
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!LoadMedia())			//check Func Loadmedia
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool Quit_Menu = false;
			bool Play_Again = false;

			Mix_PlayMusic(gMenuMusic, REPEATITIVE);
			while (!Quit_Menu)										//Repeat menu until exit
			{
				SDL_Event e_mouse;
				while (SDL_PollEvent(&e_mouse) != 0)								//Check mouse event
				{
					if (e_mouse.type == SDL_QUIT)
					{
						Quit_Menu = true;
					}														

					bool Quit_Game = false;
					HandlePlayButton(&e_mouse, PlayButton, Quit_Menu, Play_Again, gClick);

					HandleHelpButton(&e_mouse, gBackButton,
						HelpButton, BackButton,
						gInstructionTexture, gBackButtonTexture,
						gRenderer, Quit_Game, gClick);

					HandleExitButton(&e_mouse, ExitButton, Quit_Menu, gClick);		

					if (Quit_Game == true)
					{
						return 0;
					}
				}

				gMenuTexture.Render(0, 0, gRenderer);

				SDL_Rect* currentClip_Play = &gPlayButton[PlayButton.currentSprite];		//check mouse on or outside button
				PlayButton.Render(currentClip_Play, gRenderer, gPlayButtonTexture);

				SDL_Rect* currentClip_Help = &gHelpButton[HelpButton.currentSprite];
				HelpButton.Render(currentClip_Help, gRenderer, gHelpButtonTexture);

				SDL_Rect* currentClip_Exit = &gExitButton[ExitButton.currentSprite];
				ExitButton.Render(currentClip_Exit, gRenderer, gExitButtonTexture);

				SDL_RenderPresent(gRenderer);						//Update Render
			}

			while (Play_Again)
			{
				srand(time(NULL));
				int time = 0;
				int score = 0;
				int acceleration = 0;
				int frame_Character = 0;
				int frame_Enemy = 0;

				std::string highscore = GetHighScoreFromFile("high_score.txt");

				SDL_Event e;
				Enemy enemy1(ON_GROUND_ENEMY);
				Enemy enemy2(ON_GROUND_ENEMY);
				Enemy enemy3(IN_AIR_ENEMY);

				Mix_PlayMusic(gMusic, REPEATITIVE);
				Mix_ResumeMusic();
				GenerateEnemy(enemy1, enemy2, enemy3, gEnemyClips, gRenderer);

				int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
				std::vector <double> OffsetSpeed_Bkgr(BACKGROUND_LAYER, BASE_OFFSET_SPEED);

				bool Quit = false;
				bool Game_State = true;
				while (!Quit)
				{
					if (Game_State)
					{
						UpdateGameTimeAndScore(time, acceleration, score);

						while (SDL_PollEvent(&e) != 0)			//Play until quit
						{
							if (e.type == SDL_QUIT)
							{
								Quit = true;
								Play_Again = false;
							}

							HandlePauseButton(&e, gRenderer, gContinueButton,
								PauseButton, ContinueButton,
								gContinueButtonTexture, Game_State, gClick);

							character.HandleEvent(e, gJump);
						}
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						RenderScrollingBackground(OffsetSpeed_Bkgr, gBackgroundTexture, gRenderer);
						RenderScrollingGround(OffsetSpeed_Ground, acceleration, gGroundTexture, gRenderer);


						character.Move();
						SDL_Rect* currentClip_Character = nullptr;							//Render character move animation
						if (character.OnGround())
						{
							currentClip_Character = &gCharacterClips[frame_Character / SLOW_FRAME_CHAR];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}
						else
						{
							currentClip_Character = &gCharacterJumpClips;
							character.Render(currentClip_Character, gRenderer, gCharacterJumpTexture);
						}


						enemy1.Move(acceleration);
						enemy1.Render(gRenderer);

						enemy2.Move(acceleration);
						enemy2.Render(gRenderer);

						SDL_Rect* currentClip_Enemy = &gEnemyClips[frame_Enemy / SLOW_FRAME_ENEMY];
						enemy3.Move(acceleration);
						enemy3.Render(gRenderer, currentClip_Enemy);


						SDL_Rect* currentClip_Pause = &gPauseButton[PauseButton.currentSprite];
						PauseButton.Render(currentClip_Pause, gRenderer, gPauseButtonTexture);


						DrawPlayerScore(gText1Texture, gScoreTexture, textColor, gRenderer, gFont, score);
						DrawPlayerHighScore(gText2Texture, gHighScoreTexture, textColor, gRenderer, gFont, highscore);

						if (CheckEnemyColission(character,					//If collission pause mucsic, update score
							enemy1, enemy2, enemy3,
							currentClip_Character, currentClip_Enemy))
						{
							Mix_PauseMusic();
							Mix_PlayChannel(-1, gLose, NOT_REPEATITIVE);
							UpdateHighScore("high_score.txt", score, highscore);
							Quit = true;
						}


						SDL_RenderPresent(gRenderer);

						ControlCharFrame(frame_Character);
						ControlEnemyFrame(frame_Enemy);
					}
				}

				DrawEndGameSelection(gLoseTexture, &e, gRenderer, Play_Again);
				if (!Play_Again)										//if dont play again free all
				{
					enemy1.~Enemy();
					enemy2.~Enemy();
					enemy3.~Enemy();
				}
			}
		}
	}
	Close();

	return 0;
}


bool Init()						//initialize
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO) < 0)
	{
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))		//set scale and quality
		{
			std::cout << "Warning: Linear texture filtering not enabled!";
		}

		gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,		//creat window tile
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	//render on screen
			if (gRenderer == NULL)
			{
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);		//

				int imgFlags = IMG_INIT_PNG;									//create flags for picture
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)		//set audio 44100Hz, 2Mb
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}

				if (TTF_Init() == -1)											//check ttf is opened
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadMedia()
{
	bool success = true;

	gMusic = Mix_LoadMUS("sound/bkgr_audio.mp3");
	if (gMusic == nullptr)
	{
		success = false;
	}

	gMenuMusic = Mix_LoadMUS("sound/menu_audio.mp3");
	if (gMenuMusic == nullptr)
	{
		success = false;
	}

	gClick = Mix_LoadWAV("sound/mouse_click.wav");
	if (gClick == nullptr)
	{
		success = false;
	}

	gJump = Mix_LoadWAV("sound/jump_sound.mp3");
	if (gJump == nullptr)
	{
		success = false;
	}

	gLose = Mix_LoadWAV("sound/die_sound.mp3");
	if (gLose == nullptr)
	{
		success = false;
	}

	else
	{
		gFont = TTF_OpenFont("font/font.TTF", 14);
		if (gFont == NULL)
		{
			success = false;
		}
		else
		{
			if (!gText1Texture.LoadFromRenderedText("Your score: ", gFont, textColor, gRenderer))
			{
				std::cout << "Failed to render text1 texture" << std::endl;
				success = false;
			}

			if (!gText2Texture.LoadFromRenderedText("High score: ", gFont, textColor, gRenderer))
			{
				std::cout << "Failed to render text2 texture" << std::endl;
				success = false;
			}

			if (!gMenuTexture.LoadFromFile("imgs/background/menu.png", gRenderer))
			{
				std::cout << "Failed to load menu image" << std::endl;
				success = false;
			}

			if (!gInstructionTexture.LoadFromFile("imgs/background/instruction.png", gRenderer))
			{
				std::cout << "Failed to load instruction image" << std::endl;
				success = false;
			}

			if (!gPlayButtonTexture.LoadFromFile("imgs/button/big_button/play_button.png", gRenderer))
			{
				std::cout << "Failed to load play_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gPlayButton[i].x = 150 * i;
					gPlayButton[i].y = 0;
					gPlayButton[i].w = 150;
					gPlayButton[i].h = 98;
				}
			}

			if (!gHelpButtonTexture.LoadFromFile("imgs/button/big_button/help_button.png", gRenderer))
			{
				std::cout << "Failed to load help_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gHelpButton[i].x = 150 * i;
					gHelpButton[i].y = 0;
					gHelpButton[i].w = 150;
					gHelpButton[i].h = 98;
				}
			}

			if (!gBackButtonTexture.LoadFromFile("imgs/button/big_button/back_button.png", gRenderer))
			{
				std::cout << "Failed to load back_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gBackButton[i].x = 100 * i;
					gBackButton[i].y = 0;
					gBackButton[i].w = 100;
					gBackButton[i].h = 78;
				}
			}

			if (!gExitButtonTexture.LoadFromFile("imgs/button/big_button/exit_button.png", gRenderer))
			{
				std::cout << "Failed to load exit_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gExitButton[i].x = 150 * i;
					gExitButton[i].y = 0;
					gExitButton[i].w = 150;
					gExitButton[i].h = 98;
				}
			}

			if (!gPauseButtonTexture.LoadFromFile("imgs/button/big_button/pause_button.png", gRenderer))
			{
				std::cout << "Failed to load pause_button image " << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gPauseButton[i].x = 22 * i;
					gPauseButton[i].y = 0;
					gPauseButton[i].w = 22;
					gPauseButton[i].h = 34;
				}
			}

			if (!gContinueButtonTexture.LoadFromFile("imgs/button/big_button/continue_button.png", gRenderer))
			{
				std::cout << "Failed to load continue_button image " << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gContinueButton[i].x = 22 * i;
					gContinueButton[i].y = 0;
					gContinueButton[i].w = 22;
					gContinueButton[i].h = 34;
				}
			}

			for (int i = 0; i < BACKGROUND_LAYER; ++i)
			{
				if (!gBackgroundTexture[i].LoadFromFile(LAYER[i].c_str(), gRenderer))
				{
					std::cout << "Failed to load background image" << std::endl;
					success = false;
				}
			}

			if (!gGroundTexture.LoadFromFile("imgs/background/ground.png", gRenderer))
			{
				std::cout << "Failed to load ground image" << std::endl;
				success = false;
			}

			if (!gCharacterJumpTexture.LoadFromFile("imgs/character/jump.png", gRenderer))
			{
				std::cout << "Failed to load character image" << std::endl;
				success = false;
			}
			else
			{
				gCharacterJumpClips.x = 0;
				gCharacterJumpClips.y = 0;
				gCharacterJumpClips.w = 36;
				gCharacterJumpClips.h = 57;
			}

			if (!gCharacterTexture.LoadFromFile("imgs/character/kaerecto.png", gRenderer))
			{
				std::cout << "Failed to load character_run image." << std::endl;
				success = false;
			}
			else
			{
				gCharacterClips[0].x = 1;
				gCharacterClips[0].y = 0;
				gCharacterClips[0].w = 42;
				gCharacterClips[0].h = 57;

				gCharacterClips[1].x = 51;
				gCharacterClips[1].y = 0;
				gCharacterClips[1].w = 55;
				gCharacterClips[1].h = 57;

				gCharacterClips[2].x = 113;
				gCharacterClips[2].y = 0;
				gCharacterClips[2].w = 44;
				gCharacterClips[2].h = 57;

				gCharacterClips[3].x = 162;
				gCharacterClips[3].y = 0;
				gCharacterClips[3].w = 37;
				gCharacterClips[3].h = 57;

				gCharacterClips[4].x = 203;
				gCharacterClips[4].y = 0;
				gCharacterClips[4].w = 43;
				gCharacterClips[4].h = 57;

				gCharacterClips[5].x = 250;
				gCharacterClips[5].y = 0;
				gCharacterClips[5].w = 57;
				gCharacterClips[5].h = 57;

				gCharacterClips[6].x = 313;
				gCharacterClips[6].y = 0;
				gCharacterClips[6].w = 47;
				gCharacterClips[6].h = 57;

				gCharacterClips[7].x = 366;
				gCharacterClips[7].y = 0;
				gCharacterClips[7].w = 37;
				gCharacterClips[7].h = 57;
			}

			if (!gLoseTexture.LoadFromFile("imgs/background/lose.png", gRenderer))
			{
				std::cout << "Failed to load lose image." << std::endl;
				success = false;
			}
		}
	}
	return success;
}

void Close()
{
	gMenuTexture.Free();
	gInstructionTexture.Free();
	gCharacterTexture.Free();
	gGroundTexture.Free();
	gPlayButtonTexture.Free();
	gHelpButtonTexture.Free();
	gExitButtonTexture.Free();
	gBackButtonTexture.Free();
	gPauseButtonTexture.Free();
	gContinueButtonTexture.Free();
	gLoseTexture.Free();
	gText1Texture.Free();
	gScoreTexture.Free();
	gText2Texture.Free();
	gHighScoreTexture.Free();

	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		gBackgroundTexture[i].Free();
	}

	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenuMusic);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gLose);
	Mix_FreeChunk(gJump);
	gMusic = nullptr;
	gMenuMusic = nullptr;
	gClick = nullptr;
	gLose = nullptr;
	gJump = nullptr;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
