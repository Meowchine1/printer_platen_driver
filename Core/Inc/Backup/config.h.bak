

#include "encoder.h"

// ------------------- MANAGEMENT DEFINITIONS --------------------------------------
#define TESTING // Определение для отладки без кнопок перемещения столика
#define TEST_ASF_PE // Определение для теста датчиков.


static const uint32_t TABLE_LENGTH = 50;

static const uint16_t PE_START_ENCODER_COUNT = ENC_SCROLL + 300; // количество импульсов энкодера перед началом печати,
														  // которое должно пройти перед тем как включить датчик PE

static const uint16_t PE_END_ENCODER_COUNT = ENC_SCROLL + 300; // количество импульсов энкодера после завершения печати,
														// которое должно пройти перед тем как выключить датчик PE

static const uint16_t ASF_ENCODER_COUNT = ENC_SCROLL; // количество импульсов энкодера, которое должно пройти перед тем как включить датчик ASF
static const uint16_t ASF_DURATION_MILLIS = 800; // Продолжительность работы датчика ASF в миллисекундах
