#pragma once

#include <stdlib.h>

#include "RenderResult.h"

void* newRenderer(unsigned int width, unsigned int height);
void deleteRenderer(void* app);

struct RenderResult render(void* app, const char* html);
