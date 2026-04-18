#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


Test(test_rush_hour_search, empty_menu)
{
    menu.head = NULL;
    ItemNode *result = rushHourSearch();
    cr_assert(zero(ptr, result), "Should return NULL when menu is empty");
}

Test(test_rush_hour_search, fewer_than_five_items)
{
    menu.head = NULL;
    helper_add_menu_item("burger", 10, 8);
    helper_add_menu_item("fries", 5, 3);
    helper_add_menu_item("salad", 7, 6);
    helper_add_menu_item("soup", 12, 4);

    ItemNode *result = rushHourSearch();
    cr_assert(zero(ptr, result), "Should return NULL when menu has fewer than 5 items");

    helper_free_menu();
}

Test(test_rush_hour_search, exactly_five_items)
{
    menu.head = NULL;
    helper_add_menu_item("e", 50, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("a", 10, 5);

    ItemNode *result = rushHourSearch();
    cr_assert(not(zero(ptr, result)), "Should return non-NULL with exactly 5 items");

    // Should be sorted shortest to longest: a(10), b(20), c(30), d(40), e(50)
    ItemNode *curr = result;
    cr_assert(eq(int, curr->item->cook_time, 10), "First item should have cook_time 10");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 20), "Second item should have cook_time 20");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 30), "Third item should have cook_time 30");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 40), "Fourth item should have cook_time 40");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 50), "Fifth item should have cook_time 50");
    cr_assert(zero(ptr, curr->next), "List should end after 5 items");

    helper_free_result(result);
    helper_free_menu();
}

Test(test_rush_hour_search, selects_five_shortest)
{
    menu.head = NULL;
    helper_add_menu_item("steak", 100, 20);
    helper_add_menu_item("lobster", 90, 30);
    helper_add_menu_item("pasta", 25, 12);
    helper_add_menu_item("salad", 5, 6);
    helper_add_menu_item("soup", 15, 4);
    helper_add_menu_item("fries", 8, 3);
    helper_add_menu_item("burger", 20, 10);
    helper_add_menu_item("cake", 35, 7);

    ItemNode *result = rushHourSearch();
    cr_assert(not(zero(ptr, result)), "Should return non-NULL");

    // 5 shortest: salad(5), fries(8), soup(15), burger(20), pasta(25)
    ItemNode *curr = result;
    cr_assert(eq(int, curr->item->cook_time, 5), "First should be cook_time 5");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 8), "Second should be cook_time 8");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 15), "Third should be cook_time 15");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 20), "Fourth should be cook_time 20");
    curr = curr->next;
    cr_assert(eq(int, curr->item->cook_time, 25), "Fifth should be cook_time 25");
    cr_assert(zero(ptr, curr->next), "List should end after 5 items");

    helper_free_result(result);
    helper_free_menu();
}

Test(test_rush_hour_search, items_point_to_menu_items)
{
    menu.head = NULL;
    helper_add_menu_item("a", 10, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("e", 50, 5);

    ItemNode *result = rushHourSearch();
    cr_assert(not(zero(ptr, result)), "Should return non-NULL");

    // returned ItemNodes should point to the same Item structs as the menu
    ItemNode *curr = result;
    while (curr != NULL)
    {
        int found = 0;
        ItemNode *menu_curr = menu.head;
        while (menu_curr != NULL)
        {
            if (curr->item == menu_curr->item)
            {
                found = 1;
                break;
            }
            menu_curr = menu_curr->next;
        }
        cr_assert(eq(int, found, 1), "Result items should point to existing menu items, not copies");
        curr = curr->next;
    }

    helper_free_result(result);
    helper_free_menu();
}

Test(test_rush_hour_search, malloc_fail_array)
{
    menu.head = NULL;
    helper_add_menu_item("a", 10, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("e", 50, 5);

    // fail the malloc for the Item** array
    bytes_until_fail = 0;
    ItemNode *result = rushHourSearch();
    cr_assert(zero(ptr, result), "Should return NULL on malloc failure for array");
    bytes_until_fail = -1;

    helper_free_menu();
}

Test(test_rush_hour_search, malloc_fail_node)
{
    menu.head = NULL;
    helper_add_menu_item("a", 10, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("e", 50, 5);

    // allow the array malloc but fail on the first node malloc
    bytes_until_fail = 5 * sizeof(Item *);
    ItemNode *result = rushHourSearch();
    cr_assert(zero(ptr, result), "Should return NULL when node malloc fails");
    bytes_until_fail = -1;

    helper_free_menu();
}

Test(test_rush_hour_search, malloc_fail_mid_list)
{
    menu.head = NULL;
    helper_add_menu_item("a", 10, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("e", 50, 5);

    // Allow array + 2 nodes, fail on third node
    bytes_until_fail = 5 * sizeof(Item *) + 2 * sizeof(ItemNode);
    ItemNode *result = rushHourSearch();
    cr_assert(zero(ptr, result), "Should return NULL and clean up when malloc fails mid-list");
    bytes_until_fail = -1;

    helper_free_menu();
}

Test(test_rush_hour_search, does_not_modify_menu)
{
    menu.head = NULL;
    helper_add_menu_item("a", 10, 5);
    helper_add_menu_item("b", 20, 5);
    helper_add_menu_item("c", 30, 5);
    helper_add_menu_item("d", 40, 5);
    helper_add_menu_item("e", 50, 5);

    // Count menu items before
    int count_before = 0;
    ItemNode *curr = menu.head;
    while (curr != NULL)
    {
        count_before++;
        curr = curr->next;
    }

    ItemNode *result = rushHourSearch();

    // Count menu items after
    int count_after = 0;
    curr = menu.head;
    while (curr != NULL)
    {
        count_after++;
        curr = curr->next;
    }

    cr_assert(eq(int, count_before, count_after), "rushHourSearch should not modify the menu");

    helper_free_result(result);
    helper_free_menu();
}
