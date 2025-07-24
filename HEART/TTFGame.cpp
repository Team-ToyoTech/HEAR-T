#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <ctime>
#include "TTFGame.h"
#include <cpr/cpr.h>
#include <thread>
using namespace std;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

void TTFGame() {
    srand(static_cast<unsigned int>(time(0)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_Window* window = SDL_CreateWindow("Balance Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_Color textColor = { 255, 255, 255 };
    SDL_Color boxColor = { 30, 64, 175, 255 };
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    SDL_Surface* loadedSurface = IMG_Load("C:/Users/ASUS/Desktop/sdlproject/Practice/Practice/x64/Debug/test2.png"); // ���� �̹��������� ����.. ��ε� �̻��ϰ�.. ��ġ�ڴ�
    SDL_Surface* loadedSurface2 = IMG_Load("test.png");
    if (!loadedSurface) {
        cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << "\n";
    }
    if (!loadedSurface2) {
        cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << "\n";
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, loadedSurface2);
    SDL_FreeSurface(loadedSurface);
    if (!texture) {
        cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
    }
    bool running = true;
    SDL_Event e;
    SDL_Rect img1Rect = { 460, 390, 300, 300 };  // ���� �̹���
    SDL_Rect img2Rect = { 1160, 390, 300, 300 }; // ������ �̹���

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr, &img1Rect);
        SDL_RenderCopy(renderer, texture2, nullptr, &img2Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}