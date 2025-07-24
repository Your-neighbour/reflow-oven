#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <ESP32RotaryEncoder.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const int knobSW = 20;
const int knobCLK = 21;
const int knobDT = 22;

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 25); // ESP32

MUIU8G2 mui;
RotaryEncoder knob(knobCLK, knobDT);

muif_t muif_list[] = {  
  MUIF_VARIABLE("BN", NULL, mui_u8g2_btn_exit_wm_fi)
};

fds_t fds_data[] = 
MUI_FORM(1)
MUI_XYT("BN", 64, 30, " Select Me ")
;
void knobCallback( long value )
{
    // This gets executed every time the knob is turned
    Serial.printf( "Value: %i\n", value );
}


void setup(void) {
    u8g2.begin();
    pinMode(knobSW, INPUT);
    pinMode(knobCLK, INPUT);
    pinMode(knobDT, INPUT);
    
    Serial.begin(9600);

    knob.setEncoderType( EncoderType::HAS_PULLUP );

    // Range of values to be returned by the encoder: minimum is 1, maximum is 10
    // The third argument specifies whether turning past the minimum/maximum will
    // wrap around to the other side:
    //  - true  = turn past 10, wrap to 1; turn past 1, wrap to 10
    //  - false = turn past 10, stay on 10; turn past 1, stay on 1
    knob.setBoundaries( 1, 3, true );

    // The function specified here will be called every time the knob is turned
    // and the current value will be passed to it
    knob.onTurned( &knobCallback );
    knob.begin();



    mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
    mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}




uint8_t is_redraw = 1;
void loop(void) {
  

  

  if (digitalRead(knobSW) == HIGH)
  {
    Serial.println("button press");
    mui.sendSelect();
  }

  u8g2.setFont(u8g2_font_helvR08_tr);

  //select screen
  if ( mui.isFormActive() )
  {
    // menu is active: draw the menu 
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
          mui.draw();
      } while( u8g2.nextPage() );
      is_redraw = 0;
    }
    // handle events 
    switch(u8g2.getMenuEvent()) {
      case U8X8_MSG_GPIO_MENU_SELECT:
        mui.sendSelect();
        is_redraw = 1;
        break;
    }

  } else {  
    //encoder value + button screen
    u8g2.firstPage();
    do {
        u8g2.setCursor(0,20);
        u8g2.println(knob.getEncoderValue());
        u8g2.println(digitalRead(knobSW));
    } while( u8g2.nextPage() );
  } // mui.isFormActive()
   

  //lastStateCLK = currentStateCLK; 

  delay(1);
}
