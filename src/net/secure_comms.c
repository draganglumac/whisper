/*
 * secure_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "secure_comms.h"

void secure_comms_start(session *s) {
  JNXCHECK(s->is_connected);
  printf("Starting secure comms on %s.\n",s->secure_comms_port);
}

