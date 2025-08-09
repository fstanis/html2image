#include "Renderer.h"

extern "C" {

void* newRenderer(unsigned int width, unsigned int height, double scale) {
  return new Renderer(width, height, scale);
}

void deleteRenderer(void* app) {
  delete static_cast<Renderer*>(app);
}

struct RenderResult render(void* app, const char* html) {
  ul::RefPtr<ul::Bitmap> bitmap =
      static_cast<Renderer*>(app)->LoadAndRender(html);
  if (bitmap.get() == nullptr) {
    return RenderResult{};
  }
  return RenderResult{.bytes = bitmap->raw_pixels(),
                      .size = bitmap->size(),
                      .width = bitmap->width(),
                      .height = bitmap->height(),
                      .bpp = bitmap->bpp()};
}
}
