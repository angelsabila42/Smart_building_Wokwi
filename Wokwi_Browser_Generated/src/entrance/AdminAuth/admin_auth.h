#ifndef WOKWI_SRC_ENTRANCE_ADMINAUTH_ADMIN_AUTH_H
#define WOKWI_SRC_ENTRANCE_ADMINAUTH_ADMIN_AUTH_H

#include <stdbool.h>

void admin_auth_init(void);
bool admin_login(const char *pin);
void admin_logout(void);
bool admin_is_authenticated(void);

#endif
