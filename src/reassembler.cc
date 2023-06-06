#include "reassembler.hh"

#include <iostream>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  if ( !buf_.is_initialized() )
  {
    buf_.initialize(output.available_capacity());
  }
  uint64_t accepetable = output.available_capacity();
  buf_.push(first_index, data, accepetable);
  string res = buf_.pop(accepetable);
  if ( res.length() > 0 )
  {
    output.push(res);
  }
  if ( is_last_substring )
  {
    ready_close_ = true;
  }
  if ( ready_close_ && buf_.bytes_pending() == 0 )
  {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return buf_.bytes_pending();
}

string StreamBuffer::pop( uint64_t max_length )
{
    string result = "";
    uint64_t end_ind = end_;
    uint64_t pop_length = 0;
    for ( uint64_t i = start_; i < end_; i++)
    {
      uint64_t ind = start_ % capacity_;
      if ( flag_[ind] )
      {
        if ( pop_length < max_length )
        {
          result.push_back(buffer_[ind]);
          flag_[ind] = false;
          start_++;
          pop_length++;
        }
      } else
      {
        end_ind = i;
        break;
      }
    }

    if ( inspect_start_ < end_ind )
    {
      pending_ -= ( end_ind - inspect_start_ );
      inspect_start_ = end_ind;
    }

    return result;
}

void StreamBuffer::push(uint64_t start_ind, string data, uint64_t max_length)
{
  uint64_t push_length = 0;
  uint64_t valid_start = start_ind;
  if ( valid_start < start_ )
  {
    valid_start = start_;
  }
  uint64_t end_ind = start_ind + data.length();
  if ( end_ind > start_ + max_length )
  {
    end_ind = start_ + max_length;
  }

  for ( uint64_t i = valid_start; i < end_ind; i++ )
  {
    uint64_t ind = i % capacity_;
    if ( !flag_[ind] )
    {
      flag_[ind] = true;
      buffer_[ind] = data[i-start_ind];
      push_length++;
    }
  }
  
  pending_ += push_length;

  if ( end_ < end_ind )
  {
    end_ = end_ind;
  }
}

uint64_t StreamBuffer::bytes_pending() const
{
  return pending_;
}

void StreamBuffer::initialize(uint64_t capacity)
{
    buffer_ = vector<char>(capacity);
    flag_ = vector<bool>(capacity, false);
    capacity_ = capacity;
    is_initialized_ = true;
}

bool StreamBuffer::is_initialized()
{
  return is_initialized_;
}
