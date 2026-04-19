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
    //Error checking
    if (name == NULL || cook_time < 0 || cost < 0 || name[0] == '\0') {
        return FAILURE;
    }
    if (strlen(name) > MAX_ITEM_LEN) {
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
    //Error checking
    if (item == NULL){
        return FAILURE;
    }
    if (item->cook_time < 0 || item->cost < 0 || item->name == NULL || item->name[0] == '\0') {
        return FAILURE;
    }
    
    ItemNode *curr = menu.head;
    ItemNode *prev = NULL;
    int found = 0;

    while(curr != NULL) {
        if (strcmp(curr->item->name, item->name) == 0) {
            if (prev == NULL) {
                menu.head = curr->next;
            }
            else {
                prev->next = curr->next;
            }
            free(curr->item);
            free(curr);
            found = 1;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (found == 1) {
        return SUCCESS;
    } 
    else{
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
    //Error checking
    if (head == NULL || table_num < 0 || guests_at_table < 0 || order_time < 0) {
        return FAILURE;
    }
    if (num_table_orders >= MAX_ORDER_COUNT) {
        return FAILURE;
    }

    TableOrder *newOrder = &orders[num_table_orders];
    newOrder->head = head;
    newOrder->table_num = table_num;
    newOrder->guests_at_table = guests_at_table;
    newOrder->order_time = order_time;

    //For each loop
    ItemNode *curr = head;
    while (curr != NULL) {
        curr->item->order_count++;
        curr = curr->next;
    }
    num_table_orders++;

    return SUCCESS;
}

/**
 * Calculates the total cost of a TableOrder.
 *
 * @param order the TableOrder to determine total cost of
 *
 * @return total cost of all items in the order, or FAILURE if invalid input
 */
int calculateTableCost(TableOrder order) {
    if (order.head == NULL) { //Error checking
        return FAILURE;
    }

    ItemNode *curr = order.head;
    int cost = 0;
    while(curr != NULL){
        cost += curr->item->cost;
        curr = curr->next;
    }
    return cost;
}

/**
 * Counts how many items in an order are no longer on the menu.
 *
 * @param order the TableOrder to determine if out of date or not
 *
 * @return number of out-of-date items in the order, or FAILURE if input is invalid
 */
int orderOutOfDate(TableOrder order) {
    if (order.head == NULL) { //Error checking
        return FAILURE;
    }

    ItemNode *curr = order.head;
    int count = 0;
    while(curr != NULL){
        ItemNode *currTwo = menu.head;
        int found = 0;
        while(currTwo != NULL){
            if (currTwo->item == curr->item) {
                found = 1;
                break;
            }
            currTwo = currTwo->next;
        }
        if (found == 0) {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

/**
 * Removes menu items with cook_time greater than the ACCEPTABLE_COOKING_TIME,
 * It also increases the first 5 remaining menu item prices by $2.
 *
 * @return FAILURE on invalid input or any other error, else SUCCESS
 */
int keepChefFromQuitting(void) {
    if (menu.head == NULL) { //Error checking
        return FAILURE;
    }
    //Starting at the 1st node of menu.
    ItemNode *curr = menu.head;
    while(curr != NULL) {
        ItemNode *next = curr->next;
        //Removing if the cooktime is greater than the acceptable one.
        if (curr->item->cook_time > ACCEPTABLE_COOK_TIME) {
            removeFromMenu(curr->item);
        }
        curr = next; 
    }
    //Starting at the 1st node of menu.
    ItemNode *currTwo = menu.head;
    for (int i = 0; i < 5 && currTwo != NULL; i++) {
        currTwo->item->cost += 2; 
        currTwo = currTwo->next;
    }
    return SUCCESS;
}

//UP TILL HERE!!!!!


/**
 * Adds a free apple pie onto the given TableOrder.
 * If apple pie is not on the menu, add it with cost $0 first.
 *
 * @return FAILURE on invalid state or any other error, else SUCCESS
 */
int freeDessert(TableOrder *order) {
    //Error checking.
    if (order == NULL) {
        return FAILURE;
    }
    //Initing pointers
    char *applePie = "apple pie";
    ItemNode *pie = NULL;
    ItemNode *curr = menu.head;

    //For each loop
    //Looking for the apple pie in the menu.
    while(curr != NULL) {
        if (strcmp(curr->item->name, applePie) == 0) {
            //needs head nd int table_num, int guests_at_table, int order_time
            pie = curr->item;
            break;
        }
        curr = curr->next;
    }
    curr = menu.head;
    
    while (curr != NULL) {
        if (strcmp(curr->item->name, applePie) == 0) {
            pie = curr->item;
            break;
        }
            curr = curr->next;
    }

    if (pie == NULL) {
        return FAILURE;   //Shouldn't happen, but defensive
    }

    ItemNode *newNode= malloc(sizeof(ItemNode));
    if (newNode == NULL) {
        return FAILURE;
    }

    newNode->item = pie;
    newNode->next = NULL;
    //Here just appending the new node to the order head's list.
    if (order->head == NULL) {
        order->head = newNode;
    } 
    else {
        ItemNode *tail = order->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode;
    }
    return SUCCESS;
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
    //ItemNode newList = malloc(5);
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