#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../restaurant.h"
#include "ag_utils.h"

TestSuite(test_calculate_table_cost, .timeout = UNREASONABLY_LONG);

Test(test_calculate_table_cost, null_head) {
    TableOrder order;
    order.head = NULL;
    order.order_time = 12;
    order.table_num = 7;
    order.guests_at_table = 4;

    cr_assert(eq(int, calculateTableCost(order), FAILURE), "Expected order with NULL heads to fail");
    helper_reset_orders();
}

Test(test_calculate_table_cost, one_item) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);
    TableOrder order;

    helper_append_node(&order_head, patty);
    order.head = order_head;
    order.order_time = 20;
    order.table_num = 3;
    order.guests_at_table = 1;

    cr_assert(eq(int, calculateTableCost(order), 12), "Expected a one item order total to match the item cost");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_calculate_table_cost, multiple_items) {
    ItemNode *order_head = NULL;
    Item *patty = helper_create_item("krabby patty", 10, 12);
    Item *shake = helper_create_item("kelp shake", 5, 4);
    Item *kelp = helper_create_item("kelp shake", 2, 6);
    helper_append_node(&order_head, patty);
    helper_append_node(&order_head, shake);
    helper_append_node(&order_head, kelp);

    TableOrder order;
    order.head = order_head;
    order.order_time = 20;
    order.table_num = 3;
    order.guests_at_table = 1;

    cr_assert(eq(int, calculateTableCost(order), 22), "Expected the order total to equal the sum of all item costs");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}

Test(test_calculate_table_cost, duplicate_items) {
    ItemNode *order_head = NULL;
    Item *first_patty = helper_create_item("krabby patty", 3, 7);
    Item *second_patty = helper_create_item("krabby patty", 3, 7);
    helper_append_node(&order_head, first_patty);
    helper_append_node(&order_head, second_patty);

    TableOrder order;
    order.head = order_head;
    order.order_time = 30;
    order.table_num = 11;
    order.guests_at_table = 2;

    cr_assert(eq(int, calculateTableCost(order), 14), "Expected repeated items to contribute once per node in the order list");
    helper_free_list(order_head, 1);
    helper_reset_orders();
}
