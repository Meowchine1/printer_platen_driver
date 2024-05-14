

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"
#include "button.h"
#include "usbd_cdc_if.h"
#include "ender.h"
#include "encoder.h"
#include "motor.h"

#define TESTING

char buf_tx[100];
uint8_t buflen;
uint8_t buf[50];

#define SHORT_DURATION 100
#define LONG_DURATION 3000

Button_t btn_up, btn_down, btn_in, btn_back;
Ender_t PE, Laser, Endstop;
Encoder_t Enc;
Motor_t motor_vertical, motor_horizontal;
uint32_t buf_A[ENCODER_VAL_COUNT], buf_B[ENCODER_VAL_COUNT];
uint32_t buf_vertical[ENCODER_VAL_COUNT], buf_horizontal[ENCODER_VAL_COUNT];

// -----------------------------------Bit array MACRO and DEFINITION---------------------------
typedef uint8_t BARR_ELTYPE;
#define BARR_ELBITS 8
#define BARR_COUNT 9

#define BARR_ARRAYSIZE(N) ((N + BARR_ELBITS - 1) / BARR_ELBITS)
#define BARR_ELNUM(N) (N / BARR_ELBITS)
#define BARR_BITNUM(N) (N % BARR_ELBITS)
#define BARR_SET(barr, N) ( barr[BARR_ELNUM(N)] |= ( (BARR_ELTYPE)1 << BARR_BITNUM(N) ) )
#define BARR_CLEAR(barr, N) ( barr[BARR_ELNUM(N)] &= ( ~( (BARR_ELTYPE)1 << BARR_BITNUM(N) ) ) )
#define BARR_INV(barr, N) (barr[BARR_ELNUM(N)] ^= ( (BARR_ELTYPE)1 << BARR_BITNUM(N) ))
#define BARR_TEST(barr, N) (barr[BARR_ELNUM(N)] & ( (BARR_ELTYPE)1 << BARR_BITNUM(N) ))

BARR_ELTYPE State_flag [BARR_COUNT];

// ------------------------------ DESCRIPTION ----------------------------------------------------------------------
// [ button ] - {}, {},  RELEASED, LONG_PRESSED, SHORT_PRESSED, PRESSED, DEBOUNCE, IDLE
// [ ender ] - {}, {}, {}, {}, {}, CHECKED, DEBOUNCE, IDLE
// [ encoder ] - {}, {}, {}, {}, {}, {}, {}, {}
//
// btn_up(0-7), btn_down(8-15), btn_in(16-23), btn_back(24-31), PE(32-39), Laser(40-47), Endstop(48-55), Enc(56-63)
//------------------------------------------------------------------------------------------------------------------

typedef enum{   		// Хранит то, что в настоящее время делает стол.
	SEND_BACK,			// 1 = Необходимо отправить обратно для возвращения при запуске
	SEND_IN,            // 2 = Отправить стол в
	SEND_UP,
	SEND_DOWN,
	READY_TO_PRINT,     // 3 = Готовность к печати / Печать
	SCROLLING,
	PRINTING,
	ROLLER_EXTRACT,      // 4 = Извлечение валика
	INIT     // Holding in the front for button press
} PlatenState;

const char* stateStrings[] = {"SEND_BACK", "SEND_IN", "SEND_UP", "SEND_DOWN", "READY_TO_PRINT", "SCROLLING", "PRINTING", "ROLLER_EXTRACT", "INIT"};

PlatenState platenState = SEND_BACK;
int32_t stepperPosition;  // Текущее положение шагового двигателя
int32_t stepperCount;     // Где должен быть шаговый двигатель

uint8_t peState;   // Содержит переменную, которую мы устанавливаем, когда устанавливаем высокий и низкий уровень датчика PE
uint8_t passCount; // Отслеживает, сколько раз проходил валик при нажатии одной кнопки
uint8_t maxPasses; // Должно быть 1 или 2

int32_t positionEncoder  = -800;
int32_t newEncoder;

uint8_t asfState = 0;





// =============================== CONFIGURATION =================================



long enc_prev_time = 0;
uint32_t enc_difference = 0;
uint32_t enc_prev_counter = 0;
uint32_t enc_counter = 0;
const uint16_t ENC_FAST_SCROLL_SPEED = 100;
const uint16_t ENC_SCROLL = 1240;

//int16_t count = 0;
//int16_t position = 0;

//uint32_t bufA[10];
//uint32_t bufB[10];

const uint32_t TABLE_LENGTH = 50;

const uint16_t PE_ENCODER_COUNT = ENC_SCROLL + 300;
const uint16_t ASF_ENCODER_COUNT = ENC_SCROLL;
uint16_t Start_count_encoder = 0;

uint32_t PE_Start_time_duration = 0;
uint32_t ASF_Start_time_duration = 0;

const uint16_t PE_DURATION_MILLIS = 3000;
const uint16_t ASF_DURATION_MILLIS = 800;
uint8_t ASF_state = 0;
uint8_t PE_state = 0;

int height_limit(){
	return 1;
}

int putting_forward_limit(){
	return 1;
}


