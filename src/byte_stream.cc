#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), queue_(), is_error_(false), is_closed_(false), read_cumulative_(0), write_cumulative_(0) {}

void Writer::push( string data )
{
  // Your code here.
  if ( is_closed_ )
  {
    return;
  }
  if ( is_error_ )
  {
    return;
  }
  uint64_t size = min(data.length(), capacity_ - queue_.size());
  data = data.substr(0, size);
    for (const auto &c : data)
    {
        queue_.push_back(c);
    }
  write_cumulative_ += size;
}

void Writer::close()
{
  // Your code here.
  // release queue
  is_closed_ = true;
}

void Writer::set_error()
{
  // Your code here.
  is_error_ = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return is_closed_;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - queue_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return write_cumulative_;
}

string_view Reader::peek() const
{
  // Your code here.
  return {std::string_view(&queue_.front(), 1)};
}

bool Reader::is_finished() const
{
  // Your code here.
  return is_closed_ && queue_.empty();
}

bool Reader::has_error() const
{
  // Your code here.
  return is_error_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  const size_t size = min(len, queue_.size());
  queue_.erase(queue_.begin(), queue_.begin() + size);
  read_cumulative_ += size;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return queue_.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return read_cumulative_;
}

uint64_t min(uint64_t x, uint64_t y) {
  if ( x < y ) {
    return x;
  }
  return y;
}