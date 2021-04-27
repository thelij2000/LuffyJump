#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include "Base.h"
#include "Button.h"
#include "Enemy.h"
#include "Character.h"

bool Init();
bool LoadMedia();
void Close();

std::string GetHighScoreFromFile(std::string path);

void UpdateHighScore(std::string path,
    const int& score,
    const std::string& old_high_score);

int UpdateGameTimeAndScore(int& time, int& speed, int& score);

void RenderScrollingBackground(std::vector <double>& offsetSpeed,
    LoadTexture(&gBackgroundTexture)[BACKGROUND_LAYER],
    SDL_Renderer* gRenderer);

void RenderScrollingGround(int& speed,
    const int acceleration,
    LoadTexture gGroundTexture,
    SDL_Renderer* gRenderer);

void HandlePlayButton(SDL_Event* e,
    Button& PlayButton,
    bool& QuitMenu,
    bool& Play,
    Mix_Chunk* gClick);

void HandleHelpButton(SDL_Event* e,
    SDL_Rect(&gBackButton)[BUTTON_TOTAL],
    Button& HelpButton,
    Button& BackButton,
    LoadTexture gInstructionTexture,
    LoadTexture gBackButtonTexture,
    SDL_Renderer* gRenderer,
    bool& Quit_game,
    Mix_Chunk* gClick);

void HandleExitButton(SDL_Event* e,
    Button& ExitButton,
    bool& Quit,
    Mix_Chunk* gClick);

void HandleContinueButton(Button ContinueButton,
    LoadTexture gContinueButtonTexture,
    SDL_Event* e,
    SDL_Renderer* gRenderer,
    SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
    bool& Game_State, Mix_Chunk* gClick);

void HandlePauseButton(SDL_Event* e,
    SDL_Renderer* gRenderer,
    SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
    Button& PauseButton,
    Button ContinueButton,
    LoadTexture gContinueButtonTexture,
    bool& game_state,
    Mix_Chunk* gClick);

void GenerateEnemy(Enemy& enemy1,
    Enemy& enemy2,
    Enemy& enemy3,
    SDL_Rect(&gEnemyClips)[FLYING_FRAMES],
    SDL_Renderer* gRenderer);

bool CheckColission(Character character,
    SDL_Rect* char_clip,
    Enemy enemy,
    SDL_Rect* enemy_clip = nullptr);

bool CheckEnemyColission(Character character,
    Enemy enemy1,
    Enemy enemy2,
    Enemy enemy3,
    SDL_Rect* char_clip,
    SDL_Rect* enemy_clip = nullptr);

void ControlCharFrame(int& frame);

void ControlEnemyFrame(int& frame);

void DrawPlayerScore(LoadTexture gTextTexture,
    LoadTexture gScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont,
    const int& score);

void DrawPlayerHighScore(LoadTexture gTextTexture,
    LoadTexture gHighScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont,
    const std::string& HighScore);

void DrawEndGameSelection(LoadTexture gLoseTexture,
    SDL_Event* e,
    SDL_Renderer* gRenderer,
    bool& Play_Again);

#endif 