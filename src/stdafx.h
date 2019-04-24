#ifndef __STDAFX_H__
#define __STDAFX_H__

#define NOMINMAX

#include <cstdint>
#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "easyloggingpp.h"

#include "definitions.h"

#include "EventHandler.h"
#include "EventManager.h"

#include "IdGenerator.h"
#include "StaticId.h"


// ECS
// TODO: Components, Systems

#include "EntityAbstract.h"
#include "Entity.h"
#include "Entities.h"

#include "Wall.h"
//

#include "Object.h"
#include "Layer.h"
#include "RoomGen.h"
#include "Window.h"
#include "TextRenderer.h"
#include "Renderer.h"
#include "MainMenu.h"
#include "TextTester.h"
#include "Game.h"
#include "Core.h"

#endif
