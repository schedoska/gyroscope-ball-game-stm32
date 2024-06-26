#include "ball_sound.h"

// Funkcja inicjalizująca tablice z próbkami wstawiając funkcję sinusa
void generateSineWave(uint32_t frequency, uint32_t amplitude, uint32_t timer_frequency)
{
	amplitude /= 2;		// Amplituda w argumencie ma zakres (0-4096) więc trzeba ją zmniejszyć do max. 2048
	if(amplitude > 2047) amplitude = 2047;	// Zabezpieczenie przez zbyt wielkimi wartościami

	float period = 1.0 / timer_frequency;
	for(int t = 0; t < SAMPLE_NUMBER; t++)
	{
		sound_wav[t] =	amplitude * (sinf(2.0 * PI_CONST_SOUND * t * period * frequency) + 1);
	}
}

// Funkcja wstawiająca do tablicy próbek w wybranym miejscy sinusoidę o wybranej częstotliwości
void addSineWaveAt(uint32_t frequency, uint32_t amplitude, uint32_t timer_frequency, uint32_t begin, uint32_t end)
{
	amplitude /= 2;
	float period = 1.0 / timer_frequency;
	// Zabezpieczenie przed przekroczniem wielkości tablicy
	if(end > SAMPLE_NUMBER)
	{
		end = SAMPLE_NUMBER;
	}
	for(int t = begin; t < end; t++)
	{
		int sample = amplitude * sinf(2.0 * PI_CONST_SOUND * t * period * frequency);
		// Sygnały są ze sobą mieszane można więc na raz odtwarzać wiele częstotliwość
		sound_wav[t] = sample + (int)sound_wav[t];
		// Zabezpieczenie przed zbyt wielkimi wartościami
		if(sound_wav[t] > 4095) sound_wav[t] = 4095;
	}
}

// Funkjca generująca prostą melodię metodą manualną
void initMelody_1(){
	generateSineWave(2000, 000, 10000);
	addSineWaveAt(500, MAX_AMPLITUDE, TIMER_FREQUENCY, 0, 500);
	addSineWaveAt(800, MAX_AMPLITUDE, TIMER_FREQUENCY, 500, 1000);
	addSineWaveAt(650, MAX_AMPLITUDE, TIMER_FREQUENCY, 1000, 1500);
	addSineWaveAt(900, MAX_AMPLITUDE, TIMER_FREQUENCY, 1500, 2000);
	addSineWaveAt(1200, MAX_AMPLITUDE, TIMER_FREQUENCY, 2000, 2500);
	addSineWaveAt(1600, MAX_AMPLITUDE, TIMER_FREQUENCY, 2500, 3000);
	addSineWaveAt(650, MAX_AMPLITUDE, TIMER_FREQUENCY, 3000, 3500);
	addSineWaveAt(800, MAX_AMPLITUDE, TIMER_FREQUENCY, 3500, 4000);
	addSineWaveAt(1000, MAX_AMPLITUDE, TIMER_FREQUENCY, 4000, 4500);
	addSineWaveAt(700, MAX_AMPLITUDE, TIMER_FREQUENCY, 4500, 5000);
	addSineWaveAt(2300, MAX_AMPLITUDE, TIMER_FREQUENCY, 5000, 5500);
	addSineWaveAt(2500, MAX_AMPLITUDE, TIMER_FREQUENCY, 5500, 6000);
	addSineWaveAt(3000, MAX_AMPLITUDE, TIMER_FREQUENCY, 6000, 6300);
}

// Funkcja genrując aprostą metlodię metodą proceduralną
void initMelody_2(){
	generateSineWave(2000, 000, 10000);

	uint32_t freqArray[13] = {800, 1200, 1600, 1200, 800, 400, 900, 1800, 2200, 2500, 2000, 2500, 2900};

	for(int i = 0; i < 13; i++)
	{
		addSineWaveAt(freqArray[i], MAX_AMPLITUDE, TIMER_FREQUENCY, i * 500, (i + 1) * 500);
	}
}
