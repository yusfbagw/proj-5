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
    if (strlen(name) >= MAX_ITEM_LEN) {
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
    if (item->cook_time < 0 || item->cost < 0 || item->name[0] == '\0') {
        return FAILURE;
    }
    
    ItemNode *curr = menu.head;
    ItemNode *prev = NULL;
    int found = 0;

    while(curr != NULL) {
        if (curr->item == item && strcmp(curr->item->name, item->name) == 0) {
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
    //Here we are just going to walk through the orders and just calculate the cost of the items for that order.
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
    //Here we're going to walk through the orders array and check for the number of times that are out of date
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
        //If we can't find the two same items we say that the item is no longer on the menu and increment count.
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
    //Starting at the 1st node of menu.
    //Going to walk through the menu list.
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
        //Adding 2 dollars to each item's cost that needs more time.
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
    char applePie[MAX_ITEM_LEN] = "apple pie";
    Item *pie = NULL;
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
    
    if (pie == NULL) {
        if (addToMenu(applePie, 0, 0) == FAILURE) {
            return FAILURE;
        }
        curr = menu.head;
        while (curr != NULL) {
            if (strcmp(curr->item->name, applePie) == 0) {
                pie = curr->item;
                break;
            }
            curr = curr->next;
        }
        if (pie == NULL) {//pie shouldn't be null, but just in case.
            return FAILURE;   
        }
    }

    ItemNode *newNode= malloc(sizeof(ItemNode));
    if (newNode == NULL) {
        return FAILURE;
    }

    newNode->item = pie;
    newNode->next = NULL;
    //Here i'm just appending the new node to the order head's list.
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
    if (num_table_orders == 0) {
        TableOrder incomplete = {NULL, 0, 0, 0};
        return incomplete;
    }

    TableOrder oldest = orders[0];
    for (int i = 1; i < num_table_orders; i++) {
        if (orders[i].order_time < oldest.order_time) {
            oldest = orders[i];
        }
    }
    return oldest;
}

/**
 * Make a new linked list of ItemNodes that will return the five items with the shortest cook time on the menu.
 * They must be ordered from shortest to longest cooktime.
 *
 * @return head of new list, or FAILURE if fewer than 5 menu items or a malloc failure
 */
ItemNode *rushHourSearch(void) {
    ItemNode *curr = menu.head;
    int count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    if (count < 5) {
        return NULL;
    }

    //This is to track the picked items and the new list state.
    Item *picked[5];
    int picked_count = 0;
    ItemNode *newHead = NULL;
    ItemNode *newTail = NULL;

    // We need to find the best/shortest cook time for the unpicked item.
    for (int i = 0; i < 5; i++) {
        Item *best = NULL;
        curr = menu.head;
        while (curr != NULL) {
            int alreadyPicked = 0;
            for (int j = 0; j < picked_count; j++) {
                if (picked[j] == curr->item) {
                    alreadyPicked = 1;
                    break;
                }
            }

        if (!alreadyPicked) {
                if (best == NULL || curr->item->cook_time < best->cook_time) {
                    best = curr->item;
                }
            }
            curr = curr->next;
        }

        ItemNode *node = malloc(sizeof(ItemNode));
        if (node == NULL) {
            //This is in case of a malloc failure.
            ItemNode *c = newHead;
            while (c != NULL) {
                ItemNode *n = c->next;
                free(c);
                c = n;
            }
            return NULL;
        }
        node->item = best;
        node->next = NULL;
        
        if (newHead == NULL) {
            newHead = node;
        } else {
            newTail->next = node;
        }
        newTail = node;

        //Here we need to record the picked item.
        picked[picked_count] = best;
        picked_count++;
    }
    //returning the head of the new list holding the shortest cooking time things.
    return newHead;
}

/*

This helper function returns 1 if the item's referenced by any ItemNode in the menu 
or in any TableOrder EXCEPT the one at excludeIndex.

*/
static int isItemReferencedElsewhere(Item *item, int excludeIndex) {
    //checking menu
    ItemNode *curr = menu.head;
    while (curr != NULL) {
        if (curr->item == item){
            return 1;
        }
        curr = curr->next;
    }
    //checking all orders except excludeIndex
    for (int i = 0; i < num_table_orders; i++) {
        if (i == excludeIndex) continue;
        ItemNode *node = orders[i].head;
        while (node != NULL) {
            if (node->item == item) {
               return 1; 
            }
            node = node->next;
        }
    }
    return 0;
}

/**
 * Frees one table order and takes it out of the global orders.
 *
 * @param table_num the index of orders that should be freed
 *
 * @return FAILURE on invalid input, else SUCCESS
 */
int freeTableOrder(int table_num) {
    if (table_num < 0) {
        return FAILURE;
    }
    if (table_num >= num_table_orders) {
        return FAILURE;
    }

    //Tracking items already freed in this pass to avoid double-frees if the same item appears twice.
    Item *freed_items[MAX_ORDER_COUNT * 10];
    int freed_count = 0;
 
    //Going to walk through the orders array and checking for if the table order is used else where, and freeing if it is.
    ItemNode *curr = orders[table_num].head;
    while (curr != NULL) {
        ItemNode *next = curr->next;
        Item *it = curr->item;
 
        int already_freed = 0;
        for (int i = 0; i < freed_count; i++) {
            if (freed_items[i] == it) {
                already_freed = 1;
                break;
            }
        }
 
        if (!already_freed && !isItemReferencedElsewhere(it, table_num)) {
            freed_items[freed_count] = it;
            freed_count++;
            free(it);
        }
        free(curr);
        curr = next;
    }
 
    for (int i = table_num; i < num_table_orders - 1; i++) {
        orders[i] = orders[i + 1];
    }
    //Decrementing the number of table orders as we freed one.
    num_table_orders--;
 
    return SUCCESS;
}

/**
 * Shutsdown the restaurant by freeing all allocated memory
 *
 * Hint: Consider the structs being kept track of that have dynamically allocated memory during runtime.
 *
 * @return FAILURE if any errors, else SUCCESS
 */
int shutdownRestaurant(void) {
    while (num_table_orders > 0) {
        freeTableOrder(0);
    }

    ItemNode *curr = menu.head;
    while (curr != NULL) {
        ItemNode *next = curr->next;
        free(curr->item);
        free(curr);
        curr = next;
    }
    
    menu.head = NULL;

    return SUCCESS;
}