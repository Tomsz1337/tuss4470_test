#pragma once

#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Inicjalizuje program PIO do generowania impulsów
/// \param pio        Instancja PIO (np. pio0)
/// \param pin        Numer pinu do wyjścia PWM
/// \param freqHz     Częstotliwość impulsów (np. 40000)
/// \return           Numer maszyny stanu PIO (0-3) lub -1 przy błędzie
int8_t pulse_gen_program_init(PIO pio, uint pin, uint freqHz);

/// Uruchamia generację zadanej liczby impulsów
/// \param pio        Instancja PIO
/// \param sm         Numer maszyny stanu
/// \param pulses     Liczba impulsów do wygenerowania
void pulse_gen_start(PIO pio, int8_t sm, uint pulses);

#ifdef __cplusplus
}
#endif
