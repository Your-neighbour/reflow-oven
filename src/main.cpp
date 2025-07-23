#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

/* Constructor */
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 20, /* CS=*/ 5, /* reset=*/ 22); // ESP32

/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {
  u8g2.begin();
}

/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(1000);
}