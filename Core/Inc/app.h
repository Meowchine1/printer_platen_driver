#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "init_components.h"

// -------------------------------------------------------------------------------------------

char buf_tx[300];
uint8_t buflen;
uint8_t buf[300];

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

BARR_ELTYPE State_flag[BARR_COUNT];

// ------------------------------ DESCRIPTION ----------------------------------------------------------------------
// [ button ] - {}, {},  RELEASED, LONG_PRESSED, SHORT_PRESSED, PRESSED, DEBOUNCE, IDLE
// [ ender ] - {}, {}, {}, {}, {}, CHECKED, DEBOUNCE, IDLE
// [ encoder ] - {}, {}, {}, {}, {}, {}, {}, {}
//
// btn_up(0-7), btn_down(8-15), btn_in(16-23), btn_back(24-31), PE(32-39), Laser(40-47), Endstop(48-55), Enc(56-63)
//------------------------------------------------------------------------------------------------------------------

typedef enum {
	SEND_BACK,
	SEND_IN,
	SEND_UP,
	SEND_DOWN,
	READY_TO_PRINT,
	SCROLLING,
	PRINTING,
	ROLLER_EXTRACT,
	INIT
} PlatenState;

PlatenState platenState = INIT;

const char *stateStrings[] = { "SEND_BACK", "SEND_IN", "SEND_UP", "SEND_DOWN",
		"READY_TO_PRINT", "SCROLLING", "PRINTING", "ROLLER_EXTRACT", "INIT" };


int height_limit() {
	return 1;
}

int putting_forward_limit() {
	return 1;
}

