//
//  graphics.h
//  OHVM
//  issue at :https://stackoverflow.com/questions/28016258/using-homebrew-installed-sdl2-with-xcode
//  Created by wwhai on 2020/12/25.
//

#ifndef graphics_h
#define graphics_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#define GRAPHICS_WIDTH 640
#define GRAPHICS_HEIGHT 480
//
SDL_Window *main_indow = NULL;
SDL_Renderer *main_renderer = NULL;
TTF_Font *font = NULL;
//
void init_graphics(SDL_Window *window, SDL_Renderer *renderer);
//
void enter_graphics(void);
//
void exit_graphics(SDL_Window *window, SDL_Renderer *renderer);
//
void points(SDL_Point points[]);
//
void lines(SDL_Renderer *renderer, SDL_Point points[]);
//
void rectangle(SDL_Renderer *renderer,
               int x, int y,
               int w, int h,
               SDL_bool filled);
//
void draw_text(SDL_Renderer *renderer,
               TTF_Font *font,
               int x, int y,
               char text[],
               int r, int g, int b);
//
void refresh_graphics(SDL_Renderer *renderer);
//
void clear_graphics(SDL_Renderer *renderer);
#endif
/* graphics_h */
