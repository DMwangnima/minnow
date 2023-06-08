#include "tcp_receiver.hh"

#include <optional>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  if ( !has_syn )
  {
    if ( !message.SYN )
    {
      return;
    }
    has_syn = true;
    isn = message.seqno;
  }
  uint64_t checkpoint = inbound_stream.bytes_pushed() + 1;
  uint64_t first_ind = message.seqno.unwrap(isn, checkpoint) + message.SYN - 1;
  reassembler.insert(first_ind, message.payload, message.FIN, inbound_stream);
}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
  TCPReceiverMessage msg;
  uint64_t cap = inbound_stream.available_capacity();
  if ( !has_syn )
  {
    msg.ackno = nullopt;
  } else {
    uint64_t checkpoint = inbound_stream.bytes_pushed() + 1;
    if ( inbound_stream.is_closed() )
    {
      checkpoint++;
    }
    msg.ackno = Wrap32::wrap(checkpoint, isn);
  }
  msg.window_size = cap > 65535 ? 65535 : cap;
  return msg;
}
