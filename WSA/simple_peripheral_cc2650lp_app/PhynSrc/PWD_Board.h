/*******************************************************************************
PWD_Board.h
*******************************************************************************/

#ifndef __PWD_Board_H__
#define __PWD_Board_H__

#include <ti/drivers/PIN.h>
#include <driverlib/ioc.h>

extern const PIN_Config BoardGpioInitTable[];

// LEDs are active high 
#define LED_ON                        1 
#define LED_OFF                       0


// Mapping of pins to board signals using general board aliases
//      <board signal alias>         <pin mapping>
// ----------------------------------------------------------------
#define   PIN_UART_RX                       IOID_1  /* BT_UART_TX on Schematic */
#define   PIN_UART_TX                       IOID_2  /* BT_UART_RX on Schematic */
#define   PIN_LED_BLUE                      IOID_7
#define   PIN_LED_RED                       IOID_8



#if 0
    //#define Board_DK_LED3                       IOID_5          /* P1.2  */
    //#define Board_DK_LED4                       IOID_6          /* P1.4  */


    /* Button Board */
    #define Board_KEY_SELECT                    IOID_7          /* P1.14 */
    #define Board_KEY_UP                        IOID_4          /* P1.10 */
    #define Board_KEY_DOWN                      IOID_3          /* P1.12 */
    #define Board_KEY_LEFT                      PIN_UNASSIGNED
    #define Board_KEY_RIGHT                     PIN_UNASSIGNED
    /* LCD  Board */
    #define Board_3V3_EN                        PIN_UNASSIGNED
    #define Board_LCD_MODE                      PIN_UNASSIGNED
    #define Board_LCD_RST                       PIN_UNASSIGNED
    #define Board_LCD_CSN                       PIN_UNASSIGNED
    /* UART Board */
    #define Board_UART_RX                       IOID_1          /* P1.7  */
    #define Board_UART_TX                       IOID_2          /* P1.9  */
    #define Board_UART_CTS                      PIN_UNASSIGNED
    #define Board_UART_RTS                      PIN_UNASSIGNED
    /* SPI Board */
    #define Board_SPI0_MISO                     IOID_0          /* P1.20 */
    #define Board_SPI0_MOSI                     IOID_9          /* P1.18 */
    #define Board_SPI0_CLK                      IOID_8          /* P1.16 */
    #define Board_SPI0_CSN                      PIN_UNASSIGNED
    /* PWM outputs */
    #define Board_PWMPIN0                       Board_DK_LED3
    #define Board_PWMPIN1                       Board_DK_LED4
    #define Board_PWMPIN2                       PIN_UNASSIGNED
    #define Board_PWMPIN3                       PIN_UNASSIGNED
    #define Board_PWMPIN4                       PIN_UNASSIGNED
    #define Board_PWMPIN5                       PIN_UNASSIGNED
    #define Board_PWMPIN6                       PIN_UNASSIGNED
    #define Board_PWMPIN7                       PIN_UNASSIGNED
#endif

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/

#define Board_UART                  PWD_UART0          /* Generic UART instance identifiers */

#define Board_CRYPTO                CC2650DK_4XS_CRYPTO0        /* Generic Crypto instance identifiers */

#define Board_GPTIMER0A             PWD_GPTIMER0A      /* Generic GPTimer instance identifiers */
#define Board_GPTIMER0B             PWDGPTIMER0B
#define Board_GPTIMER1A             PWDGPTIMER1A
#define Board_GPTIMER1B             PWDGPTIMER1B
#define Board_GPTIMER2A             PWDGPTIMER2A
#define Board_GPTIMER2B             PWDGPTIMER2B
#define Board_GPTIMER3A             PWDGPTIMER3A
#define Board_GPTIMER3B             PWDGPTIMER3B




#if 0
#define Board_PWM0                  CC2650DK_4XS_PWM0           /* Generic PWM instance identifiers */
#define Board_PWM1                  CC2650DK_4XS_PWM1
#define Board_PWM2                  CC2650DK_4XS_PWM2
#define Board_PWM3                  CC2650DK_4XS_PWM3
#define Board_PWM4                  CC2650DK_4XS_PWM4
#define Board_PWM5                  CC2650DK_4XS_PWM5
#define Board_PWM6                  CC2650DK_4XS_PWM6
#define Board_PWM7                  CC2650DK_4XS_PWM7
#define Board_TRNG                  CC2650DK_4XS_TRNG0          /* Generic TRNG instance identiifer */


    /* These #defines allow us to reuse TI-RTOS across other device families */
    #define     Board_LED0              Board_DK_LED3
    #define     Board_LED1              Board_DK_LED4
    #define     Board_LED2              Board_LED0

    #define     Board_BUTTON0           Board_KEY_UP
    #define     Board_BUTTON1           Board_KEY_DOWN

    #define     Board_UART0             Board_UART
    #define     Board_AES0              Board_AES
    #define     Board_WATCHDOG0         Board_WATCHDOG
#endif

#define     Board_initGeneral() { \
    Power_init(); \
    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) \
        {System_abort("Error with PIN_init\n"); \
    } \
}

#define     Board_initGPIO()
#define     Board_initPWM()         PWM_init()
#define     Board_initSPI()         SPI_init()
#define     Board_initUART()        UART_init()
#define     Board_initWatchdog()    Watchdog_init()
#define     GPIO_toggle(n)
#define     GPIO_write(n,m)

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    PWD_CryptoName
 *  @brief  Enum of Crypto names on the CC2650 dev board
 */
typedef enum PWD_CryptoName {
    PWD_CRYPTO0 = 0,
    PWD_CRYPTOCOUNT
} PWD_CryptoName;

/*!
 *  @def    PWD_SPIName
 *  @brief  Enum of SPI names on the CC2650 dev board
 */
typedef enum PWD_SPIName {
    PWD_SPI0 = 0,
    PWD_SPICOUNT
} PWD_SPIName;

/*!
 *  @def    PWD_TRNGName
 *  @brief  Enum of TRNG names on the board
 */
typedef enum PWD_TRNGName {
    PWD_TRNG0 = 0,
    PWD_TRNGCOUNT
} PWD_TRNGName;

/*!
 *  @def    PWD_UARTName
 *  @brief  Enum of UARTs on the CC2650 dev board
 */
typedef enum PWD_UARTName {
    PWD_UART0 = 0,
    PWD_UARTCOUNT
} PWD_UARTName;

/*!
 *  @def    PWD_UdmaName
 *  @brief  Enum of DMA buffers
 */
typedef enum PWD_UdmaName {
    PWD_UDMA0 = 0,
    PWD_UDMACOUNT
} PWD_UdmaName;

/*!
 *  @def    PWD_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum PWD_GPTimerName
{
    PWD_GPTIMER0A = 0,
    PWD_GPTIMER0B,
    PWD_GPTIMER1A,
    PWD_GPTIMER1B,
    PWD_GPTIMER2A,
    PWD_GPTIMER2B,
    PWD_GPTIMER3A,
    PWD_GPTIMER3B,
    PWD_GPTIMERPARTSCOUNT
} PWD_GPTimerName;

/*!
 *  @def    PWD_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum PWD_GPTimers
{
    PWD_GPTIMER0 = 0,
    PWD_GPTIMER1,
    PWD_GPTIMER2,
    PWD_GPTIMER3,
    PWD_GPTIMERCOUNT
} PWD_GPTimers;

/*!
 *  @def    PWD_PWM
 *  @brief  Enum of PWM outputs on the board
 */
typedef enum PWD_PWM
{
    PWD_PWM0 = 0,
    PWD_PWM1,
    PWD_PWM2,
    PWD_PWM3,
    PWD_PWM4,
    PWD_PWM5,
    PWD_PWM6,
    PWD_PWM7,
    PWD_PWMCOUNT
} PWD_PWM;


#endif 
