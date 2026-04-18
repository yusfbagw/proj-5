#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "ag_utils.h"

TestSuite(test_add_table_order, .timeout = UNREASONABLY_LONG);

Test(test_add_table_order, null_head) {
    cr_assert(eq(int, addTableOrder(NULL, 1, 2, 3), FAILURE), "Expected NULL order list to fail");
    cr_assert(eq(int, num_table_orders, 0), "Expected invalid input to not change num_table_orders");
    helper_reset_orders();
}

Test(test_add_table_order, negative_table_number) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);

    helper_append_node(&order_head, patty);
    cr_assert(eq(int, addTableOrder(order_head, -1, 2, 3), FAILURE), "Expected negative table number to fail");
    cr_assert(eq(int, num_table_orders, 0), "Expected invalid input to not change num_table_orders");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_add_table_order, negative_guest_count) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);

    helper_append_node(&order_head, patty);
    cr_assert(eq(int, addTableOrder(order_head, 1, -1, 3), FAILURE), "Expected negative guest count to fail");
    cr_assert(eq(int, num_table_orders, 0), "Expected invalid input to not change num_table_orders");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_add_table_order, negative_order_time) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);

    helper_append_node(&order_head, patty);
    cr_assert(eq(int, addTableOrder(order_head, 1, 2, -1), FAILURE), "Expected negative order time to fail");
    cr_assert(eq(int, num_table_orders, 0), "Expected invalid input to not change num_table_orders");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_add_table_order, full_capacity) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);

    helper_append_node(&order_head, patty);
    num_table_orders = MAX_ORDER_COUNT;
    cr_assert(eq(int, addTableOrder(order_head, 4, 4, 4), FAILURE), "Expected full order array to fail");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_add_table_order, valid_order) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);
    Item *shake = helper_create_item("kelp shake", 5, 4);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);
    helper_append_node(&menu.head, shake);

    helper_append_node(&order_head, patty);
    helper_append_node(&order_head, shake);
    helper_append_node(&order_head, patty);

    cr_assert(eq(int, addTableOrder(order_head, 17, 3, 42), SUCCESS), "Expected valid table order to succeed");
    cr_assert(eq(int, num_table_orders, 1), "Expected num_table_orders to be incremented");
    cr_assert(eq(ptr, orders[0].head, order_head), "Expected stored order head to match the provided list head");
    cr_assert(eq(int, orders[0].table_num, 17), "Expected table number to be stored");
    cr_assert(eq(int, orders[0].guests_at_table, 3), "Expected guest count to be stored");
    cr_assert(eq(int, orders[0].order_time, 42), "Expected order time to be stored");
    cr_assert(eq(int, patty->order_count, 2), "Expected duplicate ordered items to increment count for each occurrence");
    cr_assert(eq(int, shake->order_count, 1), "Expected each ordered item to increment its order count");
    helper_reset_orders();
    helper_reset_menu();
}

Test(test_add_table_order, missing_menu_item) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);
    Item *kelp = helper_create_item("kelp shake", 5, 4);

    menu.head = NULL;
    helper_append_node(&menu.head, patty);

    helper_append_node(&order_head, patty);
    helper_append_node(&order_head, kelp);

    cr_assert(eq(int, addTableOrder(order_head, 9, 2, 15), FAILURE), "Expected orders containing off-menu items to fail");
    cr_assert(eq(int, num_table_orders, 0), "Expected failed orders to not change num_table_orders");
    cr_assert(eq(int, patty->order_count, 0), "Failed orders should not partially increment menu item counts");
    helper_free_list(order_head, 0);
    free(kelp);
    helper_reset_menu();
    helper_reset_orders();
}
