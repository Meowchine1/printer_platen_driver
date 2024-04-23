

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"
#include "button.h"
#include "motor.h"
#include "usbd_cdc_if.h"
#include "ender.h"
#include "encoder.h"

char buf_tx[100];


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
	READY_TO_PRINT,     // 3 = Готовность к печати / Печать
	ROLLER_EXTRACT,      // 4 = Извлечение валика
	HOLDING_IN_FRONT     // Holding in the front for button press
} PlatenState;

PlatenState platenState = SEND_BACK;
int32_t stepperPosition;  // Текущее положение шагового двигателя
int32_t stepperCount;     // Где должен быть шаговый двигатель

uint8_t peState;   // Содержит переменную, которую мы устанавливаем, когда устанавливаем высокий и низкий уровень датчика PE
uint8_t passCount; // Отслеживает, сколько раз проходил валик при нажатии одной кнопки
uint8_t maxPasses; // Должно быть 1 или 2

int32_t positionEncoder  = -800;
int32_t newEncoder;

uint8_t asfState = 0;




