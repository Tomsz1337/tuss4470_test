import serial
import csv
import time

# Ustawienia portu szeregowego (dopasuj do swojego systemu)
port = 'COM5'  # Zmień na odpowiedni port
baudrate = 1000000  # Ustawienia zgodne z tymi w Arduino

# Inicjalizacja połączenia szeregowego
ser = serial.Serial(port, baudrate, timeout=1)

# Czekanie na dane z Arduino
time.sleep(2)  # Opóźnienie na stabilizację połączenia

# Tworzenie pliku CSV do zapisu
with open('pomiar_adc.csv', mode='w', newline='') as file:
    writer = csv.writer(file)

    # Wczytywanie danych z Arduino
    while True:
        line = ser.readline().decode('utf-8').strip()  # Odczyt jednej linii
        if line:
            try:
                # Sprawdzanie, czy linia zawiera dane
                if line.startswith("echo_index"):
                    print(f"{line}")
                else:
                    # Rozdzielanie danych CSV
                    values = line.split(',')
                    writer.writerow(values)
                    print(f"Zapisano: {values}")
            except Exception as e:
                print(f"Błąd podczas zapisu danych: {e}")
