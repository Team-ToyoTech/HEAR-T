#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <ctime>

using namespace std;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

struct GameObject {
    wstring question1;
    wstring question2;
    wstring vsString;
};

vector<wstring> split_by_string(const wstring& str, const wstring& delimiter) {
    vector<wstring> result;
    size_t pos_start = 0, pos_end;
    while ((pos_end = str.find(delimiter, pos_start)) != wstring::npos) {
        result.push_back(str.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + delimiter.length();
    }
    result.push_back(str.substr(pos_start));
    return result;
}

// 멀티라인 텍스트 렌더링
void RenderMultilineText(SDL_Renderer* renderer, TTF_Font* font, const string& text,
    const SDL_Rect& area, SDL_Color color) {
    vector<string> lines;
    string currentLine;
    stringstream ss(text);
    string word;
    int maxWidth = area.w;

    while (ss >> word) {
        string testLine = currentLine.empty() ? word : currentLine + " " + word;
        int w = 0, h = 0;
        TTF_SizeUTF8(font, testLine.c_str(), &w, &h);
        if (w > maxWidth && !currentLine.empty()) {
            lines.push_back(currentLine);
            currentLine = word;
        }
        else {
            currentLine = testLine;
        }
    }
    if (!currentLine.empty()) lines.push_back(currentLine);

    int lineHeight = TTF_FontHeight(font);
    int totalHeight = static_cast<int>(lines.size()) * lineHeight;
    int y = area.y + (area.h - totalHeight) / 2;

    for (const string& line : lines) {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textW, textH;
        TTF_SizeUTF8(font, line.c_str(), &textW, &textH);
        SDL_Rect dstRect = { area.x + (area.w - textW) / 2, y, textW, textH };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        y += lineHeight;
    }
}

// 둥근 사각형 그리기
void DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect inner = { rect.x + radius, rect.y + radius, rect.w - 2 * radius, rect.h - 2 * radius };
    SDL_RenderFillRect(renderer, &inner);

    SDL_Rect left = { rect.x, rect.y + radius, radius, rect.h - 2 * radius };
    SDL_Rect right = { rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius };
    SDL_Rect top = { rect.x + radius, rect.y, rect.w - 2 * radius, radius };
    SDL_Rect bottom = { rect.x + radius, rect.y + rect.h - radius, rect.w - 2 * radius, radius };
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);
    SDL_RenderFillRect(renderer, &top);
    SDL_RenderFillRect(renderer, &bottom);

    for (int w = 0; w < radius; ++w) {
        for (int h = 0; h < radius; ++h) {
            if ((w * w + h * h) <= radius * radius) {
                SDL_RenderDrawPoint(renderer, rect.x + radius - w, rect.y + radius - h);
                SDL_RenderDrawPoint(renderer, rect.x + rect.w - radius + w - 1, rect.y + radius - h);
                SDL_RenderDrawPoint(renderer, rect.x + radius - w, rect.y + rect.h - radius + h - 1);
                SDL_RenderDrawPoint(renderer, rect.x + rect.w - radius + w - 1, rect.y + rect.h - radius + h - 1);
            }
        }
    }
}

void balanceGame() {
    srand(static_cast<unsigned int>(time(0)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Window* window = SDL_CreateWindow("Balance Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/malgun.ttf", 72); // 크기 적당히 조절
    if (!font) {
        SDL_Log("Font Error: %s", TTF_GetError());
        return; 
    }

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Color boxColor = { 30, 64, 175, 255 };

    wifstream in(L"balanceGame.txt");
    in.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    vector<GameObject> balanceGameList;
    wstring s;
    while (getline(in, s)) {
        vector<wstring> v = split_by_string(s, L" vs ");
        if (v.size() == 2) {
            GameObject g;
            g.question1 = v[0];
            g.vsString = L"VS";
            g.question2 = v[1];
            balanceGameList.push_back(g);
        }
    }
    in.close();

    SDL_Event event;
    bool running = true;
    vector<bool> visited(balanceGameList.size(), false);
    int stringIndex = 0;
    wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    int cnt = 0;
    while (running) {
        cnt++;
        if (cnt > balanceGameList.size())break;
        stringIndex = rand() % balanceGameList.size();
        while (visited[stringIndex]) {
            stringIndex = rand() % balanceGameList.size();
        }
        visited[stringIndex] = true;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect topRect = { 200, 250, 1520, 150 };
        SDL_Rect bottomRect = { 200, 600, 1520, 150 };

        string q1 = converter.to_bytes(balanceGameList[stringIndex].question1);
        string q2 = converter.to_bytes(balanceGameList[stringIndex].question2);
        string vs = converter.to_bytes(balanceGameList[stringIndex].vsString);

        // Draw box
        DrawRoundedRect(renderer, topRect, 20, boxColor);
        DrawRoundedRect(renderer, bottomRect, 20, boxColor);

        // Render q1
        SDL_Surface* surface1 = TTF_RenderUTF8_Blended(font, q1.c_str(), textColor);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
        int textW1, textH1;
        TTF_SizeUTF8(font, q1.c_str(), &textW1, &textH1);
        SDL_Rect dst1 = { topRect.x + (topRect.w - textW1) / 2, topRect.y + (topRect.h - textH1) / 2, textW1, textH1 };
        SDL_RenderCopy(renderer, texture1, nullptr, &dst1);
        SDL_FreeSurface(surface1);
        SDL_DestroyTexture(texture1);

        // Render q2
        SDL_Surface* surface2 = TTF_RenderUTF8_Blended(font, q2.c_str(), textColor);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        int textW2, textH2;
        TTF_SizeUTF8(font, q2.c_str(), &textW2, &textH2);
        SDL_Rect dst2 = { bottomRect.x + (bottomRect.w - textW2) / 2, bottomRect.y + (bottomRect.h - textH2) / 2, textW2, textH2 };
        SDL_RenderCopy(renderer, texture2, nullptr, &dst2);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(texture2);

        // Render VS
        SDL_Surface* vsSurface = TTF_RenderUTF8_Blended(font, vs.c_str(), textColor);
        SDL_Texture* vsTexture = SDL_CreateTextureFromSurface(renderer, vsSurface);
        int vsW, vsH;
        TTF_SizeUTF8(font, vs.c_str(), &vsW, &vsH);
        SDL_Rect vsRect = { (WINDOW_WIDTH - vsW) / 2, 475, vsW, vsH };
        SDL_RenderCopy(renderer, vsTexture, nullptr, &vsRect);
        SDL_FreeSurface(vsSurface);
        SDL_DestroyTexture(vsTexture);

        SDL_RenderPresent(renderer);

        bool wait = true;
        while (wait) {
            SDL_WaitEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN &&
                (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)) {
                wait = false;
            }
        }
		SDL_Delay(100); // 지연 시간 추가
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}