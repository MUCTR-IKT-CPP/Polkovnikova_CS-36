import numpy as np
import random
import time
import matplotlib.pyplot as plt
from collections import defaultdict

# Функция для создания графа в виде матрицы смежности
def create_weighted_graph(num_vertices, min_edges):
    # Инициализируем матрицу смежности нулями
    graph = np.zeros((num_vertices, num_vertices), dtype=int)
    
    # Обеспечиваем минимальную связность: создаем "скелет" графа (например, цепочку)
    for i in range(num_vertices - 1):
        weight = random.randint(1, 20)  # Случайный вес ребра от 1 до 20
        graph[i][i + 1] = weight
        graph[i + 1][i] = weight  # Граф неориентированный, поэтому симметрично
    
    # Добавляем дополнительные случайные ребра
    for i in range(num_vertices):
        # Определяем, сколько дополнительных ребер добавить (минимум min_edges, но не больше, чем вершин)
        current_edges = sum(1 for j in range(num_vertices) if graph[i][j] != 0)
        edges_to_add = max(0, min_edges - current_edges) + random.randint(0, num_vertices // 4)
        
        for _ in range(edges_to_add):
            j = random.randint(0, num_vertices - 1)
            if i != j and graph[i][j] == 0:  # Если ребра еще нет
                weight = random.randint(1, 20)
                graph[i][j] = weight
                graph[j][i] = weight  # Симметрия для неориентированного графа
    
    return graph

# Алгоритм Дейкстры для поиска кратчайших путей от одной вершины ко всем остальным
def dijkstra(graph, start_vertex):
    num_vertices = len(graph)
    distances = [float('inf')] * num_vertices
    distances[start_vertex] = 0
    visited = [False] * num_vertices
    for _ in range(num_vertices):
        # Находим вершину с минимальной дистанцией среди непосещенных
        min_dist = float('inf')
        min_vertex = -1
        for v in range(num_vertices):
            if not visited[v] and distances[v] < min_dist:
                min_dist = distances[v]
                min_vertex = v
        
        if min_vertex == -1:
            break
        
        visited[min_vertex] = True
        # Обновляем расстояния до соседей
        for v in range(num_vertices):
            if (graph[min_vertex][v] > 0 and not visited[v] and
                distances[min_vertex] + graph[min_vertex][v] < distances[v]):
                distances[v] = distances[min_vertex] + graph[min_vertex][v]
    
    return distances

# Функция для поиска кратчайших путей между всеми парами вершин
def all_pairs_shortest_paths(graph):
    num_vertices = len(graph)
    all_distances = []
    for start in range(num_vertices):
        distances = dijkstra(graph, start)
        all_distances.append(distances)
    return all_distances

# Основная часть: создание графов, замер времени, построение графика
vertex_counts = [10, 20, 50, 100]  # Количество вершин
min_edges_list = [3, 4, 10, 20]    # Минимальное количество ребер для каждой вершины
num_tests = 5                      # Количество тестов для каждого размера графа
average_times = []                 # Среднее время выполнения для каждого размера графа

for idx, num_vertices in enumerate(vertex_counts):
    min_edges = min_edges_list[idx]
    print(f"\nGraph with {num_vertices} vertices (minimum {min_edges} of edges per vertex):")
    
    # Создаем граф
    graph = create_weighted_graph(num_vertices, min_edges)
    
    # Выводим матрицу смежности
    print("Adjacency Matrix: ")
    print(graph)
    
    # Проводим тесты и замеряем время
    times = []
    for test in range(num_tests):
        start_time = time.time()
        all_pairs_shortest_paths(graph)
        end_time = time.time()
        elapsed_time = end_time - start_time
        times.append(elapsed_time)
        print(f"Test {test + 1} for {num_vertices} vertices: {elapsed_time:.4f} seconds")
    
    # Считаем среднее время
    avg_time = sum(times) / num_tests
    average_times.append(avg_time)
    print(f"Average time for {num_vertices} vertices: {avg_time:.4f} seconds")

# Построение графика
plt.figure(figsize=(8, 6))
plt.plot(vertex_counts, average_times, marker='o', linestyle='-', color='b')
plt.title("Зависимость времени выполнения от количества вершин")
plt.xlabel("Количество вершин (N)")
plt.ylabel("Время выполнения (секунды)")
plt.grid(True)
plt.show()