/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stddef.h>
#include <_ansi.h>

_BEGIN_STD_C

#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"

#include "xmc_usic.h"
#include "xmc_spi.h"
#include "xmc_i2c.h"
#include "xmc_gpio.h"

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          32
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           130
#endif

// some LEDs don't display anything in first two columns
// #define SSD1306_WIDTH           130

#ifndef SSD1306_BUFFER_SIZE
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8
#endif

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

typedef enum {
    SSD1306_OK = 0x00,
    SSD1306_ERR = 0x01  // Generic error.
} SSD1306_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
    uint8_t DisplayOn;
	XMC_USIC_CH_t* channel;
	void (*delay_fnc)(uint32_t ms);
#if defined(SSD1306_USE_SPI)
	XMC_GPIO_PORT_t* rst_port;
	uint8_t rst_pin;
	XMC_GPIO_PORT_t* dc_port;
	uint8_t dc_pin;
#endif
} SSD1306_t;
typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1306_VERTEX;

extern SSD1306_t SSD1306;

// Procedure definitions
#if defined(SSD1306_USE_I2C)
void ssd1306_Init(XMC_USIC_CH_t* channel, void(*delay_fnc)(uint32_t));
#elif defined(SSD1306_USE_SPI)
void ssd1306_Init(XMC_USIC_CH_t* channel, XMC_GPIO_PORT_t* rst_port, uint8_t rst_pin, XMC_GPIO_PORT_t* dc_port, uint8_t dc_pin, void(*delay_fnc)(uint32_t));
#endif

void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color);
void ssd1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_COLOR color);
void ssd1306_Polyline(const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_COLOR color);
void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void ssd1306_SetContrast(const uint8_t value);
/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void ssd1306_SetDisplayOn(const uint8_t on);
/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t ssd1306_GetDisplayOn();

// Low-level procedures
void ssd1306_Reset(void);
void ssd1306_WriteCommand(uint8_t byte);
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size);
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len);

_END_STD_C

#endif // __SSD1306_H__
