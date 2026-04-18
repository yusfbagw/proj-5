#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


Test(test_shutdown_restaurant, empty_restaurant) {
	helper_reset();
	cr_assert(eq(int, shutdownRestaurant(), SUCCESS), "Should succeed even with nothing to free");
	cr_assert(eq(ptr, menu.head, NULL), "Menu head should be NULL");
	cr_assert(eq(int, num_table_orders, 0), "Should have 0 orders");
}

Test(test_shutdown_restaurant, with_data) {
	helper_reset();

	Item *burger = helper_create_item("burger", 10, 8);
	Item *fries = helper_create_item("fries", 5, 3);
	ItemNode *m1 = helper_create_node(burger);
	ItemNode *m2 = helper_create_node(fries);
	m1->next = m2;
	menu.head = m1;

	ItemNode *o1 = helper_create_node(burger);
	orders[0].head = o1;
	orders[0].table_num = 1;
	orders[0].guests_at_table = 2;
	orders[0].order_time = 100;

	ItemNode *o2 = helper_create_node(fries);
	orders[1].head = o2;
	orders[1].table_num = 2;
	orders[1].guests_at_table = 3;
	orders[1].order_time = 200;
	num_table_orders = 2;

	cr_assert(eq(int, shutdownRestaurant(), SUCCESS), "Should succeed");
	cr_assert(eq(ptr, menu.head, NULL), "Menu should be NULL after shutdown");
	cr_assert(eq(int, num_table_orders, 0), "Should have 0 orders after shutdown");
}

Test(test_shutdown_restaurant, only_menu) {
	helper_reset();

	Item *a = helper_create_item("soup", 15, 7);
	Item *b = helper_create_item("salad", 5, 5);
	Item *c = helper_create_item("bread", 8, 3);
	ItemNode *n1 = helper_create_node(a);
	ItemNode *n2 = helper_create_node(b);
	ItemNode *n3 = helper_create_node(c);
	n1->next = n2;
	n2->next = n3;
	menu.head = n1;

	cr_assert(eq(int, shutdownRestaurant(), SUCCESS), "Should succeed with only menu items");
	cr_assert(eq(ptr, menu.head, NULL), "Menu should be NULL after shutdown");
	cr_assert(eq(int, num_table_orders, 0), "Should still have 0 orders");
}
