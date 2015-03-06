/*
 * secure_comms.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SECURE_COMMS_H
#define SECURE_COMMS_H
#include "../session/session.h"
#include "../net/discovery.h"
#include <jnxc_headers/jnxtypes.h>
/* Called from both remote endpoints simultaneously */

void secure_comms_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family); 

#endif /* !SECURE_COMMS_H */
