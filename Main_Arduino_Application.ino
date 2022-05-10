/*
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int Correct_Answer_Is_Entered = false, Equation_Timings_are_Sent = false, Time_is_Up = false;
LiquidCrystal_I2C Liquid_Crystal_Display (0x27, 16, 2);
short Amount_of_Correctly_Solved_Equations = 0, Number_1 = random (1, 10), Number_2 = random (1, 10), Sum = Number_1 + Number_2;
String Sum_String = "", Times_Taken_to_Solve_an_Equation_String = "";

short Number_of_Sensors = 5, LEDs [] = {45, 47, 49, 51, 53}, Previous_Target, Score = 0, Touch_Sensors [] = {22, 24, 26, 28, 30};

long *Times_Taken_to_Solve_an_Equation;

long Start_of_the_Equation_Answer_Entrance = millis (), End_of_the_Equation_Answer_Entrance;

void setup ()
{
	short Pin;
	for (Pin = 0; Pin < Number_of_Sensors; Pin = Pin + 1)
	{
	pinMode (LEDs [Pin], OUTPUT);
	pinMode (Touch_Sensors [Pin], INPUT);
	}
	Liquid_Crystal_Display.init ();
	Liquid_Crystal_Display.backlight ();
	Liquid_Crystal_Display.setCursor (0, 0);
	Liquid_Crystal_Display.print (String (Number_1) + " + " + String (Number_2) + " = ");
	Liquid_Crystal_Display.setCursor (0, 1);
	Liquid_Crystal_Display.print ("Time left: ");
	Serial.begin (9600);
}

void loop()
{
	long Time_of_The_End_of_The_Trial, Time_of_The_Start_of_The_Trial;
	short LED, Target = random (0, Number_of_Sensors);
	if (Score < 10)
	{
		if (Score == 0)
		{
			Previous_Target = Target;
		}
		else
		{
			while (Previous_Target == Target)
			{
			Target = random (0, Number_of_Sensors);
			}
			Previous_Target = Target;
		}
		for (LED = 0; LED < Number_of_Sensors; LED = LED + 1)
		{
			digitalWrite (LEDs [LED], LOW);
		}
		digitalWrite (LEDs [Target], HIGH);
		Time_of_The_Start_of_The_Trial = millis ();
		while (!digitalRead (Touch_Sensors [Target]))
		{
			Enter_The_Answer ();
		}
		Time_of_The_End_of_The_Trial = millis () - Time_of_The_Start_of_The_Trial;
		Score = Score + 1;
		Serial.println ("{\"Touch Sensor Number\": " + String (Target) + ", \"Time taken to press the sensor\": " + String (Time_of_The_End_of_The_Trial / 1000.0) + "}");
	}
	else if (!Equation_Timings_are_Sent)
	{
		if (Amount_of_Correctly_Solved_Equations == 0)
		{
			Times_Taken_to_Solve_an_Equation_String = "[]";
		}
		else
		{
			int Index;
			Times_Taken_to_Solve_an_Equation_String = Times_Taken_to_Solve_an_Equation_String + "[";
			for (Index = 0; Index < Amount_of_Correctly_Solved_Equations - 1; Index = Index + 1)
			{
			Times_Taken_to_Solve_an_Equation_String = Times_Taken_to_Solve_an_Equation_String + String (Times_Taken_to_Solve_an_Equation [Index] / 1000.0) + ",";
			}
			Times_Taken_to_Solve_an_Equation_String = Times_Taken_to_Solve_an_Equation_String + String (Times_Taken_to_Solve_an_Equation [Amount_of_Correctly_Solved_Equations - 1] / 1000.0) + "]";
		}
		Serial.println (Times_Taken_to_Solve_an_Equation_String);
		Equation_Timings_are_Sent = true;
	}
	else
	{
		for (LED = 0; LED < Number_of_Sensors; LED = LED + 1)
		{
			digitalWrite (LEDs [LED], HIGH);
		}
		delay (1000);
		for (LED = 0; LED < Number_of_Sensors; LED = LED + 1)
		{
			digitalWrite (LEDs [LED], LOW);
		}
		delay (1000);
	}
}

void Set_Color (short Red, short Green, short Blue, short Red_Pin, short Green_Pin, short Blue_Pin)
{
	analogWrite (Red_Pin, Red);
	analogWrite (Green_Pin, Green);
	analogWrite (Blue_Pin, Blue);  
}

void Enter_The_Answer ()
{
	if (Correct_Answer_Is_Entered || Time_is_Up)
	{
		Number_1 = random (1, 10);
		Number_2 = random (1, 10);
		Sum = Number_1 + Number_2;
		Liquid_Crystal_Display.clear ();
		Liquid_Crystal_Display.setCursor (0, 0);
		Liquid_Crystal_Display.print (String (Number_1) + " + " + String (Number_2) + " = ");
		Liquid_Crystal_Display.setCursor (0, 1);
		Liquid_Crystal_Display.print ("Time left: ");
		Correct_Answer_Is_Entered = false;
		Time_is_Up = false;
		Start_of_the_Equation_Answer_Entrance = millis ();
	}
	if (Serial.available ()) 
	{
		while (Serial.available ()) 
		{
			End_of_the_Equation_Answer_Entrance = millis () - Start_of_the_Equation_Answer_Entrance;
			if ((End_of_the_Equation_Answer_Entrance) > 5000)
			{
				Time_is_Up = true;
				break;
			}
			Liquid_Crystal_Display.setCursor (11, 1);
			Liquid_Crystal_Display.print (String ((5000 - End_of_the_Equation_Answer_Entrance) / 1000) + " s");
			char Input = Serial.read ();
			if (isDigit (Input))
			{
				Sum_String = Sum_String + Input;
			}
			if (Input == '\n')
			{
				if (Sum_String.toInt () == Sum)
				{
					Correct_Answer_Is_Entered = true;
					Liquid_Crystal_Display.setCursor (8, 0);
					Liquid_Crystal_Display.print (String (Sum));
					Add_a_Time_Taken_To_Solve_an_Equation (End_of_the_Equation_Answer_Entrance);
					Amount_of_Correctly_Solved_Equations = Amount_of_Correctly_Solved_Equations + 1;
					delay (1000);
				}
				Sum_String = "";
			}
		}
	}
	End_of_the_Equation_Answer_Entrance = millis () - Start_of_the_Equation_Answer_Entrance;
	Liquid_Crystal_Display.setCursor (11, 1);
	Liquid_Crystal_Display.print (String ((5000 - End_of_the_Equation_Answer_Entrance) / 1000) + " s");
	if (((End_of_the_Equation_Answer_Entrance) > 5000) && !Correct_Answer_Is_Entered)
	{
		Time_is_Up = true;
	}
}

void Add_a_Time_Taken_To_Solve_an_Equation (long Time_Taken_To_Solve_an_Equation)
{
	if (Amount_of_Correctly_Solved_Equations == 1)
	{
		Times_Taken_to_Solve_an_Equation == new long (Amount_of_Correctly_Solved_Equations);
		Times_Taken_to_Solve_an_Equation [0] = Time_Taken_To_Solve_an_Equation;
	}
	else
	{
		int Index;
		long *Copy_of_the_Times_Taken_to_Solve_an_Equation_Array = new long (Amount_of_Correctly_Solved_Equations - 1);
		for (Index = 0; Index < Amount_of_Correctly_Solved_Equations - 1; Index = Index + 1)
		{
			Copy_of_the_Times_Taken_to_Solve_an_Equation_Array [Index] = Times_Taken_to_Solve_an_Equation [Index];
		}
		delete [] Times_Taken_to_Solve_an_Equation;
		Times_Taken_to_Solve_an_Equation = new long (Amount_of_Correctly_Solved_Equations);
		for (Index = 0; Index < Amount_of_Correctly_Solved_Equations - 1; Index = Index + 1)
		{
			Times_Taken_to_Solve_an_Equation [Index] = Copy_of_the_Times_Taken_to_Solve_an_Equation_Array [Index];
		}
		Times_Taken_to_Solve_an_Equation [Amount_of_Correctly_Solved_Equations - 1] = Time_Taken_To_Solve_an_Equation;
	}
}
*/

// Important:
// https://www.instructables.com/id/Building-a-touch-sensor-low-cost/
// https://playground.arduino.cc/Main/CapacitiveSensor/
// https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/

#include <CapacitiveSensor.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

CapacitiveSensor Capacitive_Sensors [] = 
{
	CapacitiveSensor (49, 48), 
	CapacitiveSensor (47, 46), 
	CapacitiveSensor (45, 44), 
	CapacitiveSensor (43, 42), 
	CapacitiveSensor (41, 40), 
	CapacitiveSensor (39, 38), 
	CapacitiveSensor (37, 36), 
	CapacitiveSensor (35, 34), 
	CapacitiveSensor (33, 32), 
	CapacitiveSensor (31, 30)
};

LiquidCrystal_I2C Liquid_Crystal_Display (0x27, 16, 2);

short Amount_of_Correctly_Solved_Equations = 0, Number_1 = random (1, 10), Number_2 = random (1, 10), Sum = Number_1 + Number_2;
String Sum_String = "", Times_Taken_to_Solve_an_Equation_String = "";
long *Times_Taken_to_Solve_an_Equation;

void setup () 
{
	short Input_Pin, Output_Pin;
	for (Output_Pin = 2; Output_Pin <= 24; Output_Pin = Output_Pin + 1)
	{
		pinMode (Output_Pin, OUTPUT);
	}
	for (Input_Pin = 50; Input_Pin <= 53; Input_Pin = Input_Pin + 1)
	{
		pinMode (Input_Pin, INPUT);
	}
	Liquid_Crystal_Display.init ();
	Liquid_Crystal_Display.backlight ();
	Liquid_Crystal_Display.setCursor (0, 0);
	Liquid_Crystal_Display.print (String (Number_1) + " + " + String (Number_2) + " = ");
	Liquid_Crystal_Display.setCursor (0, 1);
	Liquid_Crystal_Display.print ("Time left: ");
	Serial.begin (9600);
}

void loop () 
{
	const int Thresholds [] = {100, 1000, 100, 100, 100, 100, 1000, 100, 50, 250};
	int Touch_Sensors_1 [4];
	long Touch_Sensors_2 [10];
	short Index, LED_Pins [] = {10, 5, 7, 4, 6, 3, 24, 22, 2, 9, 8, 11, 13, 12};
	for (Index = 0; Index < 4; Index = Index + 1)
	{
		Touch_Sensors_1 [Index] = digitalRead (50 + Index);
	}
	for (Index = 0; Index < 10; Index = Index + 1)
	{
		Touch_Sensors_2 [Index] = Capacitive_Sensors [Index].capacitiveSensor (100);
	}
	for (Index = 0; Index < 4; Index = Index + 1) // LEDs: 10, 5, 7, 4
	{
		if (Touch_Sensors_1 [Index] == HIGH)
		{
			analogWrite (LED_Pins [Index], 255);
		}
		else if (Touch_Sensors_1 [Index] == LOW)
		{
			analogWrite (LED_Pins [Index], 0);
		}
	}
	for (Index = 0; Index < 2; Index = Index + 1) // LEDs: 6, 3
	{
		if (Touch_Sensors_2 [Index] >= Thresholds [Index])
		{
			analogWrite (LED_Pins [Index + 4], 255);
		}
		else if (Touch_Sensors_2 [Index] < Thresholds [Index])
		{
			analogWrite (LED_Pins [Index + 4], 0);
		}
	}
	for (Index = 0; Index < 2; Index = Index + 1) // LEDs: 24, 22
	{
		if (Touch_Sensors_2 [Index + 2] >= Thresholds [Index + 2])
		{
			digitalWrite (LED_Pins [Index + 6], HIGH);
		}
		else if (Touch_Sensors_2 [Index + 2] < Thresholds [Index + 2])
		{
			digitalWrite (LED_Pins [Index + 6], LOW);
		}
	}
	for (Index = 0; Index < 6; Index = Index + 1) // LEDs: 2, 9, 8, 11, 13, 12
	{
		if (Touch_Sensors_2 [Index + 4] >= Thresholds [Index + 4])
		{
			analogWrite (LED_Pins [Index + 8], 255);
		}
		else if (Touch_Sensors_2 [Index + 4] < Thresholds [Index + 4])
		{
			analogWrite (LED_Pins [Index + 8], 0);
		}
	}
}
