import serial
import csv
import time

# Ustawienia portu szeregowego
port = 'COM8'  
baudrate = 115200  

ser = serial.Serial(port, baudrate, timeout=1)

# Tworzenie pliku CSV do zapisu
with open('pomiar_adc.csv', mode='w', newline='') as file:
    writer = csv.writer(file)

    while True:
        line = ser.readline().decode('utf-8').strip()
        if not line:
            continue

        if line == "sp":
            data_line = ser.readline().decode('utf-8').strip()
            values = data_line.split(',')
            if len(values) == 1000:
                writer.writerow(values)
                print(f"Zapisano: {values[:3]} ... {values[-3:]}")
            else:
                print(f"Błędna ilość danych: {len(values)}")
