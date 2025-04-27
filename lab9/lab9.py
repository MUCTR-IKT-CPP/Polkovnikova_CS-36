import hashlib
import random
import string
import time
import matplotlib.pyplot as plt
from collections import defaultdict

def sha1_hash(data):
    """Function to compute SHA-1 hash"""
    return hashlib.sha1(data.encode('utf-8')).hexdigest()

def generate_random_string(length):
    """Generate random string of given length"""
    return ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(length))

def modify_string(s, num_changes):
    """Modify string with specified number of changes"""
    s_list = list(s)
    indices = random.sample(range(len(s)), num_changes)
    for i in indices:
        s_list[i] = random.choice(string.ascii_letters + string.digits)
    return ''.join(s_list)

def longest_common_substring(s1, s2):
    """Find length of longest common substring"""
    m = [[0] * (1 + len(s2)) for _ in range(1 + len(s1))]
    longest = 0
    for x in range(1, 1 + len(s1)):
        for y in range(1, 1 + len(s2)):
            if s1[x - 1] == s2[y - 1]:
                m[x][y] = m[x - 1][y - 1] + 1
                if m[x][y] > longest:
                    longest = m[x][y]
            else:
                m[x][y] = 0
    return longest

def test_hash_collisions_changes():
    """Test 1: compare hashes of strings with different numbers of differences"""
    results = {1: [], 2: [], 4: [], 8: [], 16: []}
    base_string = generate_random_string(128)
    
    for num_changes in results.keys():
        max_lengths = []
        for _ in range(1000):
            modified_string = modify_string(base_string, num_changes)
            hash1 = sha1_hash(base_string)
            hash2 = sha1_hash(modified_string)
            lcs = longest_common_substring(hash1, hash2)
            max_lengths.append(lcs)
        avg_max_length = sum(max_lengths) / len(max_lengths)
        results[num_changes] = avg_max_length
    
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(list(results.keys()), list(results.values()), marker='o')
    plt.xlabel('Number of differences in input strings')
    plt.ylabel('Average length of longest common substring in hashes')
    plt.title('Hash similarity dependence on input string differences')
    plt.grid(True)
    plt.show()
    
    return results

def test_hash_collisions_generation():
    """Test 2: collision detection in mass hash generation"""
    results = {}
    for i in range(2, 7):
        N = 10 ** i
        hashes = set()
        collisions = 0
        for _ in range(N):
            s = generate_random_string(256)
            h = sha1_hash(s)
            if h in hashes:
                collisions += 1
            else:
                hashes.add(h)
        results[N] = collisions
    
    # Print results in table format
    print("\nCollision detection results:")
    print("+-----------+----------------+")
    print("| Number of | Collisions     |")
    print("| hashes    | detected       |")
    print("+-----------+----------------+")
    for n, coll in results.items():
        print(f"| {n:9} | {coll:14} |")
    print("+-----------+----------------+")
    
    return results

def test_hash_speed():
    """Test 3: measuring hash computation speed for different input sizes"""
    sizes = [64, 128, 256, 512, 1024, 2048, 4096, 8192]
    results = {}
    
    for size in sizes:
        times = []
        for _ in range(1000):
            s = generate_random_string(size)
            start_time = time.time()
            sha1_hash(s)
            end_time = time.time()
            times.append(end_time - start_time)
        avg_time = sum(times) / len(times)
        results[size] = avg_time
    
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(list(results.keys()), list(results.values()), marker='o')
    plt.xlabel('Input size (characters)')
    plt.ylabel('Average hash computation time (seconds)')
    plt.title('Hash computation time vs input size')
    plt.grid(True)
    plt.xscale('log')
    plt.yscale('log')
    plt.show()
    
    # Print results in table format
    print("\nSpeed measurement results:")
    print("+-----------+---------------------+")
    print("| Input     | Average time        |")
    print("| size      | per hash (sec)      |")
    print("+-----------+---------------------+")
    for size, t in results.items():
        print(f"| {size:7}   | {t:.10f}        |")
    print("+-----------+---------------------+")
    
    return results

# Run all tests
if __name__ == "__main__":
    print("Running test 1...")
    test1_results = test_hash_collisions_changes()
    print("Test 1 results:", test1_results)
    
    print("\nRunning test 2...")
    test2_results = test_hash_collisions_generation()
    
    print("\nRunning test 3...")
    test3_results = test_hash_speed()
