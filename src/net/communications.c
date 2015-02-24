/*
 * =====================================================================================
 *
 *       Filename:  communications.c
 *
 *    Description:  communication 
 *
 *        Version:  1.0
 *        Created:  02/24/2015 08:42:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  AlexsJones (jonesax@hush.com),
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "communications.h"
communication_service *communication_service_create() {
  communication_service *cs = malloc(sizeof(communication_service));

  return cs;
}
void communication_service_destroy(communication_service **cs) {
  JNXCHECK(*cs);
  free(*cs);
  *cs = NULL;
}
