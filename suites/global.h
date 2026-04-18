#ifndef RESTAURANT_GLOBAL_H
#define RESTAURANT_GLOBAL_H
#include "../restaurant.h"
#include "ag_utils.h"

extern TableOrder orders[MAX_ORDER_COUNT];
extern int num_table_orders;
extern Menu menu;
extern int bytes_until_fail;

#endif