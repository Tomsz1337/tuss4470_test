import serial
import matplotlib
matplotlib.use('TkAgg')  # Szybszy interaktywny backend
import matplotlib.pyplot as plt
import numpy as np
import time

# --- KONFIGURACJA ---
serial_port = 'COM9'
baud_rate = 115200
num_samples = 1000
max_cols = 150

speed_of_sound = 330  # m/s
sample_time = 20e-6   # 20 µs
sample_resolution = (speed_of_sound * sample_time * 100) / 2  # cm

# --- INICJALIZACJA POŁĄCZENIA ---
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# --- PRZYGOTOWANIE MACIERZY ---
data = np.zeros((num_samples, max_cols))

# --- USTAWIENIA WYKRESU ---
plt.ion()
fig, ax = plt.subplots()
waterfall = ax.imshow(data, aspect='auto', cmap='viridis', interpolation='nearest', vmin=0, vmax=2000)

# Osie Y
ax.invert_yaxis()
tick_step = 50
ax.set_yticks(np.arange(0, num_samples, tick_step)[::-1])
ax.set_yticklabels([f'{dist:.1f}' for dist in np.arange(0, num_samples, tick_step)[::-1] * sample_resolution])
ax.set_ylabel('Distance (cm)')
ax.grid(axis='y', linestyle='--', color='gray', linewidth=0.5)

# Osie X
ax.set_xticks(np.linspace(0, max_cols - 1, num=10))
ax.set_xticklabels(['' for _ in range(10)])
ax.set_xlabel('Elapsed Time (s)')
title_obj = ax.set_title('Waiting for data...')

# Rysowanie tła tylko raz
fig.canvas.draw()
bg_cache = fig.canvas.copy_from_bbox(ax.bbox)

# --- PARSOWANIE DANYCH ---
buffering = False
line_buffer = ""

def parse_data(line):
    global buffering, line_buffer
    try:
        line = line.decode('utf-8').strip()
        if line == "sp":
            buffering = True
            line_buffer = ""
            return None
        elif buffering:
            line_buffer += line
            parts = line_buffer.split(',')
            if len(parts) == num_samples:
                buffering = False
                return [int(x.strip()) for x in parts]
    except Exception as e:
        print(f"[ERROR] Parsing failed: {e}")
    return None

# --- GŁÓWNA PĘTLA ---
start_time = time.time()
frame_count = 0

try:
    while True:
        line = ser.readline()
        new_data = parse_data(line)

        if new_data and len(new_data) == num_samples:
            elapsed = time.time() - start_time

            # Aktualizacja danych
            data = np.roll(data, -1, axis=1)
            data[:, -1] = new_data

            # Aktualizacja wykresu
            waterfall.set_array(data)
            title_obj.set_text(f'Waterfall Chart | Elapsed: {elapsed:.1f}s | Sample Res: {sample_resolution:.2f} cm')

            # Szybkie odświeżenie tylko obrazu
            fig.canvas.restore_region(bg_cache)
            ax.draw_artist(waterfall)
            fig.canvas.blit(ax.bbox)
            fig.canvas.flush_events()

            frame_count += 1

except KeyboardInterrupt:
    print("Zamykanie...")
finally:
    ser.close()
