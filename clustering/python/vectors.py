import numpy as np
import matplotlib.pyplot as plt

# --- Carga según tu base ---
data = np.load('../data_eda.npy')      # (N, 2)
queries = np.load('../queries_eda.npy')# (Q, 2)

# Elige la consulta a visualizar
q_idx = 16           # <-- cámbialo si quieres otra
m_values = [16, 32, 64, 128]

# Distancias de la consulta (queries[q_idx]) a todos los datos
q = queries[q_idx]                                # (2,)
dists = np.sum((data - q)**2, axis=1)             # L2^2
order = np.argsort(dists)                         # índices ordenados por cercanía

for m in m_values:
    nn_idx = order[:m]

    plt.figure()
    # Datos (fondo)
    plt.scatter(data[:, 0], data[:, 1], s=10, alpha=0.25, label='data')
    # Todas las queries (opcional, como referencia)
    plt.scatter(queries[:, 0], queries[:, 1], s=10, alpha=0.25, marker='x', label='queries')
    # m vecinos más cercanos
    plt.scatter(data[nn_idx, 0], data[nn_idx, 1], s=30, marker='s', label=f'{m} NN')
    # La consulta seleccionada
    plt.scatter([q[0]], [q[1]], s=90, marker='*', label=f'query #{q_idx}')

    plt.title(f'm={m} vecinos de la consulta #{q_idx}')
    plt.legend()
    plt.axis('equal')
    plt.tight_layout()
    plt.show()
