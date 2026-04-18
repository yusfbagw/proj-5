#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


Test(test_free_table_order, negative_table_num) {
	helper_reset();
	cr_assert(eq(int, freeTableOrder(-1), FAILURE), "Should fail for negative table num");
}

Test(test_free_table_order, not_found) {
	helper_reset();

	Item *item = helper_create_item("burger", 10, 8);
	ItemNode *node = helper_create_node(item);
	orders[0].head = node;
	orders[0].table_num = 5;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	cr_assert(eq(int, freeTableOrder(99), FAILURE), "Table 99 doesn't exist");
	cr_assert(eq(int, num_table_orders, 1), "num_table_orders should not change");

	free(node);
	free(item);
}

Test(test_free_table_order, single_order) {
	helper_reset();

	Item *item = helper_create_item("pasta", 20, 12);
	ItemNode *node = helper_create_node(item);
	orders[0].head = node;
	orders[0].table_num = 3;
	orders[0].guests_at_table = 4;
	orders[0].order_time = 50;
	num_table_orders = 1;

	cr_assert(eq(int, freeTableOrder(3), SUCCESS), "Should succeed");
	cr_assert(eq(int, num_table_orders, 0), "Should have 0 orders after free");
}

Test(test_free_table_order, shared_with_menu) {
	helper_reset();

	Item *burger = helper_create_item("burger", 10, 8);

	ItemNode *menu_node = helper_create_node(burger);
	menu.head = menu_node;

	ItemNode *order_node = helper_create_node(burger);
	orders[0].head = order_node;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	cr_assert(eq(int, freeTableOrder(1), SUCCESS), "Should succeed");
	cr_assert(eq(int, num_table_orders, 0), "Should have 0 orders");
	cr_assert(eq(str, menu.head->item->name, "burger"), "Menu item should still exist");

	helper_free_menu();
}

Test(test_free_table_order, compaction) {
	helper_reset();

	Item *a = helper_create_item("item_a", 5, 5);
	Item *b = helper_create_item("item_b", 10, 10);
	Item *c = helper_create_item("item_c", 15, 15);

	ItemNode *m1 = helper_create_node(a);
	ItemNode *m2 = helper_create_node(b);
	ItemNode *m3 = helper_create_node(c);
	m1->next = m2;
	m2->next = m3;
	menu.head = m1;

	ItemNode *n1 = helper_create_node(a);
	orders[0].head = n1;
	orders[0].table_num = 10;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;

	ItemNode *n2 = helper_create_node(b);
	orders[1].head = n2;
	orders[1].table_num = 20;
	orders[1].guests_at_table = 3;
	orders[1].order_time = 200;

	ItemNode *n3 = helper_create_node(c);
	orders[2].head = n3;
	orders[2].table_num = 30;
	orders[2].guests_at_table = 4;
	orders[2].order_time = 300;
	num_table_orders = 3;

	cr_assert(eq(int, freeTableOrder(10), SUCCESS), "Should succeed");
	cr_assert(eq(int, num_table_orders, 2), "Should have 2 orders left");

	// the tests below will evaluate to true if it is found in either spot
	cr_assert(
		orders[0].table_num == 30 ||
		orders[1].table_num == 30,
		"Expected to find table 30 in orders"
	); 
	cr_assert(
		orders[0].table_num == 20 ||
		orders[1].table_num == 20,
		"Expected to find table 20 in orders"
	);

	helper_free_nodes(orders[0].head);
	helper_free_nodes(orders[1].head);
	num_table_orders = 0;
	helper_free_menu();
}
