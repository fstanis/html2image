#include "Renderer.h"

#include <AppCore/AppCore.h>
#include <chrono>
#include <thread>

std::mutex Renderer::platform_mutex_;
std::unique_ptr<Platform> Renderer::platform_;

void Renderer::PreparePlatform() {
  std::lock_guard<std::mutex> guard(platform_mutex_);
  if (platform_ != nullptr) {
    return;
  }
  platform_ = std::make_unique<Platform>();
}

Renderer::Renderer(uint32_t width, uint32_t height, double scale) {
  PreparePlatform();
  renderer_ = ul::Renderer::Create();
  view_ = renderer_->CreateView(width, height,
                                ul::ViewConfig{.is_accelerated = false,
                                               .initial_device_scale = scale,
                                               .initial_focus = false},
                                nullptr);
  view_->set_load_listener(this);

  bitmap_ = static_cast<ul::BitmapSurface*>(view_->surface())->bitmap();
}

Renderer::~Renderer() {
  view_ = nullptr;
  renderer_ = nullptr;
  bitmap_ = nullptr;
}

ul::RefPtr<ul::Bitmap> Renderer::LoadAndRender(const char* html) {
  using std::chrono::milliseconds;
  using std::this_thread::sleep_for;
  if (loading_.test_and_set()) {
    return nullptr;
  }
  bitmap_->UnlockPixels();
  view_->LoadHTML(html);
  do {
    renderer_->Update();
    sleep_for(milliseconds(10));
  } while (loading_.test());
  renderer_->Render();
  bitmap_->LockPixels();
  return bitmap_;
}

void Renderer::OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ul::String& url) {
  if (is_main_frame) {
    loading_.clear();
  }
}
