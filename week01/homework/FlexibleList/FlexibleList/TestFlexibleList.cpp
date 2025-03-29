#include "FlexibleList.h"
#include <benchmark/benchmark.h>
#include <list>

static void benchFlexibleListInsert(benchmark::State& state)
{
	for (auto _ : state)
	{
		FlexibleList f1;
		for (int i = 0; i < state.range(0); ++i)   // 每次测试会选定一个规模返回，range(0)代表这个范围的第一个数
		{
			f1.pushBack(i);
		}
	}
}

BENCHMARK(benchFlexibleListInsert)->Range(1, 1 << 20);  //range(1, 1 << 20)好像代表了一个区间，意为1~2的20次方这个区间，在这个返回内选一些规模点进行测试

static void benchListInsert(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::list<int> l;
		for (int i = 0; i < state.range(0); ++i)
		{
			l.push_back(i);
		}
	}
}

//BENCHMARK(benchListInsert)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(benchListInsert)->Range(1, 1 << 20);

static void benchmarkFlexibleListTraverse(benchmark::State& state) {
	FlexibleList list;
	for (int i = 0; i < state.range(0); ++i) {
		list.pushBack(i);
	}

	for (auto _ : state) {
		int sum = 0;
		FlexibleList::Iterator endIt = list.end();
		for (FlexibleList::Iterator it = list.begin(); it != endIt; ++it) {
			sum += it.getValue<int>();
		}
		benchmark::DoNotOptimize(sum);
	}
}
BENCHMARK(benchmarkFlexibleListTraverse)->Range(1, 1 << 20);

static void benchmarkStdListTraverse(benchmark::State& state) {
	std::list<int> list;
	for (int i = 0; i < state.range(0); ++i) {
		list.push_back(i);
	}

	for (auto _ : state) {
		int sum = 0;
		for (const auto& val : list) {
			sum += val;
		}
		benchmark::DoNotOptimize(sum);
	}
}
BENCHMARK(benchmarkStdListTraverse)->Range(1, 1 << 20);

static void benchmarkFlexibleListErase(benchmark::State& state) {
	FlexibleList l;
	for (int i = 0; i < state.range(0); ++i)
	{
		l.pushBack(i);
	}
	for (auto _ : state) {
		FlexibleList::Iterator it = l.begin();
		while (!l.empty() && it.isValid()) 
		{
			it = l.erase(it); // 假设有合适的 erase 方法
		}
	}
}
BENCHMARK(benchmarkFlexibleListErase)->Range(1, 1 << 20);
//
static void benchmarkStdListErase(benchmark::State& state) {
	std::list<int> list;
	for (int i = 0; i < state.range(0); ++i)
	{
		list.push_back(i);
	}
	for (auto _ : state) {
		std::list<int>::iterator it = list.begin();
		while (!list.empty())
		{
			it = list.erase(it);
		}
	}
}
BENCHMARK(benchmarkStdListErase)->Range(1, 1 << 20);

static void benchmarkFlexibleListPopback(benchmark::State& state)
{
	FlexibleList l;
	for (int i = 0; i < state.range(0); ++i)
	{
		l.pushBack(i);
	}

	for (auto _ : state)
	{
		while (!l.empty())
		{
			l.popBack();
		}
	}
}
BENCHMARK(benchmarkFlexibleListPopback)->Range(1, 1 << 20);

static void benchmarkStdlistPopback(benchmark::State& state)
{
	std::list<int> l;
	for (int i = 0; i < state.range(0); i++)
	{
		l.push_back(i);
	}

	for (auto _ : state)
	{
		while (!l.empty())
		{
			l.pop_back();
		}
	}
}
BENCHMARK(benchmarkStdlistPopback)->Range(1, 1 << 20);


//BENCHMARK_MAIN();
