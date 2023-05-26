#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//Output Pins
const int PWM = 10;
const int LED = 7;
const int CE = 8;
//Input pins
const int Vsense = A1;
const int Isense = A2;
const int Button1 = 2;
const int Button2 = 3;
const int Button3 = 4;


//pre-sets
const int set_current = 500; //constant current for discharge set to 500mA
const int set_UV = 3.2; //under voltage cut of for cell set at 3.2V


//Global variables
int timer_count = 0;
int pwm_value = 0;
int Cell_Current = 0;
float Cell_Voltage = 0;
char Mode = 0;
int Capacity = 0;
int sec = 0;



void set_interrupt()
{
  cli(); //stop all interrupts
  //Timer2 (interrupt each 8.128 ms)
  // interrupt time = 1/(16Mhz/1024) * 127 =  8.128ms;
  TCCR2A = 0;                 // Reset entire TCCR1A to 0
  TCCR2B = 0;                 // Reset entire TCCR1B to 0
  TCCR2B |= B00000111;        //Set CS20, CS21 and CS22 to 1 so we get prescalar 1024
  TIMSK2 |= B00000100;        //Set OCIE1B to 1 so we enable compare match B
  OCR2B = 127;                //Finally we set compare register B to this value
  sei();                      //Enable back the interrupts
}

void setup() {
  set_interrupt();

  Serial.begin(9600);
  Serial.print("Battery Capacity Tester");
  pinMode(LED, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode (PWM, OUTPUT);

  pinMode (Button1, INPUT);
  pinMode (Button2, INPUT);
  pinMode (Button3, INPUT);

  digitalWrite (CE, LOW); //Turn off charging
  analogWrite(PWM, 0); //Turn off discharge

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  // Display static text
  display.println("Cell Capacity Tester");
  display.setCursor(25, 40);
  display.println("Circuit Digest");
  display.display();

  delay(1000);
}

void home_screen()
{
  display.clearDisplay();
  display.setCursor(20, 5); display.println("Capacity Tester");
  display.setCursor(5, 20); display.println("1. Charge Cell");
  display.setCursor(5, 30); display.println("2. Test Capacity");
  display.setCursor(5, 40); display.println("3. Cell Voltage");
  display.setCursor(15, 55); display.println("  Select Menu");
  display.display();
  delay (500);
  display.setCursor(15, 55); display.println("->Select Menu");
  display.display();
}

void Charge_cell()
{
  digitalWrite (CE, HIGH); //Turn on charging

  display.clearDisplay();
  display.setCursor(25, 10); display.println("---CHARGING---");
  display.setCursor(5, 40); display.println("Cell Voltage:     V");
  display.setCursor(90, 40); display.println(Cell_Voltage);
  display.display();
  delay (100);
  display.setCursor(25, 10); display.println(">--CHARGING--<"); display.display();
  delay (100);
  display.setCursor(25, 10); display.println(">>-CHARGING-<<"); display.display();
  delay (100);
  display.setCursor(25, 10); display.println(">>>CHARGING<<<"); display.display();
}

void Test_Capacity()
{
  digitalWrite (CE, LOW); //Turn off charging


  //Calculate capacity => Capacity (mAh) = (Discharge Current (mA) x Discharge Time (seconds)) / 3,600
  Capacity = (Cell_Current * sec) / 3600;
  Serial.println (Capacity);



  display.clearDisplay();
  display.setCursor(20, 10); display.println("   DISCHARGING   ");
  display.setCursor(5, 30); display.println("|V|:    V");
  display.setCursor(26, 30); display.println(Cell_Voltage);
  display.setCursor(70, 30); display.println("|I|:   mA");
  display.setCursor(95, 30); display.println(Cell_Current);
  display.setCursor(20, 50); display.println("CAPACITY =    mAh");
  display.setCursor(85, 50); display.println(Capacity);
  display.display();
  delay (100);
  display.setCursor(20, 10); display.println("  <DISCHARGING>"); display.display();
  delay (100);
  display.setCursor(20, 10); display.println(" <<DISCHARGING>> "); display.display();
  delay (100);
  display.setCursor(20, 10); display.println("<<<DISCHARGING>>>"); display.display();


  if (Cell_Voltage < set_UV) //If cell voltage is low stop discharging
  {
    pwm_value = 0;
    display.clearDisplay();
    display.setCursor(20, 10); display.println("COMPLETED");
    display.setCursor(20, 50); display.println("CAPACITY =    mAh");
    display.setCursor(85, 50); display.println(Capacity);
    display.display();
    while (1);
  }
  else //else adjust the pwm signal to get desired set current
  {
    //=>Regulate Cell Current using PWM
    if (Cell_Current < (set_current - 10)) //10mA tollerance
      pwm_value = pwm_value + 1;
    if (Cell_Current > (set_current + 10)) //10mA tollerance
      pwm_value = pwm_value - 1;
    if (pwm_value > 255)
      pwm_value = 255;
  }

  analogWrite(PWM, pwm_value);
}


void Cell_voltage()
{
  display.clearDisplay();
  digitalWrite (CE, LOW); //Turn off charging
  analogWrite(PWM, 0); //Turn off discharge
  display.setCursor(5, 40); display.println("Cell Voltage:     V");
  display.setCursor(90, 40); display.println(Cell_Voltage);
  display.display();
}




void loop() {

  //=>Read Cell Voltage and current
  Cell_Voltage = (analogRead(Vsense) * 1.111) / 100; //1.11 is the multipleir values based on our voltage divider used
  delay (10);
  Cell_Current = analogRead(Isense); //no multiplier value needed
  delay (10);


  if (Mode == 0) {
    home_screen();
  }
  if (digitalRead(Button1) == LOW || Mode == 1) {
    Mode = 1;
    Charge_cell();
  }
  if (digitalRead(Button2) == LOW || Mode == 2) {
    Mode = 2;
    Test_Capacity();
  }
  else {
    sec = 0; //do not start timer in other modes
  }
  if (digitalRead(Button3) == LOW || Mode == 3) {
    Mode = 3;
    Cell_voltage();
  }

}


ISR(TIMER2_COMPB_vect) {
  timer_count = timer_count + 1;
  if (timer_count >= 606) //for every 10sec
  {
    timer_count = 0;
    sec = sec + 10; // every 10 sec increment the seconds counter
  }
}
