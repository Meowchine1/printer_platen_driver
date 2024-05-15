
//#define ENCODER_VAL_COUNT 10

#define ENC_FAST_SCROLL_SPEED  100
#define ENC_SCROLL 1240

typedef struct {
//	uint16_t Pin;
//	GPIO_TypeDef* Port;
//	int32_t Position;
//	uint32_t Values_A[ENCODER_VAL_COUNT];
//	uint32_t Values_B[ENCODER_VAL_COUNT];
	uint32_t enc_prev;
	uint32_t enc_increment;
	uint32_t enc_counter;

	uint32_t enc_relative_counter;

} Encoder_t;
