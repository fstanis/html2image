#include "Renderer.h"
#include "embeddedfiles.h"

#include <AppCore/AppCore.h>
#include <chrono>
#include <thread>

// exported from Go
extern "C" void logMessage(void*, unsigned char, char*);

const ul::RefPtr<ul::Buffer> icudt67l_buffer =
    ul::Buffer::Create(const_cast<uint8_t*>(icudt67l_data),
                       icudt67l_size,
                       nullptr,
                       nullptr);
const ul::RefPtr<ul::Buffer> cacert_buffer =
    ul::Buffer::Create(const_cast<uint8_t*>(cacert_data),
                       cacert_size,
                       nullptr,
                       nullptr);

constexpr const char* icudt67l_filename{"resources/icudt67l.dat"};
constexpr const char* cacert_filename{"resources/cacert.pem"};

Renderer::Renderer(uint32_t width, uint32_t height) {
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
  done_ = false;
  bitmap_->UnlockPixels();
  view_->LoadHTML(html);
  do {
    renderer_->Update();
    sleep_for(milliseconds(10));
  } while (!done_);
  renderer_->Render();
  bitmap_->LockPixels();
  return bitmap_;
}

bool Renderer::FileExists(const ul::String& file_path) {
  const char* path = file_path.utf8().data();
  if (strcmp(path, icudt67l_filename) == 0 ||
      strcmp(path, cacert_filename) == 0) {
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
  const char* path = file_path.utf8().data();
  if (strcmp(path, icudt67l_filename) == 0) {
    return icudt67l_buffer;
  }
  if (strcmp(path, cacert_filename) == 0) {
    return cacert_buffer;
  }
  return nullptr;
}

void Renderer::OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ul::String& url) {
  if (is_main_frame) {
    done_ = true;
  }
}

void Renderer::LogMessage(ul::LogLevel log_level, const ul::String& message) {
  logMessage(this, static_cast<uint8_t>(log_level),
             const_cast<char*>(message.utf8().data()));
}
