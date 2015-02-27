/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/2015 01:32:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __UTILS_H__
#define __UTILS_H__
#include <jnxc_headers/jnxguid.h>
#include "../data/peer.h"
#include <jnxc_headers/jnxlog.h>
void print_guid(jnx_guid *g) {
  jnx_char *str;
  jnx_guid_to_string(g,&str);
  JNX_LOG(NULL,"%s",str);
  free(str);
}
void print_pair(jnx_guid *a, jnx_guid *b) {
  jnx_char *str,*str2;
  jnx_guid_to_string(a,&str);
  jnx_guid_to_string(b,&str2);
  JNX_LOG(NULL,"[%s] [%s]",str,str2);
  free(str);
  free(str2);
}
void print_peer(peer *p) {
  printf("----------\n");
  jnx_char *str;
  jnx_guid_to_string(&(*p).guid,&str);
  printf("Peer guid => %s\n",str);
  free(str);
  printf("Peer host_addres => %s\n",p->host_address);
  printf("Peer user_name => %s\n",p->user_name);
  printf("Peer last seen => %ld\n",p->last_seen);
}
#endif
