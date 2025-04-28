import csv
import matplotlib.pyplot as plt

# Wczytywanie danych z pliku CSV
def load_flat_data(filename):
    flat_data = []
    with open(filename, mode='r') as file:
        reader = csv.reader(file)
        for row in reader:
            for value in row:
                if value.strip() != '':
                    flat_data.append(int(value))
    return flat_data

# Nazwa pliku CSV
filename = 'pomiar_adc.csv'
data = load_flat_data(filename)

# Oś X – indeksy próbek
x = list(range(len(data)))

# Tworzenie wykresu
plt.figure(figsize=(10, 5))
plt.plot(x, data, label='ADC wartości', color='blue')

# Opisy osi i tytuł
plt.title('Wykres wartości ADC')
plt.xlabel('Numer próbki')
plt.ylabel('Wartość ADC')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
