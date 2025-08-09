#include "Platform.h"

#include <AppCore/AppCore.h>
#include <string_view>

// exported from Go
extern "C" void logMessage(unsigned char, char*);

constexpr std::string_view icudt67l_filename{"resources/icudt67l.dat"};
constexpr std::string_view cacert_filename{"resources/cacert.pem"};

Platform::Platform() {
  platform_fs_ = ul::GetPlatformFileSystem("ultralight");
  ul::Platform::instance().set_config(
      ul::Config{.user_stylesheet = "html{overflow:hidden}"});
  ul::Platform::instance().set_font_loader(ul::GetPlatformFontLoader());
  ul::Platform::instance().set_file_system(this);
  ul::Platform::instance().set_logger(this);
}

Platform::~Platform() {
  platform_fs_ = nullptr;
}

bool Platform::FileExists(const ul::String& file_path) {
  std::string_view path = file_path.utf8().data();
  if (path == icudt67l_filename || path == cacert_filename) {
    return true;
  }
  return false;
}

ul::String Platform::GetFileMimeType(const ul::String& file_path) {
  return "application/unknown";
}

ul::String Platform::GetFileCharset(const ul::String& file_path) {
  return "utf-8";
}

ul::RefPtr<ul::Buffer> Platform::OpenFile(const ul::String& file_path) {
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

void Platform::LogMessage(ul::LogLevel log_level, const ul::String& message) {
  logMessage(static_cast<uint8_t>(log_level),
             const_cast<char*>(message.utf8().data()));
}
