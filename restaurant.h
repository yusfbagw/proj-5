#ifndef RESTAURANT_H
#define RESTAURANT_H

/* DO NOT MODIFY FILE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suites/fakemalloc.h"

#define UNUSED(x) ((void)x)
#define FAILURE 0
#define SUCCESS 1

#define INCOMPLETE -1
#define INCOMPLETE_PTR NULL

#define MAX_ORDER_COUNT 50
#define MAX_ITEM_LEN 64
#define ACCEPTABLE_COOK_TIME 60

#define INCOMPLETE -1
#define INCOMPLETE_PTR NULL

// Structs
typedef struct Item {
	char name[MAX_ITEM_LEN];
	int order_count;
	int cook_time;
	int cost;
} Item;

typedef struct ItemNode {
	struct Item *item;
	struct ItemNode *next;
} ItemNode;

typedef struct TableOrder {
	struct ItemNode *head;
	int order_time;
	int table_num;
	int guests_at_table;
} TableOrder;

typedef struct Menu {
	struct ItemNode *head;
	int last_updated;
} Menu;

// Menu methods
int addToMenu(char name[MAX_ITEM_LEN], int cook_time, int cost);

int removeFromMenu(Item* item);

// TableOrder

int addTableOrder(ItemNode* head, int table_num, int guests_at_table, int order_time);

int calculateTableCost(TableOrder order);

int orderOutOfDate(TableOrder order);

int keepChefFromQuitting(void);

int freeDessert(TableOrder *order);

TableOrder findOldestOrder(void);

ItemNode* rushHourSearch(void);

int freeTableOrder(int order_index);

int shutdownRestaurant(void);

#endif
