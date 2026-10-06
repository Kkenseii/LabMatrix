#include "pch.h"
#include <sstream>
#include <random>
#include <deque>
#include <string>
#include "memdata.h"
#include "tvector.h"
#include "tmathvector.h"
#include "tmatrix.h"

#define MEMDATA_TESTS 0
#define VECTOR_TESTS 0

#ifdef MEMDATA_TESTS

TEST(FunctionsForMemData, calculate_capacity) {
	EXPECT_EQ(0, calculate_capacity(0));
	EXPECT_EQ(15, calculate_capacity(1));
	EXPECT_EQ(15, calculate_capacity(15));
	EXPECT_EQ(30, calculate_capacity(16));
	EXPECT_EQ(30, calculate_capacity(30));
	EXPECT_EQ(45, calculate_capacity(31));
	EXPECT_EQ(45, calculate_capacity(45));
}

TEST(ClassMemData, can_create_with_default_constructor) {
	MemData<double> mem;

	EXPECT_EQ(0, mem.size());
	EXPECT_EQ(0, mem.capacity());
	EXPECT_TRUE(mem.is_empty());
}

TEST(ClassMemData, can_create_with_constructor_by_size) {
	MemData<double> mem(10);
	EXPECT_EQ(10, mem.size());
	EXPECT_EQ(15, mem.capacity());
}

TEST(ClassMemData, can_create_with_initializer_list) {
	MemData<double> mem({ 1.1, 2.2, 3.3 });

	EXPECT_EQ(3, mem.size());
	EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
	EXPECT_DOUBLE_EQ(2.2, mem.data()[1]);
	EXPECT_DOUBLE_EQ(3.3, mem.data()[2]);
}

TEST(ClassMemData, can_create_with_array) {
	double arr[] = { 1.1, 2.2, 3.3 };
	MemData<double> mem(arr, 3);

	EXPECT_EQ(3, mem.size());
	EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, copy_constructor) {
	MemData<double> a({ 1.1, 2.2 });
	MemData<double> b(a);

	EXPECT_EQ(a.size(), b.size());
	EXPECT_NE(a.data(), b.data());
}

TEST(ClassMemData, move_constructor) {
	MemData<double> a({ 1.1, 2.2 });
	const double* ptr = a.data();

	MemData<double> b(std::move(a));

	EXPECT_EQ(ptr, b.data());
	EXPECT_EQ(0, a.size());
}

TEST(ClassMemData, set_memory) {
	MemData<double> mem({ 1.1, 2.2 });
	mem.set_memory(30);

	EXPECT_EQ(30, mem.capacity());
	EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, reset_memory) {
	MemData<double> mem({ 1.1, 2.2, 3.3 });
	mem.reset_memory(30);

	EXPECT_EQ(30, mem.capacity());
	EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, clear_memory) {
	MemData<double> mem({ 1.1, 2.2 });
	mem.clear_memory();

	EXPECT_EQ(0, mem.size());
	EXPECT_EQ(nullptr, mem.data());
}

TEST(ClassMemData, assignment) {
	MemData<double> a({ 1.1, 2.2 });
	MemData<double> b;
	b = a;

	EXPECT_EQ(a.size(), b.size());
	EXPECT_NE(a.data(), b.data());
}

TEST(ClassMemData, move_assignment) {
	MemData<double> a({ 1.1, 2.2 });
	MemData<double> b;

	b = std::move(a);

	EXPECT_EQ(0, a.size());
	EXPECT_EQ(2, b.size());
}

#endif

#ifdef VECTOR_TESTS

TEST(ClassVector, default_constructor) {
	TVector<double> v;
	EXPECT_TRUE(v.is_empty());
}

TEST(ClassVector, size_constructor) {
	TVector<double> v(10);
	EXPECT_EQ(10, v.size());
}

TEST(ClassVector, initializer_list) {
	TVector<double> v({ 1,2,3 });
	EXPECT_DOUBLE_EQ(2, v[1]);
}

TEST(ClassVector, push_back) {
	TVector<double> v;
	v.push_back(1);
	v.push_back(2);

	EXPECT_EQ(2, v.size());
	EXPECT_DOUBLE_EQ(2, v.back());
}

TEST(ClassVector, push_front) {
	TVector<double> v;
	v.push_front(2);
	v.push_front(1);

	EXPECT_DOUBLE_EQ(1, v.front());
}

TEST(ClassVector, insert) {
	TVector<double> v({ 1,2,4 });
	v.insert(3, 1);

	EXPECT_DOUBLE_EQ(3, v[1]);
}

TEST(ClassVector, pop_back) {
	TVector<double> v({ 1,2,3 });
	v.pop_back();

	EXPECT_EQ(2, v.size());
}

TEST(ClassVector, pop_front) {
	TVector<double> v({ 1,2,3 });
	v.pop_front();

	EXPECT_DOUBLE_EQ(2, v[0]);
}

TEST(ClassVector, erase) {
	TVector<double> v({ 1,2,3 });
	v.erase(1);

	EXPECT_DOUBLE_EQ(3, v[1]);
}

TEST(ClassVector, sort) {
	TVector<double> v({ 3,1,2 });
	v.sort();

	EXPECT_DOUBLE_EQ(1, v[0]);
	EXPECT_DOUBLE_EQ(2, v[1]);
	EXPECT_DOUBLE_EQ(3, v[2]);
}

TEST(ClassVector, shuffle) {
	TVector<double> v({ 1,2,3,4,5 });
	v.shuffle();

	EXPECT_EQ(5, v.size());
}

TEST(ClassVector, copy_assignment) {
	TVector<double> a({ 1,2 });
	TVector<double> b;

	b = a;

	EXPECT_EQ(a.size(), b.size());
}

TEST(ClassVector, move_assignment) {
	TVector<double> a({ 1,2 });
	TVector<double> b;

	b = std::move(a);

	EXPECT_EQ(0, a.size());
}

#endif

TEST(MemData, DefaultIsEmpty) {
	MemData<int> m;
	EXPECT_TRUE(m.is_empty());
	EXPECT_EQ(m.size(), 0u);
	EXPECT_EQ(m.capacity(), 0u);
	EXPECT_TRUE(m.data() == nullptr);
}

TEST(MemData, CapacityIsMultipleOfStep) {
	EXPECT_EQ(calculate_capacity(0), 0u);
	EXPECT_EQ(calculate_capacity(1), (size_t)MEM_STEP);
	EXPECT_EQ(calculate_capacity(MEM_STEP), (size_t)MEM_STEP);
	EXPECT_EQ(calculate_capacity(MEM_STEP + 1), (size_t)(2 * MEM_STEP));
}

TEST(MemData, SizeConstructorZeroFills) {
	MemData<double> m(5);
	EXPECT_EQ(m.size(), 5u);
	EXPECT_EQ(m.capacity(), (size_t)MEM_STEP);
	for (size_t i = 0; i < 5; i++) EXPECT_EQ(m.data()[i], 0.0);
}

TEST(MemData, InitializerListConstructor) {
	MemData<int> m{ 1, 2, 3 };
	EXPECT_EQ(m.size(), 3u);
	EXPECT_EQ(m.data()[0], 1);
	EXPECT_EQ(m.data()[2], 3);
}

TEST(MemData, ArrayConstructor) {
	int a[] = { 4, 5, 6, 7 };
	MemData<int> m(a, 4);
	EXPECT_EQ(m.size(), 4u);
	EXPECT_EQ(m.data()[3], 7);

	MemData<int> e(nullptr, 4);
	EXPECT_TRUE(e.is_empty());
}

TEST(MemData, CopyIsDeep) {
	MemData<int> a{ 1, 2, 3 };
	MemData<int> b(a);
	EXPECT_EQ(b.size(), 3u);
	EXPECT_TRUE(a.data() != b.data());
	EXPECT_EQ(b.data()[1], 2);
}

TEST(MemData, MoveStealsMemory) {
	MemData<int> a{ 1, 2, 3 };
	MemData<int> b(std::move(a));
	EXPECT_EQ(b.size(), 3u);
	EXPECT_TRUE(a.is_empty());
	EXPECT_EQ(a.capacity(), 0u);
}

TEST(MemData, CopyAndMoveAssignment) {
	MemData<int> a{ 1, 2, 3 };
	MemData<int> b;
	b = a;
	EXPECT_EQ(b.size(), 3u);
	b = b;
	EXPECT_EQ(b.size(), 3u);
	MemData<int> c;
	c = std::move(b);
	EXPECT_EQ(c.data()[2], 3);
	EXPECT_TRUE(b.is_empty());
}

TEST(MemData, IsFull) {
	MemData<int> m;
	EXPECT_TRUE(m.is_full());
	MemData<int> k(MEM_STEP);
	EXPECT_TRUE(k.is_full());
	MemData<int> p(3);
	EXPECT_FALSE(p.is_full());
}

TEST(MemData, SetMemoryKeepsDataAndTruncates) {
	MemData<int> m{ 1, 2, 3, 4 };
	m.set_memory(30);
	EXPECT_EQ(m.capacity(), 30u);
	EXPECT_EQ(m.size(), 4u);
	EXPECT_EQ(m.data()[3], 4);
	m.set_memory(2);
	EXPECT_EQ(m.capacity(), 2u);
	EXPECT_EQ(m.size(), 2u);
	EXPECT_EQ(m.data()[1], 2);
	m.set_memory(0);
	EXPECT_EQ(m.size(), 0u);
	EXPECT_TRUE(m.data() == nullptr);
}

TEST(MemData, ResetMemoryWithOffset) {
	MemData<int> m{ 1, 2, 3 };
	m.reset_memory(10, 2);
	EXPECT_EQ(m.capacity(), 10u);
	EXPECT_EQ(m.data()[0], 0);
	EXPECT_EQ(m.data()[2], 1);
	EXPECT_EQ(m.data()[4], 3);
}

TEST(MemData, ClearMemory) {
	MemData<int> m{ 1, 2, 3 };
	m.clear_memory();
	EXPECT_TRUE(m.is_empty());
	EXPECT_EQ(m.capacity(), 0u);
	EXPECT_TRUE(m.data() == nullptr);
}

TEST(TVector, DefaultIsEmpty) {
	TVector<int> v;
	EXPECT_TRUE(v.is_empty());
	EXPECT_EQ(v.size(), 0u);
	EXPECT_EQ(v.capacity(), 0u);
	EXPECT_THROW(v.front(), std::out_of_range);
	EXPECT_THROW(v.back(), std::out_of_range);
	EXPECT_THROW(v[0], std::out_of_range);
}

TEST(TVector, Constructors) {
	TVector<int> a(5);
	EXPECT_EQ(a.size(), 5u);
	EXPECT_EQ(a[4], 0);

	TVector<int> b{ 1, 2, 3 };
	EXPECT_EQ(b.size(), 3u);
	EXPECT_EQ(b.front(), 1);
	EXPECT_EQ(b.back(), 3);

	int arr[] = { 7, 8 };
	TVector<int> c(arr, 2);
	EXPECT_EQ(c[1], 8);

	TVector<int> d(nullptr, 3);
	EXPECT_TRUE(d.is_empty());
}

TEST(TVector, PushBackAndFront) {
	TVector<int> v;
	v.push_back(2);
	v.push_back(3);
	v.push_front(1);
	v.push_front(0);
	EXPECT_EQ(v.size(), 4u);
	for (int i = 0; i < 4; i++) EXPECT_EQ(v[i], i);
	EXPECT_EQ(v.front(), 0);
	EXPECT_EQ(v.back(), 3);
}

TEST(TVector, GrowsByMemStep) {
	TVector<int> v;
	for (size_t i = 0; i < MEM_STEP; i++) v.push_back((int)i);
	EXPECT_EQ(v.capacity(), (size_t)MEM_STEP);
	EXPECT_TRUE(v.is_full());
	v.push_back(100);
	EXPECT_EQ(v.capacity(), (size_t)(2 * MEM_STEP));
	EXPECT_EQ(v[MEM_STEP], 100);
}

TEST(TVector, Insert) {
	TVector<int> v{ 1, 2, 4, 5 };
	v.insert(3, 2);
	EXPECT_EQ(v.size(), 5u);
	for (int i = 0; i < 5; i++) EXPECT_EQ(v[i], i + 1);
	v.insert(0, 0);
	v.insert(6, v.size());
	EXPECT_EQ(v.front(), 0);
	EXPECT_EQ(v.back(), 6);
	EXPECT_THROW(v.insert(1, 100), std::out_of_range);
}

TEST(TVector, InsertNearFrontAndBack) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i * 10);
	v.insert(5, 1);
	v.insert(95, 10);
	EXPECT_EQ(v[1], 5);
	EXPECT_EQ(v[10], 95);
	EXPECT_EQ(v.size(), 12u);
	EXPECT_EQ(v[0], 0);
	EXPECT_EQ(v[2], 10);
	EXPECT_EQ(v[11], 90);
}

TEST(TVector, PopBackAndFront) {
	TVector<int> v{ 1, 2, 3 };
	v.pop_back();
	v.pop_front();
	EXPECT_EQ(v.size(), 1u);
	EXPECT_EQ(v[0], 2);
	v.pop_back();
	EXPECT_TRUE(v.is_empty());
	EXPECT_THROW(v.pop_back(), std::out_of_range);
	EXPECT_THROW(v.pop_front(), std::out_of_range);
}

TEST(TVector, Erase) {
	TVector<int> v{ 0, 1, 2, 3, 4, 5, 6 };
	v.erase(2);
	v.erase(4);
	EXPECT_EQ(v.size(), 5u);
	int expected[] = { 0, 1, 3, 4, 6 };
	for (int i = 0; i < 5; i++) EXPECT_EQ(v[i], expected[i]);
	v.erase(0);
	v.erase(v.size() - 1);
	EXPECT_EQ(v.front(), 1);
	EXPECT_EQ(v.back(), 4);
	EXPECT_THROW(v.erase(10), std::out_of_range);
}

TEST(TVector, ShrinksAfterManyPops) {
	TVector<int> v;
	for (int i = 0; i < 100; i++) v.push_back(i);
	size_t big = v.capacity();
	while (v.size() > 5) v.pop_back();
	EXPECT_LT(v.capacity(), big);
	EXPECT_EQ(v.capacity(), (size_t)MEM_STEP);
}

TEST(TVector, IndexOperatorWritesAndThrows) {
	TVector<int> v(3);
	v[1] = 42;
	EXPECT_EQ(v[1], 42);
	EXPECT_THROW(v[3], std::out_of_range);
	const TVector<int>& c = v;
	EXPECT_EQ(c[1], 42);
	EXPECT_THROW(c[3], std::out_of_range);
}

TEST(TVector, CopyOfWrappedVector) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i);
	for (int i = 0; i < 12; i++) v.push_front(-1 - i);
	TVector<int> c(v);
	TVector<int> a;
	a = v;
	EXPECT_EQ(c.size(), v.size());
	for (size_t i = 0; i < v.size(); i++) {
		EXPECT_EQ(c[i], v[i]);
		EXPECT_EQ(a[i], v[i]);
	}
	c[0] = 777;
	EXPECT_NE(v[0], 777);
}

TEST(TVector, SortWrappedVector) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(100 - i);
	for (int i = 0; i < 12; i++) v.push_front(i * 7 % 13);
	v.sort();
	for (size_t i = 1; i < v.size(); i++) EXPECT_TRUE(v[i - 1] <= v[i]);
	EXPECT_EQ(v.size(), 22u);
}

TEST(TVector, SortSmall) {
	TVector<int> e;
	e.sort();
	TVector<int> v{ 3, 1, 2 };
	v.sort();
	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[2], 3);
}

TEST(TVector, ShuffleKeepsElements) {
	TVector<int> e;
	EXPECT_NO_THROW(e.shuffle());
	TVector<int> v;
	for (int i = 0; i < 50; i++) v.push_back(i);
	v.shuffle();
	EXPECT_EQ(v.size(), 50u);
	v.sort();
	for (int i = 0; i < 50; i++) EXPECT_EQ(v[i], i);
}

TEST(TVector, MoveConstructorAndAssignment) {
	TVector<int> a{ 1, 2, 3 };
	TVector<int> b(std::move(a));
	EXPECT_EQ(b.size(), 3u);
	EXPECT_TRUE(a.is_empty());
	a.push_back(9);
	EXPECT_EQ(a[0], 9);
	TVector<int> c;
	c = std::move(b);
	EXPECT_EQ(c.back(), 3);
	EXPECT_TRUE(b.is_empty());
}

TEST(TVector, SelfAssignment) {
	TVector<int> a{ 1, 2, 3 };
	a = a;
	EXPECT_EQ(a.size(), 3u);
	EXPECT_EQ(a[2], 3);
}

TEST(TVector, PushBackOfOwnElement) {
	TVector<int> v;
	for (int i = 0; i < (int)MEM_STEP; i++) v.push_back(i);
	v.push_back(v[3]);
	EXPECT_EQ(v.back(), 3);
}

TEST(TVector, Clear) {
	TVector<int> v{ 1, 2, 3 };
	v.clear();
	EXPECT_TRUE(v.is_empty());
	EXPECT_EQ(v.capacity(), 0u);
	v.push_back(5);
	EXPECT_EQ(v[0], 5);
}

TEST(TVector, OutputAndInput) {
	TVector<int> v{ 1, 2, 3 };
	std::ostringstream os;
	os << v;
	EXPECT_EQ(os.str(), "{ 1, 2, 3 }");

	std::istringstream in("3 4 5 6");
	TVector<int> r;
	in >> r;
	EXPECT_EQ(r.size(), 3u);
	EXPECT_EQ(r[2], 6);

	std::istringstream bad("3 1 x");
	TVector<int> keep{ 9 };
	bad >> keep;
	EXPECT_EQ(keep.size(), 1u);
	EXPECT_EQ(keep[0], 9);
}

TEST(TVector, WorksWithNonPodTypes) {
	TVector<TVector<int>> m;
	m.push_back(TVector<int>{ 1, 2 });
	m.push_back(TVector<int>{ 3 });
	EXPECT_EQ(m.size(), 2u);
	EXPECT_EQ(m[0][1], 2);
	m.push_front(TVector<int>(4));
	EXPECT_EQ(m[0].size(), 4u);
	EXPECT_EQ(m[2][0], 3);
}

TEST(TVector, RandomOperationsAgainstDeque) {
	std::mt19937 g(12345);
	TVector<int> v;
	std::deque<int> d;
	for (int step = 0; step < 20000; step++) {
		int x = (int)(g() % 1000);
		switch (g() % 7) {
		case 0: v.push_back(x); d.push_back(x); break;
		case 1: v.push_front(x); d.push_front(x); break;
		case 2: { size_t p = g() % (d.size() + 1); v.insert(x, p); d.insert(d.begin() + p, x); break; }
		case 3: if (!d.empty()) { v.pop_back(); d.pop_back(); } break;
		case 4: if (!d.empty()) { v.pop_front(); d.pop_front(); } break;
		case 5: if (!d.empty()) { size_t p = g() % d.size(); v.erase(p); d.erase(d.begin() + p); } break;
		case 6: { TVector<int> c(v); v = c; break; }
		}
		ASSERT_EQ(v.size(), d.size());
	}
	for (size_t i = 0; i < d.size(); i++) EXPECT_EQ(v[i], d[i]);
}

TEST(TVectorIterator, ConstructorsAndComparison) {
	TVector<int> v{ 1, 2, 3 };
	TVector<int>::iterator a;
	TVector<int>::iterator b(&v[0]);
	TVector<int>::iterator c(b);
	EXPECT_TRUE(b == c);
	EXPECT_FALSE(b != c);
	a = b;
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(v.begin() != v.end());
}

TEST(TVectorIterator, FillAndPrintLikeInAssignment) {
	TVector<int> my_vec(8);
	int val = 1;
	for (TVector<int>::iterator it = my_vec.begin(); it != my_vec.end(); it++) {
		*it = val++;
	}
	std::string s;
	for (TVector<int>::const_iterator it = my_vec.cbegin(); it != my_vec.cend(); it++) {
		s += std::to_string(*it) + " ";
	}
	EXPECT_EQ(s, "1 2 3 4 5 6 7 8 ");
}

TEST(TVectorIterator, IncrementDecrement) {
	TVector<int> v{ 10, 20, 30 };
	TVector<int>::iterator it = v.begin();
	EXPECT_EQ(*it++, 10);
	EXPECT_EQ(*it, 20);
	EXPECT_EQ(*++it, 30);
	EXPECT_EQ(*it--, 30);
	EXPECT_EQ(*it, 20);
	EXPECT_EQ(*--it, 10);
}

TEST(TVectorIterator, Arithmetic) {
	TVector<int> v{ 1, 2, 3, 4, 5 };
	TVector<int>::iterator it = v.begin();
	EXPECT_EQ(*(it + 3), 4);
	EXPECT_EQ(*(v.end() - 1), 5);
	it += 2;
	EXPECT_EQ(*it, 3);
	it -= 1;
	EXPECT_EQ(*it, 2);
	EXPECT_TRUE(v.begin() + 5 == v.end());
}

TEST(TVectorIterator, ConstIteratorOfConstVector) {
	TVector<int> v{ 1, 2, 3 };
	const TVector<int>& c = v;
	int sum = 0;
	for (TVector<int>::const_iterator it = c.begin(); it != c.end(); ++it) sum += *it;
	EXPECT_EQ(sum, 6);
}

TEST(TVectorIterator, EmptyVector) {
	TVector<int> v;
	EXPECT_TRUE(v.begin() == v.end());
	const TVector<int> c;
	EXPECT_TRUE(c.begin() == c.end());
}

TEST(TVectorIterator, WorksWhenBufferWrapped) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i);
	for (int i = 1; i <= 12; i++) v.push_front(-i);
	int idx = 0;
	for (TVector<int>::iterator it = v.begin(); it != v.end(); ++it, ++idx) {
		EXPECT_EQ(*it, v[idx]);
	}
	EXPECT_EQ(idx, 22);
	EXPECT_EQ(*v.begin(), -12);
	EXPECT_EQ(*(v.end() - 1), 9);
}

TEST(TVectorIterator, WorksOnConstVectorWhenWrapped) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i);
	for (int i = 1; i <= 12; i++) v.push_front(-i);
	const TVector<int>& c = v;
	int idx = 0;
	for (TVector<int>::const_iterator it = c.begin(); it != c.end(); ++it, ++idx) {
		EXPECT_EQ(*it, c[idx]);
	}
	EXPECT_EQ(idx, 22);
}

TEST(TVectorShrinkToFit, EmptyVectorStaysEmpty) {
	TVector<int> v;
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), 0u);
	EXPECT_TRUE(v.is_empty());
}

TEST(TVectorShrinkToFit, ReleasesUnusedMemory) {
	TVector<int> v;
	for (int i = 0; i < 5; i++) v.push_back(i);
	EXPECT_EQ(v.capacity(), (size_t)MEM_STEP);
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), 5u);
	EXPECT_EQ(v.size(), 5u);
	for (int i = 0; i < 5; i++) EXPECT_EQ(v[i], i);
}

TEST(TVectorShrinkToFit, ClearedVectorFreesBuffer) {
	TVector<int> v{ 1, 2, 3 };
	v.clear();
	v.push_back(1);
	v.pop_back();
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), 0u);
}

TEST(TVectorShrinkToFit, NoChangeWhenAlreadyTight) {
	TVector<int> v{ 1, 2, 3 };
	v.shrink_to_fit();
	size_t cap = v.capacity();
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), cap);
	EXPECT_EQ(v.size(), 3u);
}

TEST(TVectorShrinkToFit, KeepsOrderOfWrappedVector) {
	TVector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i);
	for (int i = 1; i <= 12; i++) v.push_front(-i);
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), v.size());
	EXPECT_EQ(v.front(), -12);
	EXPECT_EQ(v.back(), 9);
	for (size_t i = 1; i < v.size(); i++) EXPECT_EQ(v[i], v[i - 1] + 1);
}

TEST(TVectorShrinkToFit, VectorStaysUsableAfterwards) {
	TVector<int> v;
	for (int i = 0; i < 20; i++) v.push_back(i);
	while (v.size() > 3) v.pop_back();
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), 3u);
	v.push_back(100);
	v.push_front(-1);
	EXPECT_EQ(v.size(), 5u);
	EXPECT_EQ(v.capacity(), (size_t)MEM_STEP);
	EXPECT_EQ(v.front(), -1);
	EXPECT_EQ(v.back(), 100);
}

TEST(TVectorShrinkToFit, WorksForNestedVectors) {
	TVector<TVector<int>> m;
	m.push_back(TVector<int>{ 1, 2, 3 });
	m.shrink_to_fit();
	EXPECT_EQ(m.capacity(), 1u);
	EXPECT_EQ(m[0][2], 3);
}

TEST(TMathVector, Constructors) {
	TMathVector<int> a(3);
	EXPECT_EQ(a.size(), 3u);
	EXPECT_EQ(a[0], 0);

	TMathVector<int> b{ 1, 2, 3 };
	EXPECT_EQ(b[2], 3);

	int arr[] = { 4, 5 };
	TMathVector<int> c(arr, 2);
	EXPECT_EQ(c[1], 5);

	TVector<int> base{ 7, 8 };
	TMathVector<int> d(base);
	EXPECT_EQ(d[0], 7);
}

TEST(TMathVector, InheritedMethodsWork) {
	TMathVector<int> v{ 1, 2, 3 };
	v.push_back(4);
	v.push_front(0);
	EXPECT_EQ(v.size(), 5u);
	EXPECT_EQ(v.front(), 0);
	EXPECT_EQ(v.back(), 4);
	v.erase(0);
	v.sort();
	EXPECT_EQ(v[0], 1);
	v.shrink_to_fit();
	EXPECT_EQ(v.capacity(), 4u);
}

TEST(TMathVector, AddSubtract) {
	TMathVector<int> a{ 1, 2, 3 };
	TMathVector<int> b{ 10, 20, 30 };
	TMathVector<int> s = a + b;
	TMathVector<int> d = b - a;
	EXPECT_TRUE(s == (TMathVector<int>{ 11, 22, 33 }));
	EXPECT_TRUE(d == (TMathVector<int>{ 9, 18, 27 }));
	EXPECT_EQ(a[0], 1);
}

TEST(TMathVector, CompoundAssignment) {
	TMathVector<int> a{ 1, 2, 3 };
	a += TMathVector<int>{ 1, 1, 1 };
	EXPECT_TRUE(a == (TMathVector<int>{ 2, 3, 4 }));
	a -= TMathVector<int>{ 2, 2, 2 };
	EXPECT_TRUE(a == (TMathVector<int>{ 0, 1, 2 }));
	a *= 3;
	EXPECT_TRUE(a == (TMathVector<int>{ 0, 3, 6 }));
	a /= 3;
	EXPECT_TRUE(a == (TMathVector<int>{ 0, 1, 2 }));
}

TEST(TMathVector, SizeMismatchThrows) {
	TMathVector<int> a{ 1, 2, 3 };
	TMathVector<int> b{ 1, 2 };
	EXPECT_THROW(a + b, std::invalid_argument);
	EXPECT_THROW(a - b, std::invalid_argument);
	EXPECT_THROW(a * b, std::invalid_argument);
	EXPECT_THROW(a += b, std::invalid_argument);
}

TEST(TMathVector, ScalarMultiplyAndDivide) {
	TMathVector<double> a{ 1.0, 2.0, 4.0 };
	TMathVector<double> m = a * 2.0;
	TMathVector<double> m2 = 2.0 * a;
	TMathVector<double> q = a / 2.0;
	EXPECT_TRUE(m == (TMathVector<double>{ 2.0, 4.0, 8.0 }));
	EXPECT_TRUE(m2 == m);
	EXPECT_TRUE(q == (TMathVector<double>{ 0.5, 1.0, 2.0 }));
}

TEST(TMathVector, DivisionByZeroThrows) {
	TMathVector<double> a{ 1.0, 2.0 };
	EXPECT_THROW(a / 0.0, std::invalid_argument);
	EXPECT_THROW(a /= 0.0, std::invalid_argument);
}

TEST(TMathVector, UnaryMinus) {
	TMathVector<int> a{ 1, -2, 3 };
	EXPECT_TRUE(-a == (TMathVector<int>{ -1, 2, -3 }));
}

TEST(TMathVector, DotProductAndLength) {
	TMathVector<int> a{ 1, 2, 3 };
	TMathVector<int> b{ 4, -5, 6 };
	EXPECT_EQ(a * b, 12);
	TMathVector<double> c{ 3.0, 4.0 };
	EXPECT_NEAR(c.length(), 5.0, 1e-12);
	TMathVector<int> e;
	EXPECT_EQ(e.length(), 0.0);
}

TEST(TMathVector, Comparison) {
	TMathVector<int> a{ 1, 2, 3 };
	TMathVector<int> b{ 1, 2, 3 };
	TMathVector<int> c{ 1, 2, 4 };
	TMathVector<int> d{ 1, 2 };
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a != c);
	EXPECT_TRUE(a != d);
}

TEST(TMathVector, WorksOnWrappedBuffer) {
	TMathVector<int> a;
	for (int i = 0; i < 10; i++) a.push_back(i);
	for (int i = 1; i <= 12; i++) a.push_front(-i);
	TMathVector<int> b(a.size());
	for (size_t i = 0; i < b.size(); i++) b[i] = 1;
	TMathVector<int> s = a + b;
	for (size_t i = 0; i < s.size(); i++) EXPECT_EQ(s[i], a[i] + 1);
}

TEST(TMathVector, Output) {
	std::ostringstream os;
	os << TMathVector<int>{ 1, 2, 3 };
	EXPECT_EQ(os.str(), "[ 1 2 3 ]");
	std::ostringstream e;
	e << TMathVector<int>();
	EXPECT_EQ(e.str(), "[ ]");
}

TEST(TMathVector, VectorOfVectorsAdds) {
	TMathVector<TMathVector<int>> a{ TMathVector<int>{ 1, 2 }, TMathVector<int>{ 3, 4 } };
	TMathVector<TMathVector<int>> b{ TMathVector<int>{ 10, 20 }, TMathVector<int>{ 30, 40 } };
	TMathVector<TMathVector<int>> s = a + b;
	EXPECT_EQ(s[1][0], 33);
	EXPECT_EQ(s[0][1], 22);
}

TEST(TMatrixInherited, SizeAndIndexing) {
	TMatrix<int> m({ { 1, 2, 3 }, { 4, 5, 6 } });
	EXPECT_EQ(m.size(), 2u);
	EXPECT_EQ(m[0].size(), 3u);
	EXPECT_EQ(m[1][2], 6);
	m[0][1] = 20;
	EXPECT_EQ(m[0][1], 20);
	EXPECT_THROW(m[2], std::out_of_range);
	EXPECT_THROW(m[0][3], std::out_of_range);
}

TEST(TMatrixInherited, AddAndSubtractGiveMatrix) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> b({ { 10, 20 }, { 30, 40 } });
	TMatrix<int> s = a + b;
	TMatrix<int> d = b - a;
	EXPECT_TRUE(s == (TMatrix<int>({ { 11, 22 }, { 33, 44 } })));
	EXPECT_TRUE(d == (TMatrix<int>({ { 9, 18 }, { 27, 36 } })));
}

TEST(TMatrixInherited, AddWithWrongShapeThrows) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> b({ { 1, 2, 3 }, { 4, 5, 6 } });
	TMatrix<int> c({ { 1, 2 } });
	EXPECT_THROW(a + b, std::invalid_argument);
	EXPECT_THROW(a - c, std::invalid_argument);
}

TEST(TMatrixInherited, UnaryMinusAndCompoundOps) {
	TMatrix<int> a({ { 1, -2 }, { 3, 4 } });
	TMatrix<int> n = -a;
	EXPECT_EQ(n[0][1], 2);
	EXPECT_EQ(n[1][0], -3);
	a += TMatrix<int>({ { 1, 1 }, { 1, 1 } });
	EXPECT_EQ(a[0][0], 2);
	a -= TMatrix<int>({ { 2, 2 }, { 2, 2 } });
	EXPECT_EQ(a[0][0], 0);
}

TEST(TMatrixInherited, EqualityAndInequality) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> b({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> c({ { 1, 2 }, { 3, 5 } });
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a != c);
}

TEST(TMatrixInherited, RowOperationsOfTVector) {
	TMatrix<int> m({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
	m.pop_back();
	EXPECT_EQ(m.size(), 2u);
	m.erase(0);
	EXPECT_EQ(m[0][0], 3);
	m.push_back(TMathVector<int>{ 7, 8 });
	EXPECT_EQ(m.back()[1], 8);
	m.shrink_to_fit();
	EXPECT_EQ(m.capacity(), 2u);
}

TEST(TMatrixInherited, ConstructorsAndShape) {
	TMatrix<int> z(2, 3);
	EXPECT_EQ(z.size(), 2u);
	EXPECT_EQ(z[1].size(), 3u);
	EXPECT_EQ(z[1][2], 0);

	TMatrix<int> e;
	EXPECT_EQ(e.size(), 0u);

	EXPECT_THROW((TMatrix<int>({ { 1, 2 }, { 3 } })), std::invalid_argument);
	TMathVector<TMathVector<int>> bad{ TMathVector<int>{ 1, 2 }, TMathVector<int>{ 3 } };
	EXPECT_THROW(TMatrix<int> m(bad), std::invalid_argument);
}

TEST(TMatrix, RowsAndCols) {
	TMatrix<int> m(3, 4);
	EXPECT_EQ(m.rows(), 3u);
	EXPECT_EQ(m.cols(), 4u);
	TMatrix<int> e;
	EXPECT_EQ(e.rows(), 0u);
	EXPECT_EQ(e.cols(), 0u);
}

TEST(TMatrix, ScalarMultiplyAndDivide) {
	TMatrix<double> a({ { 1.0, 2.0 }, { 3.0, 4.0 } });
	TMatrix<double> m = a * 2.0;
	TMatrix<double> m2 = 2.0 * a;
	TMatrix<double> q = a / 2.0;
	EXPECT_EQ(m[1][1], 8.0);
	EXPECT_TRUE(m == m2);
	EXPECT_EQ(q[0][0], 0.5);
	EXPECT_EQ(a[0][0], 1.0);
}

TEST(TMatrix, MatrixProduct) {
	TMatrix<int> a({ { 1, 2, 3 }, { 4, 5, 6 } });
	TMatrix<int> b({ { 7, 8 }, { 9, 10 }, { 11, 12 } });
	TMatrix<int> c = a * b;
	EXPECT_EQ(c.rows(), 2u);
	EXPECT_EQ(c.cols(), 2u);
	EXPECT_TRUE(c == (TMatrix<int>({ { 58, 64 }, { 139, 154 } })));
}

TEST(TMatrix, ProductWithWrongShapeThrows) {
	TMatrix<int> a(2, 3);
	TMatrix<int> b(2, 3);
	EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TMatrix, IdentityIsNeutral) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> i = TMatrix<int>::identity(2);
	EXPECT_TRUE(a * i == a);
	EXPECT_TRUE(i * a == a);
	EXPECT_EQ(TMatrix<int>::identity(3)[2][2], 1);
	EXPECT_EQ(TMatrix<int>::identity(3)[0][2], 0);
}

TEST(TMatrix, MatrixTimesVector) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
	TMathVector<int> v{ 1, 1 };
	TMathVector<int> r = a * v;
	EXPECT_TRUE(r == (TMathVector<int>{ 3, 7, 11 }));
	EXPECT_THROW(a * (TMathVector<int>{ 1, 2, 3 }), std::invalid_argument);
}

TEST(TMatrix, Transpose) {
	TMatrix<int> a({ { 1, 2, 3 }, { 4, 5, 6 } });
	TMatrix<int> t = a.transposed();
	EXPECT_EQ(t.rows(), 3u);
	EXPECT_EQ(t.cols(), 2u);
	EXPECT_EQ(t[2][0], 3);
	EXPECT_EQ(t[0][1], 4);
	EXPECT_TRUE(t.transposed() == a);
}

TEST(TMatrix, Determinant) {
	EXPECT_NEAR((TMatrix<int>({ { 1, 2 }, { 3, 4 } })).determinant(), -2.0, 1e-9);
	EXPECT_NEAR((TMatrix<int>({ { 2, 0, 0 }, { 0, 3, 0 }, { 0, 0, 4 } })).determinant(), 24.0, 1e-9);
	EXPECT_NEAR((TMatrix<double>({ { 0.0, 1.0 }, { 1.0, 0.0 } })).determinant(), -1.0, 1e-9);
	EXPECT_NEAR((TMatrix<int>({ { 1, 2 }, { 2, 4 } })).determinant(), 0.0, 1e-9);
	EXPECT_NEAR((TMatrix<int>({ { 2, -3, 1 }, { 2, 0, -1 }, { 1, 4, 5 } })).determinant(), 49.0, 1e-9);
	EXPECT_THROW((TMatrix<int>(2, 3)).determinant(), std::invalid_argument);
}

TEST(TMatrix, Inverse) {
	TMatrix<double> a({ { 4.0, 7.0 }, { 2.0, 6.0 } });
	TMatrix<double> inv = a.inverse();
	TMatrix<double> p = a * inv;
	for (size_t i = 0; i < 2; i++) {
		for (size_t j = 0; j < 2; j++) {
			EXPECT_NEAR(p[i][j], i == j ? 1.0 : 0.0, 1e-9);
		}
	}
	EXPECT_NEAR(inv[0][0], 0.6, 1e-9);
	EXPECT_THROW((TMatrix<int>({ { 1, 2 }, { 2, 4 } })).inverse(), std::runtime_error);
	EXPECT_THROW((TMatrix<int>(2, 3)).inverse(), std::invalid_argument);
}

TEST(TMatrix, InverseNeedsRowSwap) {
	TMatrix<int> a({ { 0, 1 }, { 1, 0 } });
	TMatrix<double> inv = a.inverse();
	EXPECT_NEAR(inv[0][1], 1.0, 1e-9);
	EXPECT_NEAR(inv[0][0], 0.0, 1e-9);
}

TEST(TMatrix, Output) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	std::ostringstream os;
	os << a;
	EXPECT_EQ(os.str(), "[ 1 2 ]\n[ 3 4 ]");
}

TEST(TMatrix, AdditionResultWorksWithMatrixMethods) {
	TMatrix<int> a({ { 1, 2 }, { 3, 4 } });
	TMatrix<int> b = TMatrix<int>::identity(2);
	TMatrix<int> c = (a + b) * a;
	EXPECT_TRUE(c == (TMatrix<int>({ { 8, 12 }, { 18, 26 } })));
}