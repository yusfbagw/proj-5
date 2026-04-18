/**
 * CS 2110 - Spring 2026 - Project 5
 *
 * @author Yusuf Bagwan
 */

#include "restaurant.h"

// Global variables - do not change these!
TableOrder orders[MAX_ORDER_COUNT];
int num_table_orders = 0;
Menu menu;

/**
 * Adds an new item to the menu.
 *
 * @param name item name
 * @param cook_time time required to cook the item
 * @param cost item cost
 * 
 * Hint: Make sure to consider the case where the menu is empty
 * 
 * @return FAILURE upon invalid arguments, malloc failure, or finding duplicate menu items and SUCCESS otherwise
 */
int addToMenu(char name[MAX_ITEM_LEN], int cook_time, int cost) {
    if (name == NULL || cook_time < 0 || cost < 0 || name == '\0') {
        return FAILURE;
    }
    if (name[MAX_ITEM_LEN] > 300) {
        return FAILURE;
    }
    ItemNode *curr = menu.head;
    while(curr != NULL) {
        if (strcmp(curr->item->name, name) == 0) {
            return FAILURE;
        }
        curr = curr->next;
    }

    Item *newItem = malloc(sizeof(Item));
    if (newItem == NULL) {
        return FAILURE;
    }
    ItemNode *newNode = malloc(sizeof(ItemNode));
    
    if (newNode == NULL) {
        free(newItem);
        return FAILURE;
    }

    strcpy(newItem->name, name);
    newItem->order_count = 0;
    newItem->cook_time = cook_time;
    newItem->cost = cost;

    newNode->item = newItem;
    newNode->next = NULL;

    if (menu.head == NULL) {
        menu.head = newNode;
        return SUCCESS;
    }

    curr = menu.head;
    while(curr->next != NULL)
    {    
        curr = curr->next;
    }
    curr->next = newNode;
    
    return SUCCESS;
}

/**
 * Removes an item from the menu.
 *
 * @param item pointer for item to remove
 *
 * @return FAILURE on invalid input or item not found, else SUCCESS
 */
int removeFromMenu(Item *item) {
    if (item == NULL){
        return FAILURE;
    }

    
}

/**
 * Adds a table order to orders.
 *
 * @param head head of ordered items of the linked list
 * @param table_num table number
 * @param guests_at_table number of guests at the table
 * @param order_time time order placed
 *
 * @return FAILURE on invalid input or full order list; SUCCESS otherwise
 */
int addTableOrder(ItemNode *head, int table_num, int guests_at_table, int order_time) {
    UNUSED(head);
    UNUSED(table_num);
    UNUSED(guests_at_table);
    UNUSED(order_time);
    return INCOMPLETE;
}

/**
 * Calculates the total cost of a TableOrder.
 *
 * @param order the TableOrder to determine total cost of
 *
 * @return total cost of all items in the order, or FAILURE if invalid input
 */
int calculateTableCost(TableOrder order) {
    UNUSED(order);
    return INCOMPLETE;
}

/**
 * Counts how many items in an order are no longer on the menu.
 *
 * @param order the TableOrder to determine if out of date or not
 *
 * @return number of out-of-date items in the order, or FAILURE if input is invalid
 */
int orderOutOfDate(TableOrder order) {
    UNUSED(order);
    return INCOMPLETE;
}

/**
 * Removes menu items with cook_time greater than the ACCEPTABLE_COOKING_TIME,
 * It also increases the first 5 remaining menu item prices by $2.
 *
 * @return FAILURE on invalid input or any other error, else SUCCESS
 */
int keepChefFromQuitting(void) {
    return INCOMPLETE;
}

/**
 * Adds a free apple pie onto the given TableOrder.
 * If apple pie is not on the menu, add it with cost $0 first.
 *
 * @return FAILURE on invalid state or any other error, else SUCCESS
 */
int freeDessert(TableOrder *order) {
    UNUSED(order);
    return INCOMPLETE;
}

/**
 * Returns the oldest TableOrder from orders
 *
 * @return oldest TableOrder
 */
TableOrder findOldestOrder(void) {
    // Note: Get rid of the below TableOrder when you select a different oldest TableOrder as this is just a placeholder.
    TableOrder incomplete = {NULL, 0, 0, 0};
    return incomplete;
}

/**
 * Make a new linked list of ItemNodes that will return the five items with the shortest cook time on the menu.
 * They must be ordered from shortest to longest cooktime.
 *
 * @return head of new list, or FAILURE if fewer than 5 menu items or a malloc failure
 */
ItemNode *rushHourSearch(void) {
    return INCOMPLETE_PTR;
}

/**
 * Frees one table order and takes it out of the global orders.
 *
 * @param table_num the index of orders that should be freed
 *
 * @return FAILURE on invalid input, else SUCCESS
 */
int freeTableOrder(int table_num) {
    UNUSED(table_num);
    return INCOMPLETE;
}

/**
 * Shutsdown the restaurant by freeing all allocated memory
 *
 * Hint: Consider the structs being kept track of that have dynamically allocated memory during runtime.
 *
 * @return FAILURE if any errors, else SUCCESS
 */
int shutdownRestaurant(void) {
    return INCOMPLETE;
}