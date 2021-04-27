#ifndef LOADTEXTURE_H_
#define LOADTEXTURE_H_

#include "Base.h"

class LoadTexture
{
public:
	LoadTexture();

	~LoadTexture();

	void Free();

	bool LoadFromRenderedText(std::string textureText, TTF_Font* gFont, SDL_Color textColor, SDL_Renderer* gRenderer);

	bool LoadFromFile(std::string path, SDL_Renderer* gRenderer);

	void Render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = nullptr);

	int GetWidth();

	int GetHeight();

private:
	SDL_Texture* mTexture;

	int mWidth;
	int mHeight;
};

#endif 