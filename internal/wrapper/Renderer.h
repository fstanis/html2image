#pragma once

#include "RenderResult.h"

#include <atomic>

#include <Ultralight/Bitmap.h>
#include <Ultralight/Buffer.h>
#include <Ultralight/Ultralight.h>

namespace ul = ultralight;

class Renderer : public ul::LoadListener,
                 public ul::Logger,
                 public ul::FileSystem,
                 public ul::DownloadListener {
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
  virtual ul::DownloadId NextDownloadId(ultralight::View* caller) override;
  virtual bool OnRequestDownload(ultralight::View* caller,
                                 ul::DownloadId id,
                                 const ul::String& url) override;
  virtual void OnBeginDownload(ultralight::View* caller,
                               ul::DownloadId id,
                               const ul::String& url,
                               const ul::String& filename,
                               int64_t expected_content_length) override;
  virtual void OnReceiveDataForDownload(ultralight::View* caller,
                                        ul::DownloadId id,
                                        ul::RefPtr<ul::Buffer> data) override;
  virtual void OnFinishDownload(ultralight::View* caller,
                                ul::DownloadId id) override;
  virtual void OnFailDownload(ultralight::View* caller,
                              ul::DownloadId id) override;

  ul::RefPtr<ul::Bitmap> LoadAndRender(const char* html);

 private:
  std::atomic_flag loading_{false};
  ul::RefPtr<ul::Renderer> renderer_;
  ul::RefPtr<ul::View> view_;
  ul::RefPtr<ul::Bitmap> bitmap_;
  ul::FileSystem* platform_fs_;
};
