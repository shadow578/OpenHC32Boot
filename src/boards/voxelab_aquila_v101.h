/**
 * board configuration for Voxelab V1.0.1 / V1.0.2 mainboard
 * as used in the Voxelab Aquila X2
 */
//
// Board Identification
//
#define BOARD_NAME "Voxelab Aquila V1.0.1"

//
// Serial Configuration(s)
//
// host serial, TX on PA9 @ 115200 baud
#define HOST_SERIAL_PERIPHERAL M4_USART2
#define HOST_SERIAL_BAUDRATE 115200
#define HOST_SERIAL_TX_PIN PA9

// DWIN Screen serial, TX on PC0 @ 115200 baud
#if ENABLE_UI == 1
#define SCREEN_SERIAL_PERIPHERAL M4_USART1
#define SCREEN_SERIAL_BAUDRATE 115200
#define SCREEN_SERIAL_TX_PIN PC0

//
// UI Button Configuration
//
#define UI_BUTTON_PIN PB14
#define UI_BUTTON_MODE INPUT_PULLUP // INPUT, INPUT_PULLUP
#endif // ENABLE_UI

//
// SD Card Configuration
//
#define SDIO_D0_PIN PC8
#define SDIO_D1_PIN PC9
#define SDIO_D2_PIN PC10
#define SDIO_D3_PIN PC11
#define SDIO_CLK_PIN PC12
#define SDIO_CMD_PIN PD2
#define SDIO_DET_PIN PA10
