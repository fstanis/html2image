#pragma once

#include "RenderResult.h"

#include <mutex>
#include <atomic>
#include <memory>

#include <Ultralight/Bitmap.h>
#include <Ultralight/Buffer.h>
#include <Ultralight/Ultralight.h>

#include "Platform.h"

namespace ul = ultralight;

class Renderer : public ul::LoadListener {
 public:
  Renderer(uint32_t width, uint32_t height, double scale);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ul::String& url) override;

  ul::RefPtr<ul::Bitmap> LoadAndRender(const char* html);

 private:
  static std::mutex platform_mutex_;
  static std::unique_ptr<Platform> platform_;
  static void PreparePlatform();

  std::atomic_flag loading_{false};
  ul::RefPtr<ul::Renderer> renderer_;
  ul::RefPtr<ul::View> view_;
  ul::RefPtr<ul::Bitmap> bitmap_;
};
