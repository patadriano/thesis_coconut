
#include <LCD-I2C.h>


LCD_I2C lcd(0x27, 16, 2);

void setup() {
  
    lcd.begin();
    lcd.display();
    lcd.backlight();
}

void loop()
{
    lcd.print("     Hello"); // You can make spaces using well... spaces
    lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd.print("World!");
    delay(500);

    // Flashing the backlight
    for (int i = 0; i < 5; ++i)
    {
        lcd.backlight();
        delay(50);
        lcd.backlightOff();
        delay(50);
    }

    lcd.backlight();
    lcd.clear();
    delay(500);
}
