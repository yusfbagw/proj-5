#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "../restaurant.h"

#define UNREASONABLY_LONG 86400

extern int bytes_until_fail;

extern TableOrder orders[MAX_ORDER_COUNT];
extern int num_table_orders;
extern Menu menu;

void helper_reset_menu(void);
void helper_reset_orders(void);
Item *helper_create_item(const char *name, int cook_time, int cost);
ItemNode *helper_create_node(Item *item);
ItemNode *helper_append_node(ItemNode **head, Item *item);
void helper_free_list(ItemNode *head, int free_items);
void helper_reset(void);
void helper_free_nodes(ItemNode *head);
void helper_free_menu(void);
void helper_add_order(ItemNode *head, int table_num, int guests, int order_time);
void helper_add_menu_item(char *name, int cook_time, int cost);
void helper_free_result(ItemNode *head);

#endif
