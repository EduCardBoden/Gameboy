#include "avr/io.h"
#include "drivers/spi.h"
#include "drivers/ili9341.h"

int main(void)
{
   spi_init();
   ili9341_init();

   //test
   ili9341_fill_screen(COLOR_RED);

   while (1) {}
   return 0;

}