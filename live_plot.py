import serial
import numpy as np
import matplotlib.pyplot as plt

# Parametry
port = 'COM8'          # Twój port
baudrate = 115200
num_samples = 1000      # liczba próbek w jednej ramce
max_cols = 150          # liczba ramek na osi X wykresu
speed_of_sound = 340    # m/s (powietrze)
sample_time = 20e-6     # 20 us

sample_resolution = (speed_of_sound * sample_time * 100) / 2  # cm

# Inicjalizacja danych
data = np.zeros((num_samples, max_cols))

# Konfiguracja wykresu
plt.ion()
fig, ax = plt.subplots()
im = ax.imshow(data, aspect='auto', cmap='viridis', vmin=0, vmax=2000)
plt.colorbar(im, ax=ax)

ax.invert_yaxis()
tick_step = 50
ax.set_yticks(np.arange(0, num_samples, tick_step)[::-1])
ax.set_yticklabels([f'{dist:.2f}' for dist in np.arange(0, num_samples, tick_step)[::-1] * sample_resolution])
ax.set_xlabel('Elapsed Time (samples)')
ax.set_ylabel('Distance (cm)')

# Otwórz port szeregowy
ser = serial.Serial(port, baudrate, timeout=1)

try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line == "sp":
            data_line = ser.readline().decode('utf-8').strip()
            values = data_line.split(',')
            if len(values) == num_samples:
                frame = np.array([int(v) for v in values])
                
                # Przesuń dane w prawo i dodaj nową kolumnę z aktualną ramką
                data = np.roll(data, -1, axis=1)
                data[:, -1] = frame
                
                im.set_data(data)
                ax.set_title(f'Waterfall Live\nSample Resolution: {sample_resolution:.2f} cm')
                plt.pause(0.001)  # bardzo krótka pauza dla odświeżenia wykresu
            

except KeyboardInterrupt:
    print("Zakończono przez użytkownika")

finally:
    ser.close()
