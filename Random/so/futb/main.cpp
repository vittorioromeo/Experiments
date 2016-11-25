#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <mingw.thread.h>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>

#include <ecst/utils.hpp>
#include <ecst/thread_pool.hpp>


template<typename Pool, typename Iterator, typename Func>
void parallel_for_each(Pool& p, Iterator first, Iterator last, Func func)
{
    const auto length = std::distance(first, last);
    if (0 == length) return;

    const auto min_per_thread = 25u;
    const unsigned max_threads = (length + min_per_thread - 1) / min_per_thread;

    const auto hardware_threads = std::thread::hardware_concurrency();

    const auto num_threads = 8;

    const auto block_size = length / num_threads;

    ecst::latch l(num_threads - 1);
    l.execute_and_wait_until_zero([&]
    {
        auto block_start = first;
        for (unsigned i = 0; i < num_threads - 1; ++i)
        {
            auto block_end = block_start;
            std::advance(block_end, block_size);

            p.post([&, block_start, block_end, tempacc = 0ull]() mutable
            {
                std::for_each(block_start, block_end, [&tempacc](auto x){ tempacc += x; });
                func(tempacc);
                l.decrement_and_notify_all();
            });

            block_start = block_end;
        }

        auto tempacc2 = 0ull;
        std::for_each(block_start, last, [&tempacc2](auto x){ tempacc2 += x; });
        func(tempacc2);
    });
}

using namespace std;

constexpr size_t ARRAY_SIZE = 500'000'000;
typedef std::vector<uint64_t> Array;

template <class FE, class F>
void test_for_each(const Array& a, FE fe, F f, atomic<uint64_t>& result)
{
  auto time_begin = chrono::high_resolution_clock::now();
  result = 0;
  fe(a.begin(), a.end(), f);
  auto time_end = chrono::high_resolution_clock::now();

  cout << "Result = " << result << endl;
  cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(
            time_end - time_begin).count() << endl;
}

int main()
{
  ecst::thread_pool p;

  random_device device;
  default_random_engine engine(device());
  uniform_int_distribution<uint8_t> distribution(0, 255);

  Array a;
  a.reserve(ARRAY_SIZE);

  cout << "Generating array ... " << endl;
  for (size_t i = 0; i < ARRAY_SIZE; ++i)
    a.push_back(distribution(engine));

  atomic<uint64_t> result;
  auto acc = [&result](uint64_t value) { result += value; };

  cout << "parallel_for_each ..." << endl;
  test_for_each(a, [&p](auto a, auto b, auto c){ return parallel_for_each<decltype(p), Array::const_iterator, decltype(acc)>(p,a,b,c); }, acc, result);
  cout << "for_each ..." << endl;
  test_for_each(a, for_each<Array::const_iterator, decltype(acc)>, acc, result);

  return 0;
}
