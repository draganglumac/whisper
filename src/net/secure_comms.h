/*
 * secure_comms.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SECURE_COMMS_H
#define SECURE_COMMS_H
#include "../session/session.h"

/* Called from both remote endpoints simultaneously */

void secure_comms_start(session *s);

#endif /* !SECURE_COMMS_H */
