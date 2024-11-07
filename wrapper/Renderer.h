#pragma once

#include "RenderResult.h"

#include <Ultralight/Bitmap.h>
#include <Ultralight/Buffer.h>
#include <Ultralight/Ultralight.h>

namespace ul = ultralight;

class Renderer : public ul::LoadListener,
                 public ul::Logger,
                 public ul::FileSystem {
 public:
  Renderer(uint32_t width, uint32_t height);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual bool FileExists(const ul::String& file_path) override;
  virtual ul::String GetFileMimeType(const ul::String& file_path) override;
  virtual ul::String GetFileCharset(const ul::String& file_path) override;
  virtual ul::RefPtr<ul::Buffer> OpenFile(const ul::String& file_path) override;
  virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ul::String& url) override;
  virtual void LogMessage(ul::LogLevel log_level,
                          const ul::String& message) override;

  ul::RefPtr<ul::Bitmap> LoadAndRender(const char* html);

 private:
  volatile bool done_;
  ul::RefPtr<ul::Renderer> renderer_;
  ul::RefPtr<ul::View> view_;
  ul::RefPtr<ul::Bitmap> bitmap_;
};
