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
#include <jnxc_headers/jnxlog.h>
void print_guid(jnx_guid *g) {
  jnx_char *str;
  jnx_guid_to_string(g,&str);
  JNX_LOG(NULL,"%s",str);
  free(str);
}
#endif
