   #include <LiquidCrystal.h>
   #include <Motor.h>
   #include <SoftwareSerial.h>
   #include <stdio.h>

   const int TX = 6;
   const int RX = 7;
   
   const int LT_Y = 3;
   const int RT_Y = 1;
   
   const int LED_PIN = 13;
   const int BT_POWER_PIN = 10;

   const int CMD_INTERVAL = 20;
   const int BOOT_DELAY = 500;
   const int SWITCH_PIN = 9;
   
   const char buffer[100];
   const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
   

   Motor motorRT(MOTOR_A,CMD_INTERVAL);
   Motor motorLT(MOTOR_B,CMD_INTERVAL);
   SoftwareSerial bluetooth(RX, TX);
   LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
   
   void powerCycleBluetooth()
   {
    digitalWrite(BT_POWER_PIN,LOW);   // Power down BT module
    delay(100);
    digitalWrite(BT_POWER_PIN,HIGH);
    delay(BOOT_DELAY);
    bluetooth.begin(9600);
    delay(100);
   }
   
   bool switchOff()
   {
    return digitalRead(SWITCH_PIN) == LOW;
   }

   void setup() 
   {
    
    lcd.begin(16, 2);
    pinMode(BT_POWER_PIN, OUTPUT); 
    pinMode(SWITCH_PIN,   INPUT);
    motorRT.init(bluetooth);
    motorLT.init(bluetooth);
    powerCycleBluetooth();
    
   }  
   void loop() 
   {
    
    int8_t speedRT = motorRT.analogToSpeed( analogRead(RT_Y) );
    int8_t speedLT = motorLT.analogToSpeed( analogRead(LT_Y) );
    
    motorRT.move( speedRT );
    motorLT.move( speedLT );
    
    if(switchOff()){
      
        sprintf(buffer, "L:%4d%% R:%4d%%" ,speedLT, speedRT);
        lcd.setCursor(0,0);
        lcd.print(buffer);
      
        sprintf(buffer, "Waiting...   ");
        lcd.setCursor(0,1);
        lcd.print(buffer);
        
        return;
      }
   
    sprintf(buffer, "L:%4d%% R:%4d%%" ,speedLT, speedRT);
    lcd.setCursor(0,0);
    lcd.print(buffer);

    sprintf(buffer, "Sending...      ");
    lcd.setCursor(0,1);
    lcd.print(buffer);
   }
