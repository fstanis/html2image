#include "Renderer.h"

#include <AppCore/AppCore.h>
#include <chrono>
#include <thread>

// exported from Go
extern "C" void logMessage(void*, unsigned char, char*);

constexpr std::string_view icudt67l_filename{"resources/icudt67l.dat"};
constexpr std::string_view cacert_filename{"resources/cacert.pem"};

Renderer::Renderer(uint32_t width, uint32_t height) {
  platform_fs_ = ul::GetPlatformFileSystem("ultralight");
  ul::Platform::instance().set_config(ul::Config{});
  ul::Platform::instance().set_font_loader(ul::GetPlatformFontLoader());
  ul::Platform::instance().set_file_system(this);
  ul::Platform::instance().set_logger(this);

  renderer_ = ul::Renderer::Create();
  view_ = renderer_->CreateView(
      width, height,
      ul::ViewConfig{.is_accelerated = false, .initial_device_scale = 2.0},
      nullptr);
  view_->set_load_listener(this);
  view_->set_download_listener(this);

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

bool Renderer::FileExists(const ul::String& file_path) {
  std::string_view path = file_path.utf8().data();
  if (path == icudt67l_filename || path == cacert_filename) {
    return true;
  }
  return false;
}

ul::String Renderer::GetFileMimeType(const ul::String& file_path) {
  return "application/unknown";
}

ul::String Renderer::GetFileCharset(const ul::String& file_path) {
  return "utf-8";
}

ul::RefPtr<ul::Buffer> Renderer::OpenFile(const ul::String& file_path) {
  std::string_view path = file_path.utf8().data();
  if (path == icudt67l_filename || path == cacert_filename) {
    size_t slash_pos = path.find_last_of("/");
    if (slash_pos != std::string_view::npos) {
      path.remove_prefix(slash_pos);
    }
    return platform_fs_->OpenFile(path.data());
  }
  return nullptr;
}

void Renderer::OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ul::String& url) {
  if (is_main_frame) {
    loading_.clear();
  }
}

void Renderer::LogMessage(ul::LogLevel log_level, const ul::String& message) {
  logMessage(this, static_cast<uint8_t>(log_level),
             const_cast<char*>(message.utf8().data()));
}

ul::DownloadId Renderer::NextDownloadId(ultralight::View* caller) {
  return 0;
}

bool Renderer::OnRequestDownload(ultralight::View* caller,
                                 ul::DownloadId id,
                                 const ul::String& url) {
  return false;
}

void Renderer::OnBeginDownload(ultralight::View* caller,
                               ul::DownloadId id,
                               const ul::String& url,
                               const ul::String& filename,
                               int64_t expected_content_length) {}

void Renderer::OnReceiveDataForDownload(ultralight::View* caller,
                                        ul::DownloadId id,
                                        ul::RefPtr<ul::Buffer> data) {}

void Renderer::OnFinishDownload(ultralight::View* caller, ul::DownloadId id) {}

void Renderer::OnFailDownload(ultralight::View* caller, ul::DownloadId id) {}
