#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

bool Correct_Answer_Is_Entered = false;
LiquidCrystal_I2C Liquid_Crystal_Display (0x27, 16, 2);
short Number_1 = random (1, 10), Number_2 = random (1, 10), Sum = Number_1 + Number_2;
String Sum_String = "";

void setup ()
{
	Liquid_Crystal_Display.init ();
	Liquid_Crystal_Display.backlight ();
	Liquid_Crystal_Display.setCursor (5, 0);
	Liquid_Crystal_Display.print (String (Number_1) + " + " + String (Number_2));
	Liquid_Crystal_Display.setCursor (6, 1);
	Liquid_Crystal_Display.print ("=");
	Serial.begin (9600);
}

void loop ()
{
	if (Correct_Answer_Is_Entered)
	{
		Number_1 = random (1, 10);
		Number_2 = random (1, 10);
		Sum = Number_1 + Number_2;
		Liquid_Crystal_Display.clear ();
		Liquid_Crystal_Display.setCursor (5, 0);
		Liquid_Crystal_Display.print (String (Number_1) + " + " + String (Number_2));
		Liquid_Crystal_Display.setCursor (6, 1);
		Liquid_Crystal_Display.print ("=");
		Correct_Answer_Is_Entered = false;
	}
	if (Serial.available ()) 
	{
		while (Serial.available ()) 
		{
			char Input = Serial.read ();
			if (isDigit (Input))
			{
				Sum_String = Sum_String + Input;
			}
			if (Input == '\n')
			{
				Serial.println (Sum_String);
				if (Sum_String.toInt () == Sum)
				{
					Correct_Answer_Is_Entered = true;
					if (Sum < 10)
					{
						Liquid_Crystal_Display.setCursor (8, 1);
						Liquid_Crystal_Display.print (String (Sum));
					}
					else
					{
						Liquid_Crystal_Display.setCursor (7, 1);
						Liquid_Crystal_Display.print (String (Sum));
					}
					delay (1000);
				}
				Sum_String = "";
			}
		}
	}
}