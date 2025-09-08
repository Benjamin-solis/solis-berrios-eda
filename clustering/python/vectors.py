import numpy as np
import matplotlib.pyplot as plt

data = np.load('../data_eda.npy')
queries = np.load('../queries_eda.npy')

q_idx = 16
m_values = [16, 32, 64, 128]

q = queries[q_idx]
dists = np.sum((data - q)**2, axis=1)
order = np.argsort(dists)

for m in m_values:
    nn_idx = order[:m]

    plt.figure()
    plt.scatter(data[:, 0], data[:, 1], s=10, alpha=0.25, label='data')
    plt.scatter(queries[:, 0], queries[:, 1], s=10, alpha=0.25, marker='x', label='queries')
    plt.scatter(data[nn_idx, 0], data[nn_idx, 1], s=30, marker='s', label=f'{m} NN')
    plt.scatter([q[0]], [q[1]], s=90, marker='*', label=f'query #{q_idx}')

    plt.title(f'm={m} vecinos de la consulta #{q_idx}')
    plt.legend()
    plt.axis('equal')
    plt.tight_layout()
    plt.show()
