#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Using version 1.2.1
#include "Menu.h"
#include "Custom_Keypad.h"
#include "Common.h"
#include "Stuffer.h"
#include <EEPROM.h>
#include "Memory_Drivers.h"

int Kebap_Addr = 0;
int Kebap_Preset = 0;

int Kiufte_Addr = 4;
int Kiufte_Preset = 0;

int Nadenica_Addr = 8;
int Nadenica_Preset = 0;

int Salam_Addr = 12;
int Salam_Preset = 0;

int Lukanka_Addr = 16;
int Lukanka_Preset = 0;

int Sudjuk_Addr = 20;
int Sudjuk_Preset = 0;

int Sv_Sudjuk_Addr = 24;
int Sv_Sudjuk_Preset = 0;

unsigned long g_Start_Time = 0;
unsigned long g_Stop_Time = 0;
//Beginning of Auto generated function prototypes by Atmel Studio


//End of Auto generated function prototypes by Atmel Studio
#define Pedal_Relay_Rev_Pin   9
#define Pedal_Relay_Pin      10
#define Motor_Relay_FW_Pin      11
#define Motor_Relay_REV_Pin      12
// The LCD constructor - address shown is 0x27 - may or may not be correct for yours
// Also based on YWRobot LCM1602 IIC V1
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char g_Keyboard_Button;
t_System_State g_System_State;
uint8_t g_Menu_State;
bool g_Start_Timer_Running = false;
bool g_One_Time_Learn = false;
bool g_Rev_Mode = false;

void setup()
{

  pinMode(Pedal_Relay_Pin, INPUT);           // set Pedal_Relay pin to input
  digitalWrite(Pedal_Relay_Pin, HIGH);        // Internal Pullup Enabled+
  
  pinMode(Pedal_Relay_Rev_Pin, INPUT);           // set Pedal_Reverse_Relay pin to input
  digitalWrite(Pedal_Relay_Rev_Pin, HIGH);        // Internal Pullup Enabled+ 
  
  pinMode(Motor_Relay_FW_Pin, OUTPUT);           // set Motor_Relay_FW_Pin to output
  digitalWrite(Motor_Relay_FW_Pin, HIGH);        // set Motor_Relay_FW_Pin to NC /OFF

  pinMode(Motor_Relay_REV_Pin, OUTPUT);           // set Motor_Relay_REV_Pin to output
  digitalWrite(Motor_Relay_REV_Pin, HIGH);        // set Motor_Relay_REV_Pin to NC /OFF

  lcd.begin(20, 4); // sixteen characters across - 2 lines
  lcd.backlight();
  // first character - 1st line
  lcd.setCursor(0, 0);
  lcd.print("   Stuffer  V1.00   ");
  // 8th character - 2nd line
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
  delay(2000);
  Serial.begin(115200);  
  
  Retreave_Presets();
  g_System_State = Config_Menu;
}


void loop()
{
  // Start the main menu exec
  System_Exec();
  if(!digitalRead(Pedal_Relay_Rev_Pin)){
    while (!digitalRead(Pedal_Relay_Rev_Pin)){
      digitalWrite(Motor_Relay_REV_Pin, LOW);        // set Motor_Relay_FW_Pin to NC /OFF
    }
    digitalWrite(Motor_Relay_REV_Pin, HIGH);        // set Motor_Relay_FW_Pin to NC /OFF
  }
}

void Menu_Selection(uint8_t l_Keyboard_Button, t_System_State one, t_System_State two, t_System_State three, t_System_State four, t_System_State five, t_System_State six,
                    t_System_State seven, t_System_State eight, t_System_State nine, t_System_State zero, t_System_State star, t_System_State pound )
{

  switch (l_Keyboard_Button)
  {
    case '1':
      g_System_State = one;
      g_Keyboard_Button = 0x00;
      break;
    case '2':
      g_System_State = two;
      g_Keyboard_Button = 0x00;
      break;
    case '3':
      g_System_State = three;
      g_Keyboard_Button = 0x00;
      break;
    case '4':
      g_System_State = four;
      g_Keyboard_Button = 0x00;
      break;
    case '5':
      g_System_State = five;
      g_Keyboard_Button = 0x00;
      break;
    case '6':
      g_System_State = six;
      g_Keyboard_Button = 0x00;
      break;
    case '7':
      g_System_State = seven;
      g_Keyboard_Button = 0x00;
      break;
    case '8':
      g_System_State = eight;
      g_Keyboard_Button = 0x00;
      break;
    case '9':
      g_System_State = nine;
      g_Keyboard_Button = 0x00;
      break;
    case '0':
      g_System_State = zero;
      g_Keyboard_Button = 0x00;
      break;
    case '*':
      g_System_State = star;
      g_Keyboard_Button = 0x00;
      break;
    case '#':
      g_System_State = pound;
      g_Keyboard_Button = 0x00;
      break;
    case 0x00:
    default:
      break;
  }
}

void System_Exec(void)
{
  // Select between different modes or presets
  switch (g_System_State) {
    // Config LCD menu description
    case Config_Menu:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    System Menu    ");
      lcd.setCursor(0, 1);
      lcd.print("  1.Presets Mode   ");
      lcd.setCursor(0, 2);
      lcd.print("  2.Learning Mode  ");
      lcd.setCursor(0, 3);
      lcd.print("  3.Free Running   ");
      g_System_State = Menu;
      break;
    // Menu selection in here
    case Menu:
      g_Keyboard_Button = t_Keypad.getKey();
      if (g_Keyboard_Button) {
        Menu_Selection(g_Keyboard_Button, Config_Preset_Mode, Config_Learning_Mode, Config_Freerun_Mode, Menu, Menu, Menu, Menu, Menu, Menu, Menu, Menu, Reset_Default);
         g_Keyboard_Button= 0x00;
      }
      break;
    // Preset Mode
    case Config_Preset_Mode:
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Preset  Mode    ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1.Kebapche  2.Kiufte");
      lcd.setCursor(0, 1);
      lcd.print("3.Nadenica  4.Salam ");
      lcd.setCursor(0, 2);
      lcd.print("5.Lukanka   6.Sudjuk");
      lcd.setCursor(0, 3);
      lcd.print("7.Sudjuk Sv.  *.Back");
      Retreave_Presets();
      g_System_State = Preset_Mode;
      Serial.println("Waiting for Button press");
      break;
    case Preset_Mode:
      // ToDo: Should Presets be stored from Learning Mode?
      g_Keyboard_Button = t_Keypad.getKey();
      if (g_Keyboard_Button) {
        Menu_Selection(g_Keyboard_Button, P_Kebapcheta_Set, P_Kiufteta_Set, P_Nadenica_Set, P_Salam_Set, P_Lukanka_Set, P_Sudjuk_Set, P_Svinski_Sudjuk_Set, Preset_Mode, Preset_Mode, Preset_Mode, Config_Menu, Preset_Mode);
        g_Keyboard_Button= 0x00;
      }
      break;
    case Config_Learning_Mode:
      Serial.println("Config Learning Mode ");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Learning Mode   ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1.Kebapche  2.Kiufte");
      lcd.setCursor(0, 1);
      lcd.print("3.Nadenica  4.Salam ");
      lcd.setCursor(0, 2);
      lcd.print("5.Lukanka   6.Sudjuk");
      lcd.setCursor(0, 3);
      lcd.print("7.Sudjuk Sv.  *.Back");
      g_System_State = Learning_Mode;
      break;
    // Learning mode
    case Learning_Mode:
      Serial.println("Learning Mode ");
       
      g_Keyboard_Button = t_Keypad.getKey();
      if (g_Keyboard_Button) {
        Serial.println(g_Keyboard_Button);
        Menu_Selection(g_Keyboard_Button, L_Kebapcheta, L_Kiufteta, L_Nadenica, L_Salam, L_Lukanka, L_Sudjuk, L_Svinski_Sudjuk, Learning_Mode, Learning_Mode, Learning_Mode, Config_Menu, Learning_Mode);
        g_Keyboard_Button= 0x00;
      }

      break;
    case Config_Freerun_Mode:
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Freeruning Mode   ");
      lcd.print("  Press * to Exit   ");
      g_System_State = Freerun_Mode;
      
      break;
    // Free Running Mode
    case Freerun_Mode:
      while(t_Keypad.getKey() != '*'){
        if(!digitalRead(Pedal_Relay_Pin)){
          digitalWrite(Motor_Relay_FW_Pin, LOW);
          g_Rev_Mode = true;
        }else{
          digitalWrite(Motor_Relay_FW_Pin, HIGH);
            if(g_Rev_Mode){
            Release_Pressure_Reverse();
            g_Rev_Mode = false;
           }
        }
        if(!digitalRead(Pedal_Relay_Rev_Pin)){
          digitalWrite(Motor_Relay_REV_Pin, LOW);
        }else{
          digitalWrite(Motor_Relay_REV_Pin, HIGH);
           }
        }
      g_System_State = Config_Menu;
      break;


    case P_Kebapcheta_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("    Kebapcheta    ");
      delay(1000);
      if(Kebap_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Kebap_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Kebapcheta;
      }
        break;
        
     case P_Kebapcheta: 
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Kebap_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;

      
    case P_Kiufteta_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("     Kiufteta     ");
      delay(1000);
      if(Kiufte_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Kiufte_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Kiufteta;
      }
      break;
      
     case P_Kiufteta: 
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Kiufte_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;

      
    case P_Nadenica_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("     Nadenica     ");
      delay(1000);
      if(Nadenica_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Nadenica_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Nadenica;
      }
      break;
      
    case P_Nadenica: 
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Nadenica_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;

      
    case P_Salam_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("      Salam       ");
      delay(1000);
      if(Salam_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Salam_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Salam;
      }
      break;
      
    case P_Salam: 
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Salam_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;


      
    case  P_Lukanka_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("      Lukanka     ");
      delay(1000);
      if(Lukanka_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Lukanka_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Lukanka;
      }
      break;
      
    case P_Lukanka: 
      
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Lukanka_Preset);
       
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;


      
    case  P_Sudjuk_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("      Sudjuk      ");
      delay(1000);
      if(Sudjuk_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Sudjuk_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Sudjuk;
      }
      break;
      
    case P_Sudjuk: 
      //if pedal pressed call function and pass time to start running
        Preset_Time_Run(Sudjuk_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;


      
    case  P_Svinski_Sudjuk_Set:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print  ("  Svinski Sudjuk  ");
      delay(1000);
      if(Sv_Sudjuk_Preset == 0){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" No preset found  ");
        lcd.setCursor(0,2);
        lcd.print(" Learn Mode First ");
        delay(4000);
        g_System_State = Config_Preset_Mode;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Time: ");
        lcd.print((float)(Sv_Sudjuk_Preset/1000.0));
        lcd.print(" s");
        lcd.setCursor(0,1);
        lcd.print("      Start?      ");
        lcd.setCursor(0,2);
        lcd.print("    Press Pedal   ");
        lcd.setCursor(0,3);
        lcd.print("  or Back Button  ");
        g_System_State = P_Svinski_Sudjuk;
      }
      break;
      
    case P_Svinski_Sudjuk: 
        //if pedal pressed call function and pass time to start running
          Preset_Time_Run(Sv_Sudjuk_Preset);
      if(t_Keypad.getKey()== '*'){  
        g_System_State = Config_Preset_Mode;
      }
      break;
    case L_Kebapcheta:
      Learn();
      break;
    case L_Kiufteta:
      Learn();
      break;
    case L_Nadenica:
      Learn();
      break;
    case L_Salam:
      Learn();
      break;
    case  L_Lukanka:
      Learn();
      break;
    case  L_Sudjuk:
      Learn();
      break;
    case  L_Svinski_Sudjuk:
      Learn();
      break;
    case Reset_Default:
    Reset_To_Default();
    break;

    //Idle mode
    case Idle:
      // Serial.println("Idle");
      // Runs into here when it has nothing to do

      break;
  }

}

void Learn(void) {
   if(!g_One_Time_Learn){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("      Learning      ");
      lcd.setCursor(0, 2);
      lcd.print("  ................  ");
      g_One_Time_Learn = true;
   }
  while (digitalRead(Pedal_Relay_Pin));
    while (!digitalRead(Pedal_Relay_Pin))
    {

      // If pedal is pressed
      if (!g_Start_Timer_Running) {
        //Start the timer and get the start time.
        g_Start_Time = millis();
        digitalWrite(Motor_Relay_FW_Pin, LOW);
        g_Start_Timer_Running = true;
      }
      
    g_Stop_Time = millis();
    digitalWrite(Motor_Relay_FW_Pin, HIGH);
    Release_Pressure_Reverse();
    g_Start_Timer_Running = false;
    Store_Learned_Time(g_System_State, (int)(g_Stop_Time - g_Start_Time));
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Elapsed Time: ");
    lcd.print(float((g_Stop_Time - g_Start_Time)/1000.0));
    lcd.print(" S");
    delay(5000);
    g_One_Time_Learn= false;
    g_System_State = Config_Learning_Mode;
    }
}
//HIGH is OFF LOW is ON
void Release_Pressure_Reverse(void){
/*    uint32_t l_start_time = millis();
    while((millis() - l_start_time) < REVERSE_TIME){
      digitalWrite(Motor_Relay_FW_Pin, HIGH); 
      digitalWrite(Motor_Relay_REV_Pin, LOW);
    }
    digitalWrite(Motor_Relay_FW_Pin, HIGH);
    digitalWrite(Motor_Relay_REV_Pin, HIGH);
    */
}




void Store_Learned_Time(t_System_State item, int val) {
int test = 0;
  switch (item) {
    case L_Kebapcheta:
      
      EEPROM.put( Kebap_Addr,(val));    // Stores the learned time in EEPROM use EEPROM.get to retreive
    //  EEPROM.get(Kebap_Addr, test);
    /*  Serial.println("EEPROM VALUE: ");
      Serial.println(val);
      Serial.println(test);*/
      break;
    case L_Kiufteta:
      EEPROM.put(Kiufte_Addr, val);
      break;
    case L_Nadenica :
      EEPROM.put(Nadenica_Addr, val);
      break;
    case L_Salam:
      EEPROM.put(Salam_Addr, val);
      break;
    case L_Lukanka :
      EEPROM.put(Lukanka_Addr, val);
      break;
    case L_Sudjuk:
      EEPROM.put(Sudjuk_Addr, val);
      break;
    case L_Svinski_Sudjuk:
      EEPROM.put(Sv_Sudjuk_Addr, val);
      break;
  }
}

void Reset_To_Default(void){
      EEPROM.put(Kebap_Addr,0);    // Stores the learned time in EEPROM use EEPROM.get to retreive
      EEPROM.put(Kiufte_Addr, 0);
      EEPROM.put(Nadenica_Addr, 0);
      EEPROM.put(Salam_Addr, 0);
      EEPROM.put(Lukanka_Addr, 0);
      EEPROM.put(Sudjuk_Addr, 0);
      EEPROM.put(Sv_Sudjuk_Addr, 0);
      g_System_State= Config_Menu;
}


void Retreave_Presets(void){
  EEPROM.get(Kebap_Addr, Kebap_Preset);
  Serial.print("Kebap preset: ");
  Serial.println(Kebap_Preset);

                                  
  EEPROM.get(Kiufte_Addr, Kiufte_Preset);
  Serial.print("Kiufte preset: ");
  Serial.println(Kiufte_Preset);

                                  
  EEPROM.get(Nadenica_Addr, Nadenica_Preset);
  Serial.print("Nadenica preset: ");
  Serial.println(Nadenica_Preset);

                                   
  EEPROM.get(Salam_Addr, Salam_Preset);
    Serial.print("Salam preset: ");
  Serial.println(Salam_Preset);

  
  EEPROM.get(Lukanka_Addr, Lukanka_Preset);
    Serial.print("Lukanka preset: ");
  Serial.println(Lukanka_Preset);

  
  EEPROM.get(Sudjuk_Addr, Sudjuk_Preset);
    Serial.print("Sudjuk preset: ");
  Serial.println(Sudjuk_Preset);

  
  EEPROM.get(Sv_Sudjuk_Addr, Sv_Sudjuk_Preset);
    Serial.print("Sv_Sudjuk preset: ");
  Serial.println(Sv_Sudjuk_Preset);
  
}


void Preset_Time_Run(int preset){
  g_Start_Time = millis();
     while(!digitalRead(Pedal_Relay_Pin)){
       
       while (!digitalRead(Pedal_Relay_Pin) && ((millis()- g_Start_Time) < preset ))
       {
          // If pedal is pressed
          digitalWrite(Motor_Relay_FW_Pin, LOW);
       }
      digitalWrite(Motor_Relay_FW_Pin, HIGH);
      //Release_Pressure_Reverse();
      delay(700);
     } 
}


