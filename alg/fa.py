import time
import matplotlib.pyplot as plt
from collections import deque

def extend_queue(q, n):
    result = deque(q)
    for i in range(n):
        result.append(i)
    return result

ns = [10, 100, 1000, 10000, 50000, 100000, 500000, 1000000, 2000000, 5000000]

extend_queue_l = []

for n in ns:
    my_queue = deque(range(10000))  # базовая очередь

    start = time.perf_counter()
    extend_queue(my_queue, n)
    extend_queue_l.append(time.perf_counter() - start)

plt.figure(figsize=(10, 6))
plt.plot(ns, extend_queue_l, marker='o', label='Расширение очереди')

plt.xlabel('Количество добавляемых элементов (N)', fontsize=12)
plt.ylabel('Время работы (секунды)', fontsize=12)
plt.title('Время расширения очереди', fontsize=14)

plt.legend()
plt.grid(True)

plt.xscale('log')
plt.yscale('log')

plt.tight_layout()
plt.show()
