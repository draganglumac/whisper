/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: session_object.proto */

#ifndef PROTOBUF_C_session_5fobject_2eproto__INCLUDED
#define PROTOBUF_C_session_5fobject_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1000002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SessionObject SessionObject;


/* --- enums --- */


/* --- messages --- */

struct  _SessionObject
{
  ProtobufCMessage base;
  size_t n_guid;
  uint32_t *guid;
  char *rsa_public_key;
};
#define SESSION_OBJECT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&session_object__descriptor) \
    , 0,NULL, NULL }


/* SessionObject methods */
void   session_object__init
                     (SessionObject         *message);
size_t session_object__get_packed_size
                     (const SessionObject   *message);
size_t session_object__pack
                     (const SessionObject   *message,
                      uint8_t             *out);
size_t session_object__pack_to_buffer
                     (const SessionObject   *message,
                      ProtobufCBuffer     *buffer);
SessionObject *
       session_object__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   session_object__free_unpacked
                     (SessionObject *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SessionObject_Closure)
                 (const SessionObject *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor session_object__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_session_5fobject_2eproto__INCLUDED */
