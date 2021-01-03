//
//  graphics.c
//  OHVM
//
//  Created by wwhai on 2020/12/25.
//

#include "graphics.h"
//
void init_graphics(SDL_Window *window, SDL_Renderer *renderer)
{
}
//
void enter_graphics(void)
{
}
//
void exit_graphics(SDL_Window *window, SDL_Renderer *renderer)
{
}
//
void points(SDL_Point points[])
{
}
//
void lines(SDL_Renderer *renderer, SDL_Point points[])
{
}
//
void rectangle(SDL_Renderer *renderer,
               int x, int y,
               int w, int h,
               SDL_bool filled)
{
}
//
void draw_text(SDL_Renderer *renderer,
               TTF_Font *font,
               int x, int y,
               char text[],
               int r, int g, int b)
{
}
//
void refresh_graphics(SDL_Renderer *renderer)
{
}
//
void clear_graphics(SDL_Renderer *renderer)
{
}
