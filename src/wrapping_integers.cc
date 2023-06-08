#include "wrapping_integers.hh"
#include <iostream>
#include <math.h>
using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return Wrap32 (uint32_t((n % uint64_t(pow(2, 32)) + uint64_t(zero_point.raw_value_))) % uint64_t(pow(2, 32)));
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  uint64_t cap = uint64_t(pow(2, 32));
  uint64_t first = uint64_t(raw_value_ - zero_point.raw_value_) % cap;
  if ( checkpoint <= first )
  {
    return first;
  }
  uint64_t point = (checkpoint - first)/cap;
  uint64_t prev = point;
  uint64_t next = prev + 1;

  uint64_t dist_prev = checkpoint - first - prev * cap;
  uint64_t dist_next = first + next * cap - checkpoint;
  if ( dist_prev < dist_next )
  {
    return first + prev * cap;
  }
  return first + next * cap;
}