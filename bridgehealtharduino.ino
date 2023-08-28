#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<Servo.h>

Servo Myservo;
int pos;
int vib_pin=7;
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int newtime;

void isr() //interrupt service routine
{
rev++;
}

//Variables:
//int value; //save analog value
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  lcd.begin();
  pinMode(vib_pin,INPUT);
  pinMode(3,INPUT);
  Myservo.attach(3);
  lcd.clear();
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Safe to Go!");
  lcd.setCursor(1,0);   //Move cursor to character 2 on line 1
  lcd.print("Happy Journey");
  lcd.clear();
  pinMode(A1,INPUT);//declaring input pin flex sensor
  Serial.begin(9600);//this is for serial communication
  //lcd.init (); //initialize LCD
  // Turn on the backlight on LCD. 
  //lcd. backlight ();
  attachInterrupt(digitalPinToInterrupt(2),isr,RISING); //attaching the interrupt
}

void loop()
{
  int val;
  val=digitalRead(vib_pin);
  if (val==1)
  {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Vibration risk");
      lcd.setCursor(1,1);
      lcd.print("STOP");
      delay(2000);
      Myservo.write(90);
      delay(5000);
      Myservo.write(0); 
  }
  else if(val==0)
  {
    lcd.clear();
    lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
    lcd.print("Safe to Go!");
    lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
    lcd.print("Happy Journey");
    Myservo.write(0);
  }

int value = analogRead(A0);//water level pin
Serial.println(value);
if(value>320)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Water level rise");
    Myservo.write(90);  
    delay(3000);
}
else if(value<320)
{
  lcd.clear();
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Safe to Go!");
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("Happy Journey");
  Myservo.write(0);
  delay(3000);
}
  
  // 1 RPM = 0.01km/hr
  delay(1000);
  detachInterrupt(0); //detaches the interrupt
  newtime=millis()-oldtime; //finds the time 
  rpm=(rev/newtime)*60000; //calculates rpm
  oldtime=millis(); //saves the current time
  rev=0;
  float y=rpm*0.01;
  attachInterrupt(digitalPinToInterrupt(2),isr,RISING);
  if(y>11)
    {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("High winds");
     lcd.setCursor(2,1);
     lcd.print("STOP");
     delay(3000);
     Myservo.write(90);
     delay(6000);
     
     
    }
   else if(y<11)
      {
        lcd.clear();
        lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
        lcd.print("Safe to Go!");
        lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
        lcd.print("Happy Journey");
        Myservo.write(0);
      }
}
