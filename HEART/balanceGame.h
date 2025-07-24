#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

typedef struct {
	SDL_Rect rect;
	SDL_Color color;
	bool hovered;
	bool clicked;
} Button;

vector<wstring> split_by_string(const wstring& str, const wstring& delimiter);
void RenderMultilineText(SDL_Renderer* renderer, TTF_Font* font, const string& text,
	const SDL_Rect& area, SDL_Color color);
void balanceGame();
void DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, int radius, SDL_Color color);