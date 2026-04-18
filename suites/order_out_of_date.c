#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


//No items removed from menu
Test(test_order_out_of_date, all_items_on_menu) {
    // Add items to menu
    char burger_name[MAX_ITEM_LEN] = "burger";
    char fries_name[MAX_ITEM_LEN] = "fries";
    char soda_name[MAX_ITEM_LEN] = "soda";
    helper_add_menu_item(burger_name, 10, 5);
    helper_add_menu_item(fries_name, 8, 3);
    helper_add_menu_item(soda_name, 2, 2);
    
    // Get the pointers to the menu items
    Item *burger = menu.head->item; // burger
    Item *fries = menu.head->next->item; // fries 
    Item *soda = menu.head->next->next->item; // soda

    //Table order burger ->fries->soda
    ItemNode soda_node = {soda, NULL};
    ItemNode fries_node = {fries, &soda_node};
    ItemNode burger_node = {burger, &fries_node};
    
    TableOrder order = {&burger_node, 1000, 1, 4};
    
    cr_assert_eq(orderOutOfDate(order), 0, "Order with all items on menu should return 0; Expected 0 Got %d", orderOutOfDate(order));
    
    helper_free_menu();
}

//One Item Removed from menu
Test(test_order_out_of_date, some_items_removed) {
   
    
    char burger_name[MAX_ITEM_LEN] = "burger";
    char fries_name[MAX_ITEM_LEN] = "fries";
    char soda_name[MAX_ITEM_LEN] = "soda";
    helper_add_menu_item(burger_name, 10, 5);
    Item *fries = helper_create_item(fries_name, 8, 3);
    helper_add_menu_item(soda_name, 2, 2);

     // Get pointers to menu items
    Item *burger = menu.head->item; // burger
    Item *soda = menu.head->next->item; // soda

    // Create an order
    ItemNode soda_node = {soda, NULL};
    ItemNode fries_node = {fries, &soda_node};
    ItemNode burger_node = {burger, &fries_node};
    
    TableOrder order = {&burger_node, 1000, 1, 4};
    
    
    cr_assert_eq(orderOutOfDate(order), 1, "Should return 1 when one item is removed: Expected 1 Got %d", orderOutOfDate(order));
    helper_free_menu();
    free(fries);
}

//All Items removed

Test(test_order_out_of_date, all_items_removed) {
    
    
    char burger_name[MAX_ITEM_LEN] = "burger";
    char fries_name[MAX_ITEM_LEN] = "fries";
    char soda_name[MAX_ITEM_LEN] = "soda";
    helper_add_menu_item(burger_name, 10, 5);
    helper_add_menu_item(fries_name, 8, 3);
    helper_add_menu_item(soda_name, 2, 2);

    //get pointer to menu items
    Item *burger = menu.head->item; // burger
    Item *fries = menu.head->next->item; // fries
    Item *soda = menu.head->next->next->item; // soda
    
    ItemNode soda_node = {soda, NULL};
    ItemNode fries_node = {fries, &soda_node};
    ItemNode burger_node = {burger, &fries_node};
    
    TableOrder order = {&burger_node, 1000, 1, 4};
    
    // Remove all items
    helper_free_menu();
    
    cr_assert_eq(orderOutOfDate(order), 3, "Should return the number of items when all items are removed: Expected 3 Got %d", orderOutOfDate(order));
}

//Empty Order, should return 0
Test(test_order_out_of_date, empty_order) {
    char burger_name[MAX_ITEM_LEN] = "burger";
    helper_add_menu_item(burger_name, 10, 5);
    
    TableOrder empty_order = {NULL, 1000, 1, 4};
    
    cr_assert_eq(orderOutOfDate(empty_order), FAILURE, "Empty order should return failure: Expected FAILURE Got %d", orderOutOfDate(empty_order));
    helper_free_menu();
}

//test to make sure checking equality of items is done by pointer comparison and not by string comparison of item names
Test(test_order_out_of_date, item_equality) {
    

    char burger_name[MAX_ITEM_LEN] = "burger";
    helper_add_menu_item(burger_name, 10, 5);
    Item burger1 = {"burger", 0, 10, 5};
    
    ItemNode burger_node = {&burger1, NULL};
    
    TableOrder order = {&burger_node, 1000, 1, 4};
    
    
    cr_assert_eq(orderOutOfDate(order), 1, "Should return 1 because the items dont have the same pointer: Expected 1 Got %d", orderOutOfDate(order));

    helper_free_menu();
}