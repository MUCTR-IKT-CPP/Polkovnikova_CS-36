import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# Чтение данных
data = pd.read_csv('heap_results.csv')

# Подготовка данных
heap_types = ['Binary', 'Fibonacci']
operations = ['fill', 'find', 'delete', 'insert']

# Создание графиков
plt.figure(figsize=(15, 10))

for i, op in enumerate(operations, 1):
    plt.subplot(2, 2, i)
    for heap in heap_types:
        op_data = data[(data['HeapType'] == heap) & (data['Operation'] == op)]
        plt.plot(op_data['N'], op_data['AvgTime'], label=f'{heap} Avg')
        plt.plot(op_data['N'], op_data['MaxTime'], '--', label=f'{heap} Max')
    plt.xscale('log')
    plt.yscale('log')
    plt.title(f'{op.capitalize()} Operation')
    plt.xlabel('N')
    plt.ylabel('Time (ms)')
    plt.legend()
    plt.grid(True)

plt.tight_layout()
plt.savefig('heap_performance.png')
