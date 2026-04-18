#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../restaurant.h"
#include "global.h"
#include "ag_utils.h"


Test(test_find_oldest_order, single_order) {
	helper_reset();
	helper_add_order(NULL, 1, 4, 100);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 100), "Single order should be returned as the oldest");
	cr_assert(eq(int, result.table_num, 1), "Table number should match the only order");
	cr_assert(eq(int, result.guests_at_table, 4), "Guests at table should match the only order");
}

Test(test_find_oldest_order, oldest_is_first) {
	helper_reset();
	helper_add_order(NULL, 1, 2, 10);
	helper_add_order(NULL, 2, 3, 20);
	helper_add_order(NULL, 3, 4, 30);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 10), "Oldest order should have order_time 10");
	cr_assert(eq(int, result.table_num, 1), "Oldest order should be table 1");
}

Test(test_find_oldest_order, oldest_is_last) {
	helper_reset();
	helper_add_order(NULL, 1, 2, 30);
	helper_add_order(NULL, 2, 3, 20);
	helper_add_order(NULL, 3, 4, 5);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 5), "Oldest order should have order_time 5");
	cr_assert(eq(int, result.table_num, 3), "Oldest order should be table 3");
}

Test(test_find_oldest_order, oldest_is_middle) {
	helper_reset();
	helper_add_order(NULL, 1, 2, 50);
	helper_add_order(NULL, 2, 3, 10);
	helper_add_order(NULL, 3, 4, 40);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 10), "Oldest order should have order_time 10");
	cr_assert(eq(int, result.table_num, 2), "Oldest order should be table 2");
}

Test(test_find_oldest_order, duplicate_oldest_times) {
	helper_reset();
	helper_add_order(NULL, 1, 2, 10);
	helper_add_order(NULL, 2, 3, 10);
	helper_add_order(NULL, 3, 4, 30);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 10), "Should return an order with the smallest order_time when there are ties");
}

Test(test_find_oldest_order, many_orders) {
	helper_reset();
	helper_add_order(NULL, 0, 1, 100);
	helper_add_order(NULL, 1, 2, 90);
	helper_add_order(NULL, 2, 3, 80);
	helper_add_order(NULL, 3, 4, 70);
	helper_add_order(NULL, 4, 5, 60);
	helper_add_order(NULL, 5, 6, 3);
	helper_add_order(NULL, 6, 7, 50);
	helper_add_order(NULL, 7, 8, 40);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 3), "Should find oldest among many orders");
	cr_assert(eq(int, result.table_num, 5), "Oldest order should be table 5");
}

Test(test_find_oldest_order, zero_order_time) {
	helper_reset();
	helper_add_order(NULL, 1, 2, 0);
	helper_add_order(NULL, 2, 3, 50);
	helper_add_order(NULL, 3, 4, 25);

	TableOrder result = findOldestOrder();
	cr_assert(eq(int, result.order_time, 0), "Order with order_time 0 should be the oldest");
	cr_assert(eq(int, result.table_num, 1), "Oldest order should be table 1");
}
