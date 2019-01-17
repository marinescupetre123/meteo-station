#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <LiquidCrystal.h>
#define DHT11_PIN 7
uint8_t c = 0, I_RH, D_RH, I_Temp, D_Temp, CheckSum;

void Request()        /* Microcontroller send start pulse/request */
{
    DDRD |= (1<<DHT11_PIN);
    PORTD |= (1<<DHT11_PIN); /* add this line */
    _delay_ms(100); /* add this line */
    PORTD &= ~(1<<DHT11_PIN);  /* set to low pin */
    _delay_ms(20);  /* wait for 20ms */
    PORTD |= (1<<DHT11_PIN);  /* set to high pin */
}

void Response()       /* receive response from DHT11 */
{
  DDRD &= ~(1 << DHT11_PIN);
  while (PIND & (1 << DHT11_PIN));
  while ((PIND & (1 << DHT11_PIN)) == 0);
  while (PIND & (1 << DHT11_PIN));
}

uint8_t Receive_data()      /* receive data */
{ int q;
  for (q = 0; q < 8; q++)
  {
    while ((PIND & (1 << DHT11_PIN)) == 0); /* check received bit 0 or 1 */
    _delay_us(30);
    if (PIND & (1 << DHT11_PIN)) /* if high pulse is greater than 30ms */
      c = (c << 1) | (0x01); /* then its logic HIGH */
    else      /* otherwise its logic LOW */
      c = (c << 1);
    while (PIND & (1 << DHT11_PIN));
  }
  return c;
}

int main(void)
{
  char data[5];
  LiquidCrystal lcd(12,10, 11, 5, 4, 3, 2);
   lcd.begin(16, 2);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Humidity =");
   lcd.setCursor(0, 1);
   lcd.print("Temp = ");
   
  _delay_ms(100);
  
  while (1)
  {
    
    Request();    /* send start pulse */
    Response();   /* receive response */
    I_RH = Receive_data(); /* store first eight bit in I_RH */
    D_RH = Receive_data(); /* store next eight bit in D_RH */
    I_Temp = Receive_data(); /* store next eight bit in I_Temp */
    D_Temp = Receive_data(); /* store next eight bit in D_Temp */
    CheckSum = Receive_data(); /* store next eight bit in CheckSum */
  
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Humidity =");
   lcd.setCursor(0, 1);
   lcd.print("Temp = ");
     
      itoa(I_RH, data, 10);
      lcd.setCursor(11, 0);
      lcd.print(data);
      lcd.print(".");

      itoa(D_RH, data, 10);
      lcd.print(data);
      lcd.print("%");

      itoa(I_Temp, data, 10);
      lcd.setCursor(7, 1);
      lcd.print(data);
      lcd.print(".");

      itoa(D_Temp, data, 10);
      lcd.print(data);
      lcd.print(" C   ");


    _delay_ms(1000);
  }
}
