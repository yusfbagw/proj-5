#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../restaurant.h"
#include "ag_utils.h"

TestSuite(test_remove_from_menu, .timeout = UNREASONABLY_LONG);

Test(test_remove_from_menu, null_item) {
    Item *patty = helper_create_item("krabby patty", 7, 8);
    Item *shake = helper_create_item("kelp shake", 6, 10);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, shake);

    cr_assert(eq(int, removeFromMenu(NULL), FAILURE), "Expected NULL item removal to fail");
    cr_assert(eq(ptr, menu.head->item, patty), "Head should remain unchanged after failed removal");
    cr_assert(eq(ptr, menu.head->next->item, shake), "Next item should remain unchanged after failed removal");
    helper_reset_menu();
}

Test(test_remove_from_menu, item_not_in_menu) {
    Item *patty = helper_create_item("krabby patty", 7, 8);
    Item *shake = helper_create_item("kelp shake", 6, 10);
    Item *kelp = helper_create_item("kelp shake", 1, 1);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, shake);

    cr_assert(eq(int, removeFromMenu(kelp), FAILURE), "Expected removal of an item not in the menu to fail");
    cr_assert(eq(ptr, menu.head->item, patty), "Head should remain unchanged after failed removal");
    cr_assert(eq(ptr, menu.head->next->item, shake), "Next item should remain unchanged after failed removal");
    free(kelp);
    helper_reset_menu();
}

Test(test_remove_from_menu, head_item) {
    Item *patty = helper_create_item("krabby patty", 10, 14);
    Item *shake = helper_create_item("kelp shake", 1, 3);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, shake);

    cr_assert(eq(int, removeFromMenu(patty), SUCCESS), "Expected removing the head item to succeed");
    cr_assert(eq(ptr, menu.head->item, shake), "Expected head be the previous second node");
    cr_assert(zero(ptr, menu.head->next), "Expected only one node left after removing");
    free(patty);
    helper_reset_menu();
}

Test(test_remove_from_menu, tail_item) {
    Item *patty = helper_create_item("krabby patty", 10, 14);
    Item *shake = helper_create_item("kelp shake", 1, 3);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, shake);

    cr_assert(eq(int, removeFromMenu(shake), SUCCESS), "Expected removing the tail item to succeed");
    cr_assert(eq(ptr, menu.head->item, patty), "Expected head to remain unchanged after tail removal");
    cr_assert(zero(ptr, menu.head->next), "Expected only one node left after removing");
    free(shake);
    helper_reset_menu();
}

Test(test_remove_from_menu, middle_item) {
    Item *patty = helper_create_item("krabby patty", 4, 5);
    Item *kelp = helper_create_item("kelp shake", 8, 9);
    Item *shake = helper_create_item("kelp shake", 2, 6);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, kelp);
    helper_append_node(&menu.head, shake);

    cr_assert(eq(int, removeFromMenu(kelp), SUCCESS), "Expected removing a middle node to succeed");
    cr_assert(eq(ptr, menu.head->item, patty), "Expected head to remain unchanged after middle removal");
    cr_assert(eq(ptr, menu.head->next->item, shake), "Expected head node to link to the node after removed node");
    cr_assert(zero(ptr, menu.head->next->next), "Expected only two nodes left after removing");
    free(kelp);
    helper_reset_menu();
}
