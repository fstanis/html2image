#include "Renderer.h"

extern "C" {

void* newRenderer(unsigned int width, unsigned int height) {
  return new Renderer(width, height);
}

void deleteRenderer(void* app) {
  delete static_cast<Renderer*>(app);
}

struct RenderResult render(void* app, const char* html) {
  ul::RefPtr<ul::Bitmap> bitmap = static_cast<Renderer*>(app)->LoadAndRender(html);
  return RenderResult{.bytes = bitmap->raw_pixels(),
                      .size = bitmap->size(),
                      .width = bitmap->width(),
                      .height = bitmap->height(),
                      .bpp = bitmap->bpp()};
}
}
