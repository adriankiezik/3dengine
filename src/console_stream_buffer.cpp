#include "console_stream_buffer.h"
#include <iostream>

ConsoleStreamBuffer::ConsoleStreamBuffer()
{
  old_buffer = std::cout.rdbuf(this);
}

ConsoleStreamBuffer::~ConsoleStreamBuffer()
{
  std::cout.rdbuf(old_buffer);
}

int ConsoleStreamBuffer::overflow(int c)
{
  if (c == '\n')
  {
    consoleOutput.push_back('\n');
  }
  else
  {
    consoleOutput.push_back(static_cast<char>(c));
  }
  return c;
}

std::string ConsoleStreamBuffer::getOutput() const
{
  return consoleOutput;
}
