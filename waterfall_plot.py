import csv
import numpy as np
import matplotlib.pyplot as plt
import time

# Parametry
filename = 'pomiar_adc.csv'   
num_samples = 1000             
max_cols = 150                

# Prędkość dźwięku i czas próbkowania
#speed_of_sound = 1470             # m/s (woda)
speed_of_sound = 340             # m/s (powietrze)
sample_time = 20e-6               # Czas próbkowania (10us)
sample_resolution = (speed_of_sound * sample_time * 100) / 2  # cm


def load_flat_data(filename):
    flat_data = []
    with open(filename, mode='r') as file:
        reader = csv.reader(file)
        for row in reader:
            for value in row:
                if value.strip() != '':
                    flat_data.append(int(value))
    return flat_data

# Wczytanie danych z pliku CSV
data_flat = load_flat_data(filename)

# Dzielenie na ramki
frames = []
for i in range(0, len(data_flat), num_samples):
    frame = data_flat[i:i+num_samples]
    if len(frame) == num_samples:
        frames.append(frame)

# Przygotowanie macierzy danych
data = np.zeros((num_samples, max_cols))

# Konfiguracja wykresu
plt.ion()
fig, ax = plt.subplots()
waterfall = ax.imshow(data, aspect='auto', cmap='viridis', interpolation='nearest', vmin=0, vmax=2000)
plt.colorbar(waterfall, ax=ax)

# Skala osi Y
ax.invert_yaxis()
ax.set_ylabel('Distance (cm)')
tick_step = 50
ax.set_yticks(np.arange(0, num_samples, tick_step)[::-1])
ax.set_yticklabels([f'{dist:.2f}' for dist in np.arange(0, num_samples, tick_step)[::-1] * sample_resolution])
ax.grid(axis='y', linestyle='--', color='gray', linewidth=0.5)

# Oś X
ax.set_xticks(np.linspace(0, max_cols - 1, num=10))
ax.set_xticklabels(['' for _ in range(10)])
ax.set_xlabel('Elapsed Time (samples)')

# Wyświetlanie ramek jedna po drugiej
for frame in frames:
    data = np.roll(data, -1, axis=1)   
    data[:, -1] = frame                

    waterfall.set_data(data)
    ax.set_title(f'Waterfall Chart from CSV\nSample Resolution: {sample_resolution:.2f} cm')
    
    fig.canvas.flush_events()
    plt.draw()

plt.ioff()
plt.show()
