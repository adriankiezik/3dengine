#ifndef CONSOLE_STREAM_BUFFER_H
#define CONSOLE_STREAM_BUFFER_H

#include <streambuf>
#include <string>

class ConsoleStreamBuffer : public std::streambuf
{
public:
  ConsoleStreamBuffer();
  ~ConsoleStreamBuffer();

  std::string getOutput() const;

protected:
  int overflow(int c) override;

private:
  std::streambuf *old_buffer;
  std::string consoleOutput;
};

#endif
