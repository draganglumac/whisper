/*
 * app_context.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

extern struct discovery_service;
extern struct session_service;

typedef struct {
  discovery_service *discovery;
  session_service *session_serv;
} app_context_t;

#endif /* !APP_CONTEXT_H */
