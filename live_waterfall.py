import serial
import matplotlib.pyplot as plt
import numpy as np
import time

print("STARTING CORRECT SCRIPT ✅")

# Serial port configuration
serial_port = 'COM4'  # <-- Upewnij się, że to właściwy port!
baud_rate = 500000

# Initialize serial connection
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Parameters for the waterfall chart
num_samples = 850  # MUSI się zgadzać z Arduino!
max_cols = 150  # Ilość kolumn (czas)

# Data storage
data = np.zeros((num_samples, max_cols))
times = np.zeros(max_cols)

# Speed of sound and sampling resolution
speed_of_sound = 330  # w powietrzu
sample_time = 13.2e-6  # 13.2 µs
sample_resolution = (speed_of_sound * sample_time * 100) / 2  # w cm

# Set up the plot
plt.ion()
fig, ax = plt.subplots()
waterfall = ax.imshow(data, aspect='auto', cmap='viridis', interpolation='nearest', vmin=0, vmax=350)
plt.colorbar(waterfall, ax=ax)

ax.invert_yaxis()
ax.set_ylabel('Distance (cm)')
tick_step = 50
ax.set_yticks(np.arange(0, num_samples, tick_step)[::-1])
ax.set_yticklabels([f'{dist:.2f}' for dist in np.arange(0, num_samples, tick_step)[::-1] * sample_resolution])
ax.grid(axis='y', linestyle='--', color='gray', linewidth=0.5)
ax.set_xticks(np.linspace(0, max_cols - 1, num=10))
ax.set_xticklabels(['' for _ in range(10)])
ax.set_xlabel('Elapsed Time (s)')
plt.show(block=False)

# Start time for elapsed time calculation
start_time = time.time()

# Function to parse data from serial input
def parse_data(line):
    try:
        line = line.decode('utf-8', errors='ignore').strip()
        if "sp" in line:
            sp_start = line.index("sp") + 2
            parts = line[sp_start:].split(', ')
            values = [int(x) for x in parts]
            return values
    except Exception as e:
        print(f"Error parsing line: {line} - {e}")
    return None

# Main loop for receiving and plotting data
while True:
    try:
        line = ser.readline()
        print(f"RAW: {line}")
        new_data = parse_data(line)

        if new_data:
            if len(new_data) != num_samples:
                print(f"[!] Warning: received {len(new_data)} samples (expected {num_samples})")
                continue

            elapsed_time = time.time() - start_time

            # Shift data and insert new column
            data = np.roll(data, -1, axis=1)
            data[:, -1] = new_data

            # Update plot
            waterfall.set_data(data)
            ax.set_title(f'Waterfall Chart\nSample Resolution: {sample_resolution * 4:.2f} cm')

            fig.canvas.flush_events()
            plt.draw()
            plt.pause(0.01)

    except KeyboardInterrupt:
        print("Exiting...")
        break

# Close serial connection
ser.close()
