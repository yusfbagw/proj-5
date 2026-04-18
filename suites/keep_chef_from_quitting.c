#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"

/**
 * The chef doesn't like his job and if it becomes too much, he will quit! 
 * As Restaurant Manager, your goal is to prevent this. The method removes all items from the menu whose cooking time exceeds ACCEPTABLE_COOKING_TIME.
 *  Afterward, increase the cost of the first five items in the menu by 2 each. 
 * Returns SUCCESS if the operation completes successfully andFAILURE if any invalid conditions occur.

 acceptable cooking time is 60
 */

Test(test_keep_chef_from_quitting, normal_test) {
    char burger_name[MAX_ITEM_LEN] = "burger";
    char salad_name[MAX_ITEM_LEN] = "salad";
    char steak_name[MAX_ITEM_LEN] = "steak";
    char lobster_name[MAX_ITEM_LEN] = "lobster";    
    helper_add_menu_item(lobster_name, 80, 25);
    helper_add_menu_item(steak_name, 70, 20);
    helper_add_menu_item(burger_name, 20, 10);
    helper_add_menu_item(salad_name, 10, 5); 
    // NOTE: this function PREpends to the menu,
    // so the linked list looks like:
    // salad -> burger -> steak -> lobster
    
    
    

    //lobster and steak should be removed, salad and burger should have their price increased by 2
    keepChefFromQuitting();
    ItemNode *curr = menu.head;
    cr_assert_str_eq(curr->item->name, "salad", "First item should be salad: Expected salad Got %s", curr->item->name);
    cr_assert_eq(curr->item->cost, 7.0f, "Salad should have cost increased by 2: Expected 7.0 Got %f", curr->item->cost);
    curr = curr->next;
    cr_assert_str_eq(curr->item->name, "burger", "Second item should be burger: Expected burger Got %s", curr->item->name);
    cr_assert_eq(curr->item->cost, 12.0f, "Burger should have cost increased by 2: Expected 12.0 Got %f", curr->item->cost);
    curr = curr->next;  
    cr_assert_eq(curr, NULL, "Only two items should be on the menu after removing steak and lobster: Expected NULL Got %p", curr);

    helper_free_menu();
}

Test(test_keep_chef_from_quitting, no_items_removed) {
    char burger_name[MAX_ITEM_LEN] = "burger";
    char salad_name[MAX_ITEM_LEN] = "salad";  
    helper_add_menu_item(burger_name, 20, 10);
    helper_add_menu_item(salad_name, 10, 5);
    // NOTE: this function PREpends to the menu,
    // so the linked list looks like:
    // salad -> burger

    //both items should have their price increased by 2
    keepChefFromQuitting();
    ItemNode *curr = menu.head;
    cr_assert_str_eq(curr->item->name, "salad", "First item should be salad: Expected salad Got %s", curr->item->name);
    cr_assert_eq(curr->item->cost, 7.0f, "Salad should have cost increased by 2: Expected 7.0 Got %f", curr->item->cost);
    curr = curr->next;
    cr_assert_str_eq(curr->item->name, "burger", "Second item should be burger: Expected burger Got %s", curr->item->name);
    cr_assert_eq(curr->item->cost, 12.0f, "Burger should have cost increased by 2: Expected 12.0 Got %f", curr->item->cost);
    curr = curr->next;  
    cr_assert_eq(curr, NULL, "Only two items should be on the menu: Expected NULL Got %p", curr);
    helper_free_menu();
}

Test(test_keep_chef_from_quitting, all_items_removed) {
    char steak_name[MAX_ITEM_LEN] = "steak";
    char lobster_name[MAX_ITEM_LEN] = "lobster";    
    helper_add_menu_item(lobster_name, 80, 25);
    helper_add_menu_item(steak_name, 70, 20);
    // NOTE: this function PREpends to the menu,
    // so the linked list looks like:
    // steak -> lobster

    //both items should be removed and menu should be empty
    keepChefFromQuitting();
    ItemNode *curr = menu.head;
    cr_assert_eq(curr, NULL, "All items should be removed from the menu: Expected NULL Got %p", curr);
    helper_free_menu(); // just in case
}

Test(test_keep_chef_from_quitting, empty_menu) {
    //menu is empty, so nothing should happen and it should return SUCCESS or should mepty menu be FAILURE?
    int result = keepChefFromQuitting();
    cr_assert_eq(result, SUCCESS, "Should return SUCCESS when menu is empty: Expected SUCCESS Got %d", result);
    cr_assert_eq(menu.head, NULL, "Menu should still be empty: Expected NULL Got %p", menu.head);
}


