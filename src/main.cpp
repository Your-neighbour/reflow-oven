#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 25); // ESP32

MUIU8G2 mui;

muif_t muif_list[] = {  
  MUIF_VARIABLE("BN", NULL, mui_u8g2_btn_exit_wm_fi)
};

fds_t fds_data[] = 
MUI_FORM(1)
;

const int knobSW = 20;
const int knobCLK = 21;
const int knobDT = 22;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

void setup(void) {
  u8g2.begin();
  pinMode(knobSW, INPUT);
  pinMode(knobCLK, INPUT);
  pinMode(knobDT, INPUT);
  Serial.begin(9600);

  lastStateCLK = digitalRead(knobCLK);

  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}

uint8_t is_redraw = 1;
void loop(void) {
  u8g2.setFont(u8g2_font_helvR08_tr);
  if ( mui.isFormActive() )
  {
    /* menu is active: draw the menu */
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
          mui.draw();
      } while( u8g2.nextPage() );
      is_redraw = 0;
    }
    /* handle events */
    switch(u8g2.getMenuEvent()) {
      case U8X8_MSG_GPIO_MENU_SELECT:
        mui.sendSelect();
        is_redraw = 1;
        break;
    }
  } else {  
    /* menu not active: show something else */
    u8g2.firstPage();
    do {
        u8g2.setCursor(0,20);
        u8g2.print(millis());
    } while( u8g2.nextPage() );
  } /* mui.isFormActive() */

  currentStateCLK = digitalRead(knobCLK);

  //pulse detected
  if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH)
  {
    //ccw
    if (digitalRead(knobDT) != currentStateCLK) {
      counter++;
      currentDir = "CW";
    } else {
      // Encoder is rotating CW so increment
      counter--;
      currentDir = "CCW";
    }
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
    
  }

  lastStateCLK = currentStateCLK; 

  if (digitalRead(knobSW) == HIGH)
  {
    Serial.println("button press"); 
  }

  delay(1);
}
