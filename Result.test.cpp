#include "Result.h"

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using namespace fun;

TEST(TestResult, testInt) {

	auto i = 10;
	auto opt1 = Result<int>(i);
	ASSERT_EQ(i, *opt1);
	ASSERT_TRUE(opt1);
	ASSERT_FALSE(!opt1);
	auto opt2 = Result<int>(opt1);
	ASSERT_EQ(i, *opt2);
	ASSERT_EQ(opt1, opt2);
	ASSERT_TRUE(opt2);
	ASSERT_FALSE(!opt2);

	auto j = 4;
	opt1 = j;
	ASSERT_NE(i, *opt1);
	ASSERT_EQ(j, *opt1);
	ASSERT_NE(opt1, opt2);

	auto k = 4;
	opt2 = k;
	ASSERT_EQ(opt1, opt2);
}

TEST(TestResult, testString) {

	auto a = std::string{"Hello"};
	Result<std::string> optA(a);
	ASSERT_TRUE(optA);
	ASSERT_EQ(*optA, "Hello");
	ASSERT_EQ(5, optA->size());

	auto b = std::string{"World"};
	Result<std::string> optB(b);
	ASSERT_TRUE(optB);
	ASSERT_NE(*optB, "Hello");
	ASSERT_EQ(5, optB->size());

	Result<std::string> optC;
	ASSERT_FALSE(optC);
	ASSERT_TRUE(optC.empty());
	ASSERT_THROW(*optC, ResultEmptyException);
	ASSERT_THROW(optC->empty(), ResultEmptyException);

	ASSERT_NE(optA, optB);
	ASSERT_NE(optB, optC);
	ASSERT_NE(optA, optC);
}


TEST(TestResult, testContainer) {

	{
		auto a = std::string{"Hello"};
		Result<std::string> optA(a);
		ASSERT_FALSE(optA.empty());
		ASSERT_EQ(1, optA.size());
		ASSERT_EQ("Hello", *(optA.begin()));
		ASSERT_NE(optA.begin(), optA.end());
		ASSERT_NE(optA.cbegin(), optA.cend());
		ASSERT_NE(optA.rbegin(), optA.rend());
		ASSERT_EQ(optA.begin() + 1, optA.end());
		ASSERT_EQ(optA.cbegin() + 1, optA.cend());
		ASSERT_EQ(optA.rbegin() + 1, optA.rend());
		auto count_loops = 0;
		for (auto& item: optA) {
			ASSERT_EQ("Hello", item);
			++count_loops;
		}
		ASSERT_EQ(1, count_loops);
		for (auto& item: optA) {
			ASSERT_EQ("Hello", item);
			++count_loops;
		}
		ASSERT_EQ(2, count_loops);
	}

	{
		Result<std::string> optB;
		ASSERT_TRUE(optB.empty());
		ASSERT_EQ(0, optB.size());
		ASSERT_EQ(optB.begin(), optB.end());
		ASSERT_EQ(optB.cbegin(), optB.cend());
		ASSERT_EQ(optB.rbegin(), optB.rend());
		auto count_loops = 0;
		for (auto& item: optB) {
			++count_loops;
		}
		ASSERT_EQ(0, count_loops);
	}
}

TEST(TestResult, testSort) {

	auto const VALUES = std::vector<int> {5, 3, 1, 0, 2, 4};
	auto results = std::vector< Result<int> >();
	for (auto& value: VALUES) {
		if (value > 0) {
			results.emplace_back(value);
			ASSERT_FALSE(results.back().empty());
			ASSERT_EQ(results.back().value(), value);
		} else {
			results.emplace_back();
			ASSERT_TRUE(results.back().empty());
		}
	}

	std::sort(std::begin(results), std::end(results));
	ASSERT_TRUE(results[0].empty());
	ASSERT_EQ(0, results[0].value_or(0));
	ASSERT_EQ(1, results[1].value());
	ASSERT_EQ(2, results[2].value());
	ASSERT_EQ(3, results[3].value());
	ASSERT_EQ(4, results[4].value());
	ASSERT_EQ(5, results[5].value());
}
