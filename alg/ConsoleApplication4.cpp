#include <iostream>
#include <queue>
#include <chrono>
#include <utility>

std::queue<int> ExtendQueue(std::queue<int> q, int n)
{
    if (n <= 0)
        return q;

    for (int i = 0; i < n; i++)
    {
        q.push(i);
    }

    return q;
}

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    auto result = func(std::forward<Args>(args)...);

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    return std::make_pair(result, duration.count());
}

int main()
{
    setlocale(LC_ALL, "Rus");

    std::queue<int> myQueue;

    for (int i = 0; i < 10000; i++)
    {
        myQueue.push(i);
    }

    auto result_pair = measure_execution_time(ExtendQueue, myQueue, 5000000);
    std::queue<int> result = result_pair.first;
    auto execution_time = result_pair.second;

    std::cout << "Размер очереди: " << result.size() << std::endl;
    std::cout << "Время выполнения: " << execution_time << " мкс" << std::endl;

    return 0;
}