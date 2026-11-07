/**
 * TomatoCube IR & Magnet Polarity Tester
 * ---------------------------------------
 * BOARD: ATtiny1614/1604/814/804/414/404/214/204
 * Chip: ATtiny1614
 * Clock Speed: 20MHz
 * millis()/micros(): "TCD0 (1 series only, default there)"
 * ----------------------------------------
 */
#include <LiquidCrystal.h>

//LCD Screen

#define LCD_RS 4  //PB3
#define LCD_EN 5  //PB2

#define LCD_D4 0  //PA4
#define LCD_D5 1  //PA5
#define LCD_D6 2  //PA6
#define LCD_D7 3  //PA7

#define SWITCH 8  //PA1
#define IR_REC 9  //PA2

#define Hall_EFF 10 //PA3

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#include "PinDefinitionsAndMore.h"
#define IR_RECEIVE_PIN IR_REC 
#define DISABLE_LED_FEEDBACK_FOR_RECEIVE
#define MARK_EXCESS_MICROS    20 

#include <IRremote.h>

char buf[100];
bool addressMode = false;

int val = 0;

//--------------------------------------------------------------------------------------------
//Setup hardware pins

void setup()
{
  pinMode(SWITCH,INPUT_PULLUP);
  pinMode(Hall_EFF,INPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(12, 2);
  lcd.clear();


  // Print a message to the LCD.
  lcd.print("IR REMOTE");

  // Start the receiver
  IrReceiver.begin(IR_RECEIVE_PIN);
}

uint8_t reverseBits(uint8_t value)
{
    value = ((value & 0xF0) >> 4) | ((value & 0x0F) << 4);
    value = ((value & 0xCC) >> 2) | ((value & 0x33) << 2);
    value = ((value & 0xAA) >> 1) | ((value & 0x55) << 1);

    return value;
}

uint16_t reverseBitsPerByte(uint16_t value)
{
    uint16_t result = 0;

    result |= (uint16_t)reverseBits((uint8_t)(value >> 8)) << 8;
    result |= reverseBits((uint8_t)(value & 0xFF));

    return result;
}

//--------------------------------------------------------------------------------------------
//Main program Loop

void loop() 
{
  bool decoded = IrReceiver.decode();
  if (decoded || ((digitalRead(SWITCH) == LOW) ^ addressMode))
  {
    lcd.clear();
    addressMode = (digitalRead(SWITCH) == LOW);
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) 
    {
      lcd.print("Code too big");
      lcd.setCursor(0, 1);
      lcd.print(buf);
    } 
    else if (addressMode)
    {
      if (IrReceiver.decodedIRData.protocol == UNKNOWN)
      { 
        lcd.print("Unknown");
        sprintf(buf, "0x%X", IrReceiver.decodedIRData.decodedRawData);
      }
      else if (IrReceiver.decodedIRData.protocol == RC5)
      {
        int sys = IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] & 0x1F;
        int cmd = IrReceiver.decodedIRData.rawDataPtr->rawbuf[1] >> 2;
        sprintf(buf, "SY:%d CM:%d", sys, cmd);
        lcd.print(buf);

        int l = IrReceiver.decodedIRData.rawDataPtr->rawlen;
        lcd.setCursor(0, 1);
        sprintf(buf, "%d:", l);
        lcd.print(buf);
        int x = 3;
        int i = 0;
        while (x < 16 && i < l)
        {
          lcd.setCursor(x, 1);
          sprintf(buf, "%02X", (byte)IrReceiver.decodedIRData.rawDataPtr->rawbuf[i]);
          lcd.print(buf);
          i++;
          x+=2;
        }
      }
      else
      {
        sprintf(buf, " A=%04X %04X", IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
        lcd.print(buf);
        sprintf(buf, "~A=%04X %04X", reverseBitsPerByte(IrReceiver.decodedIRData.address), reverseBitsPerByte(IrReceiver.decodedIRData.command));
        lcd.setCursor(0, 1);
        lcd.print(buf);
      }
    }
    else
    {
      lcd.print(IrReceiver.getProtocolString(IrReceiver.decodedIRData.protocol));
      if (IrReceiver.decodedIRData.protocol == RC5)
      {
        bool field = (IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] & 0x40);
        bool ctrl = (IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] & 0x20);
        int sys = IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] & 0x1F;
        int cmd = IrReceiver.decodedIRData.rawDataPtr->rawbuf[1] >> 2;
        sprintf(buf, "CMD:%d", cmd);
        lcd.setCursor(6, 0);
        lcd.print(buf);
        sprintf(buf, "S:%d F:%d C:%d", sys, (field) ? 1 : 0, (ctrl) ? 1 : 0);
        lcd.setCursor(0, 1);
        lcd.print(buf);
      }
      else
      {
        sprintf(buf, "%02X", IrReceiver.decodedIRData.command);
        lcd.setCursor(10, 0);
        lcd.print(buf);
        sprintf(buf, "F=%X", IrReceiver.decodedIRData.flags);
        lcd.setCursor(0, 1);
        lcd.print(buf);
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_EXTRA_INFO)
        {
          sprintf(buf, "E=%04X", IrReceiver.decodedIRData.extra);
        }
        else
        {
          sprintf(buf, "A=%04X", IrReceiver.decodedIRData.address);
        }
        lcd.setCursor(6, 1);
        lcd.print(buf);
      }
    }
    if (decoded)
    {
      IrReceiver.resume();                            // Prepare for the next value
    }
  }

  // Hall Effect Sensor 
  lcd.setCursor(15, 0);
  val = analogRead(Hall_EFF);
  lcd.print(val > 600 ? "S" : val < 400 ?  "N" : "-" );
      
}
