#pragma once

#include <stddef.h>

struct RenderResult {
  void* bytes;
  size_t size;
  unsigned int width;
  unsigned int height;
  unsigned int bpp;
};
