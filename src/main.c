#include <avr/io.h>
#include "drivers/spi.h"
#include "drivers/ili9341.h"
#include "inputs/button.h"
#include "font/font.h"

int main(void)
{
   
   spi_init();
   ili9341_init();
   button_init();
   //test
   ili9341_fill_screen(COLOR_RED);
   uint8_t is_red = 1;
   draw_string("123456789", 10, 10, COLOR_BLUE, COLOR_BLACK, 2);

   while (1) 
   {

      if(button_was_pressed()) {

         if(is_red == 1) {

            ili9341_fill_screen(COLOR_GREEN);
            is_red = 0;
         }
         else {

            ili9341_fill_screen(COLOR_RED);
            is_red = 1;
         }
      }
   }
   return 0;
}