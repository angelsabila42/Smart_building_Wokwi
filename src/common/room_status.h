#ifndef ROOM_STATUS_H
#define ROOM_STATUS_H

#include <stdbool.h>
void room_status_update(int room_number, int occupancy, bool light_on);

bool get_room_occupancy(int room_number, int *occupancy);
bool get_room_light(int room_number, bool *light_on);

#endif