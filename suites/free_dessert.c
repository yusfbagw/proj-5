#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


Test(test_free_dessert, null_order) {
	helper_reset();
	num_table_orders = 1;
	cr_assert(eq(int, freeDessert(NULL), FAILURE), "Should fail for NULL order");
}

Test(test_free_dessert, no_orders) {
	helper_reset();
	TableOrder dummy;
	dummy.head = NULL;
	dummy.table_num = 0;
	dummy.guests_at_table = 0;
	dummy.order_time = 0;
	cr_assert(eq(int, freeDessert(&dummy), FAILURE), "Should fail when num_table_orders is 0");
}

Test(test_free_dessert, pie_on_menu) {
	helper_reset();

	Item *burger = helper_create_item("burger", 10, 8);
	Item *pie = helper_create_item("apple pie", 15, 5);
	ItemNode *m1 = helper_create_node(burger);
	ItemNode *m2 = helper_create_node(pie);
	m1->next = m2;
	menu.head = m1;

	ItemNode *order_node = helper_create_node(burger);
	orders[0].head = order_node;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	int prev_count = pie->order_count;

	cr_assert(eq(int, freeDessert(&orders[0]), SUCCESS), "Should succeed");

	cr_assert(not(zero(ptr, orders[0].head->next)), "Order should have 2 items now");
	cr_assert(eq(str, orders[0].head->next->item->name, "apple pie"), "Second item should be apple pie");
	cr_assert(eq(int, pie->cost, 0), "Pie cost should be set to 0");
	cr_assert(eq(int, pie->order_count, prev_count + 1), "Pie order_count should be incremented");
	cr_assert(eq(ptr, orders[0].head->next->next, NULL), "Should only have 2 items");

	helper_free_nodes(orders[0].head);
	num_table_orders = 0;
	helper_free_menu();
}

Test(test_free_dessert, pie_not_on_menu) {
	helper_reset();

	Item *burger = helper_create_item("burger", 10, 8);
	ItemNode *m1 = helper_create_node(burger);
	menu.head = m1;

	ItemNode *order_node = helper_create_node(burger);
	orders[0].head = order_node;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	cr_assert(eq(int, freeDessert(&orders[0]), SUCCESS), "Should succeed");

	int found = 0;
	ItemNode *curr = menu.head;
	while (curr != NULL) {
		if (strncmp(curr->item->name, "apple pie", MAX_ITEM_LEN) == 0) {
			found = 1;
			break;
		}
		curr = curr->next;
	}
	cr_assert(eq(int, found, 1), "Apple pie should have been added to the menu");

	cr_assert(not(zero(ptr, orders[0].head->next)), "Order should have 2 items");
	cr_assert(eq(str, orders[0].head->next->item->name, "apple pie"), "Second item should be apple pie");

	helper_free_nodes(orders[0].head);
	num_table_orders = 0;
	helper_free_menu();
}

Test(test_free_dessert, empty_order) {
	helper_reset();

	Item *pie = helper_create_item("apple pie", 15, 0);
	ItemNode *m1 = helper_create_node(pie);
	menu.head = m1;

	orders[0].head = NULL;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	cr_assert(eq(int, freeDessert(&orders[0]), SUCCESS), "Should succeed");
	cr_assert(not(zero(ptr, orders[0].head)), "Pie should become the head of the order");
	cr_assert(eq(str, orders[0].head->item->name, "apple pie"), "Only item should be apple pie");
	cr_assert(eq(ptr, orders[0].head->next, NULL), "Should only have 1 item");

	helper_free_nodes(orders[0].head);
	num_table_orders = 0;
	helper_free_menu();
}

Test(test_free_dessert, malloc_fail) {
	helper_reset();

	Item *pie = helper_create_item("apple pie", 15, 0);
	ItemNode *m1 = helper_create_node(pie);
	menu.head = m1;

	ItemNode *order_node = helper_create_node(pie);
	orders[0].head = order_node;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;
	num_table_orders = 1;

	bytes_until_fail = 0;
	cr_assert(eq(int, freeDessert(&orders[0]), FAILURE), "Should fail on malloc failure");
	bytes_until_fail = -1;

	cr_assert(eq(ptr, orders[0].head->next, NULL), "Order should still have only 1 item");

	free(order_node);
	num_table_orders = 0;
	helper_free_menu();
}
