/*
 * session_service_auth_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */
#include <jnxc_headers/jnxlog.h>
#include "handshake_control.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "../crypto/cryptography.h"
#include "../util/utils.h"
#include "auth_initiator.pb-c.h"
#include "auth_receiver.pb-c.h"

#define CHALLENGE_REQUEST_PUBLIC_KEY 1
#define CHALLENGE_REQUEST_FINISH 0

int handshake_initiator_command_generate(session *ses,\
    jnx_int is_initial_challenge,\
    jnx_uint8 **onetbuffer) {

  jnx_char *local_guid_str;
  jnx_guid_to_string(&(*ses).local_peer_guid,&local_guid_str);
  jnx_size len = strlen(local_guid_str);

  AuthInitiator auth_parcel = AUTH_INITIATOR__INIT;
  auth_parcel.initiator_guid = malloc(sizeof(char) * len);
  if(is_initial_challenge) {
    printf("Generating initial challenge flags.\n");
    auth_parcel.is_requesting_public_key = 1;
    auth_parcel.is_requesting_finish = 0;
  }else {
    printf("Generating finish request flags.\n");
    auth_parcel.is_requesting_public_key = 0;
    auth_parcel.is_requesting_finish = 1;
  }
  memcpy(auth_parcel.initiator_guid,local_guid_str,len); 
  free(local_guid_str);

  jnx_char *pub_key_str = asymmetrical_key_to_string(ses->keypair,PUBLIC);
  jnx_size pub_len = strlen(pub_key_str);
  auth_parcel.initiator_public_key = malloc(sizeof(char*) * pub_len);

  memcpy(auth_parcel.initiator_public_key,pub_key_str,pub_len);
  free(pub_key_str);

  jnx_int parcel_len = auth_initiator__get_packed_size(&auth_parcel);
  jnx_uint8 *obuffer = malloc(parcel_len);
  auth_initiator__pack(&auth_parcel,obuffer);

  free(auth_parcel.initiator_guid);
  free(auth_parcel.initiator_public_key);

  *onetbuffer = obuffer;
  return parcel_len;
}
int handshake_generate_public_key_request(session *ses,\
    jnx_uint8 **onetbuffer) {
  return handshake_initiator_command_generate(ses,CHALLENGE_REQUEST_PUBLIC_KEY,onetbuffer);
}
int handshake_generate_finish_request(session *ses,\
    jnx_uint8 **onetbuffer) { 
  return handshake_initiator_command_generate(ses,CHALLENGE_REQUEST_FINISH,onetbuffer);
}