/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: session_transport.proto */

#ifndef PROTOBUF_C_session_5ftransport_2eproto__INCLUDED
#define PROTOBUF_C_session_5ftransport_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1000002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SessionTransport SessionTransport;


/* --- enums --- */


/* --- messages --- */

struct  _SessionTransport
{
  ProtobufCMessage base;
  size_t n_guid;
  int32_t *guid;
};
#define SESSION_TRANSPORT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&session_transport__descriptor) \
    , 0,NULL }


/* SessionTransport methods */
void   session_transport__init
                     (SessionTransport         *message);
size_t session_transport__get_packed_size
                     (const SessionTransport   *message);
size_t session_transport__pack
                     (const SessionTransport   *message,
                      uint8_t             *out);
size_t session_transport__pack_to_buffer
                     (const SessionTransport   *message,
                      ProtobufCBuffer     *buffer);
SessionTransport *
       session_transport__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   session_transport__free_unpacked
                     (SessionTransport *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SessionTransport_Closure)
                 (const SessionTransport *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor session_transport__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_session_5ftransport_2eproto__INCLUDED */
