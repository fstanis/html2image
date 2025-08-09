#pragma once

#include <Ultralight/Buffer.h>
#include <Ultralight/Ultralight.h>

namespace ul = ultralight;

class Platform : public ul::Logger, public ul::FileSystem {
 public:
  Platform();
  ~Platform();

  virtual bool FileExists(const ul::String& file_path) override;
  virtual ul::String GetFileMimeType(const ul::String& file_path) override;
  virtual ul::String GetFileCharset(const ul::String& file_path) override;
  virtual ul::RefPtr<ul::Buffer> OpenFile(const ul::String& file_path) override;
  virtual void LogMessage(ul::LogLevel log_level,
                          const ul::String& message) override;
 private:
  ul::FileSystem* platform_fs_;
};
