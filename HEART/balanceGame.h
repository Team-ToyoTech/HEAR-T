#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
	SDL_Rect rect;
	SDL_Color color;
	bool hovered;
	bool clicked;
} Button;

std::vector<std::wstring> split_by_string(const std::wstring& str, const std::wstring& delimiter);
void RenderMultilineText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
	const SDL_Rect& area, SDL_Color color);
void balanceGame();
void DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, int radius, SDL_Color color);