#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <SDL.h>
#include <iostream>
#include <string>
#include <ctime> 
#include <cstdlib>

//Cleanup util
void cleanup(SDL_Window *win);

void cleanup(SDL_Renderer *ren);

void cleanup(SDL_Texture *tex);

void cleanup(SDL_Surface *surf);

//Get relative resource path
std::string getResourcePath(const std::string &subDir = "");

//Get random number
int getRandomNumber(int mod);

#endif