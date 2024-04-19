
#define SHORT_PRESS_DURATION 100
#define LONG_PRESS_DURATION 3000

typedef enum {

	IDLE,
	DEBOUNCE,
	PRESSED,
	RELEASED

} Button_state;

struct Button {

	Button_state Btn_state;
	uint32_t Pin;
	uint32_t Port;

}btn_up, btn_down, btn_in, btn_back;


void refresh_btn_state(struct Button btn);

