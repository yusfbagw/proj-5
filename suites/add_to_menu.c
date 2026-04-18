#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "ag_utils.h"
#include "../restaurant.h"


TestSuite(test_add_to_menu, .timeout = UNREASONABLY_LONG);

Test(test_add_to_menu, null_name) {
    cr_assert(eq(int, addToMenu(NULL, 0, 0), FAILURE), "Expected NULL name to fail");
    cr_assert(zero(ptr, menu.head), "Menu should remain empty after invalid input");
    helper_reset_menu();
}

Test(test_add_to_menu, negative_cook_time) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    cr_assert(eq(int, addToMenu(name, -1, 0), FAILURE), "Expected negative cook time to fail");
    cr_assert(zero(ptr, menu.head), "Menu should remain empty after invalid input");
    helper_reset_menu();
}

Test(test_add_to_menu, negative_cost) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    cr_assert(eq(int, addToMenu(name, 10, -1.0f), FAILURE), "Expected negative cost to fail");
    cr_assert(zero(ptr, menu.head), "Menu should remain empty after invalid input");
    helper_reset_menu();
}

Test(test_add_to_menu, one_valid_item) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    cr_assert(eq(int, addToMenu(name, 12, 9.0f), SUCCESS), "Expected addToMenu to succeed on valid input");
    cr_assert(not(zero(ptr, menu.head)), "Menu head should be initialized");
    cr_assert(eq(str, menu.head->item->name, "krabby patty"), "Expected inserted item name to match");
    cr_assert(eq(int, menu.head->item->cook_time, 12), "Expected inserted cook time to match");
    cr_assert(eq(int, menu.head->item->cost, 9), "Expected inserted cost to match");
    cr_assert(eq(int, menu.head->item->order_count, 0), "Expected new items to start with zero orders");
    cr_assert(zero(ptr, menu.head->next), "Expected single inserted item to be the only node");
    helper_reset_menu();
}

Test(test_add_to_menu, two_valid_items) {
    char patty_name[MAX_ITEM_LEN] = "krabby patty";
    char shake_name[MAX_ITEM_LEN] = "kelp shake";

    cr_assert(eq(int, addToMenu(patty_name, 5, 4.0f), SUCCESS), "First add should succeed");
    cr_assert(eq(int, addToMenu(shake_name, 3, 6.0f), SUCCESS), "Second add should succeed");
    cr_assert(eq(str, menu.head->item->name, "krabby patty"), "Head should remain the first item");
    cr_assert(not(zero(ptr, menu.head->next)), "Expected a second node to be after head");
    cr_assert(eq(str, menu.head->next->item->name, "kelp shake"), "Expected second item to be appended after first");
    cr_assert(zero(ptr, menu.head->next->next), "Expected exactly two nodes after two adds");
    helper_reset_menu();
}

Test(test_add_to_menu, duplicate_names) {
    char patty_name[MAX_ITEM_LEN] = "krabby patty";
    char dup_patty_name[MAX_ITEM_LEN] = "krabby patty";

    cr_assert(eq(int, addToMenu(patty_name, 5, 4.0f), SUCCESS), "First add should succeed");
    cr_assert(eq(int, addToMenu(dup_patty_name, 99, 99.0f), FAILURE), "Duplicate item should be rejected");
    cr_assert(eq(str, menu.head->item->name, "krabby patty"), "Duplicate add should not replace the existing item");
    cr_assert(zero(ptr, menu.head->next), "Duplicate add should not add a new node");
    helper_reset_menu();
}

Test(test_add_to_menu, malloc_fail_zero) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    bytes_until_fail = 0;
    cr_assert(eq(int, addToMenu(name, 20, 18.0f), FAILURE), "Expected malloc failure for item allocation to fail");
    cr_assert(zero(ptr, menu.head), "Menu should stay empty when allocation fails");
    helper_reset_menu();
}

Test(test_add_to_menu, malloc_fail_item) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    bytes_until_fail = (int) sizeof(Item);
    cr_assert(eq(int, addToMenu(name, 14, 13.0f), FAILURE), "Expected malloc failure for ItemNode allocation to fail");
    cr_assert(zero(ptr, menu.head), "Menu should stay empty when the node allocation fails");
    helper_reset_menu();
}

Test(test_add_to_menu, malloc_success) {
    char name[MAX_ITEM_LEN] = "krabby patty";

    bytes_until_fail = (int) (sizeof(Item) + sizeof(ItemNode));
    cr_assert(eq(int, addToMenu(name, 14, 13.0f), SUCCESS), "Expected success with exactly enough allocation bytes");
    helper_reset_menu();
}
