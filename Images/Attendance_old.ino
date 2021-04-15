#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <SPI.h>


#define RS 2 // LCD RS connected on pin 2.
#define RW 3 // LCD RW to be pulled LOW.
#define EN 4 // LCD EN connected on pin 4.
#define D4 5 // LCD D4 connected on pin 5.
#define D5 6 // LCD D5 connected on pin 6.
#define D6 7 // LCD D6 connected on pin 7.
#define D7 8 // LCD D7 connected on pin 8.
LiquidCrystal LCD(RS, EN, D4, D5, D6, D7);

#define SS_PIN 10 //SS(SDA) connected on pin 10.
#define RST_PIN A0 //RST connected on pin A0.
MFRC522 RFID(SS_PIN, RST_PIN);

#define redLED A1 //Red LED connected on pin A1.
#define blueLED A2 //Blue LED connected on pin A2.
#define whiteLED A3 //White LED connected on pin A3.
#define Buzzer 9 //Buzzer connected on pin 9.
#define eraseMaster A4 //Push-button connected to pull-up resistance on A4.
#define eraseALL  A5 //Push-button connected to pull-up resistance on A5.
#define MasterFlag 143 // Our flag in the EEPROM to know whether a Master is stored or not.
#define CheckerFlag 4 // Our flag in the EEPROM to know whether a Checker is stored or not.

unsigned char AdminFlag = 4; // Variable to help in exiting ProgramMode only after Master tag is swiped.
unsigned char Scanned[4]; // Variable to store ID of the tag.
unsigned char Attendance = 0; // Variable to store the attendance.
unsigned char NumberOfStudent = 0; // Variable holds information about current student (to later on find/delete him/her easily).
unsigned int Mode; // Variable stores the return value of CheckTag function (0,1,2,3);
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * ************************* Program Start  *********************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */
 
void setup() {
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * ******************* First Stage: Initialization  *************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */
  ModulesInit(); // Initialize all objects, modules, and pins.
  TurnLED_Buzzer_off(); // Make sure everything is OFF.
  delay(200); // For LCD start-up.
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * ********************** Second Stage: Removal *****************************
 * *************************** OPTIONAL *************************************
 * **************************************************************************
 * **************************************************************************
 */
  LCD.print("Hold MASTER for");
  LCD.setCursor(0,1);
  LCD.print("4s to erase Master");
  delay(5000); // Give time to the user to read the sentence.
  if(Master_ButtonIsPressed())
  {
    delay(20); // For button debounce.
    while(Master_ButtonIsPressed()); // Do nothing as long as the user holds the button.
    EraseMaster();
    delay(20); // For button debounce.
  }

  LCD.clear();
  LCD.print("Hold ALL for 4s");
  LCD.setCursor(0,1);
  LCD.print("to erase EEPROM");
  delay(5000); // Give time to the user to read the sentence.
  if(All_ButtonIsPressed())
  {
    delay(20); // For button debounce.
    while(All_ButtonIsPressed()); // Do nothing as long as the user holds the button.
    EraseAllEEPROM(); 
    delay(20); // For button debounce.
  }
  
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * ******************* Third Stage: Checking for Master *********************
 * *******************************  and Checker Tags  ************************
 * **************************************************************************
 * **************************************************************************
 */
  LCD.clear();
  if(!MasterIsHere())
  ScanMaster();

  if(!CheckerIsHere())
  ScanChecker();
    
  EmptyAttendance(); //To Count from Zero.
  LCD.clear();
  LCD.print("Program Start");
  delay(1000);
    /*
     * To Print Number of Registered Students.
     * (Call when testing the program ONLY)
     * PrintNumberOfStudents();
     */
}
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * *********************** END OF VOID SETUP  *******************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */
void loop() {
  LCD.clear();
  LCD.print("Swipe Tag");
  while(!TagPassed()); // Do Nothing.
  ScanTag();
  Mode = CheckTag(); // Function returns (0,1,2,3).
  /*
   * Use when you want to print the return value (0,1,2,3) to test the program ONLY.
   * 
   * LCD.setCursor(15,1);
   * LCD.print(Mode);
   * delay(500); 
   */
  switch(Mode){
    /*   Admin Mode   */ case 0: do{ ProgramMode(); } while(AdminFlag == 4); break; // To exit ProgramMode when only Master is swiped.
    /*     Checker     */ case 1: EndProgram(); break;
    /*     Student    */ case 2: Welcome(); break;
    /*     Stranger   */ case 3: Denied(); break;
  }
  delay(800); // To give time for the sentences to be read by user.
  TurnLED_Buzzer_off(); // Turn all LEDs and Buzzer back off.
}
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * *********************** END OF VOID LOOP  *******************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */
/************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * *********************** Functions Declarations ***************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */
void ModulesInit(){
  pinMode(RW, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(eraseMaster, INPUT_PULLUP); //Initiate pull-up resistor.
  pinMode(eraseALL, INPUT_PULLUP); //Initiate pull-up resistor.
  digitalWrite(RW, LOW);

  SPI.begin();
  RFID.PCD_Init();
  LCD.begin(16,2);
}

bool All_ButtonIsPressed(){
  bool reading = digitalRead(eraseALL);
  return !reading; 
  /*
   * Because when the button is pressed it reads LOW not HIGH ( Pull-up Resistor ).
   * I reveresed it, so that the conditions remain simple and logical to the programmer.
   */
}

void EraseAllEEPROM(){
  for(int i = 0; i < 1024;i++)
    EEPROM.update(i,0);
  LCD.clear();
  LCD.print("Erasing All");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
   LCD.clear();
   LCD.print("Erasing Done");
   delay(1000);
}

bool Master_ButtonIsPressed(){
  bool reading = digitalRead(eraseMaster);
  return !reading;
  /*
   * Because when the button is pressed it reads LOW not HIGH ( Pull-up Resistor ).
   * I reveresed it, so that the conditions remain simple and logical to the programmer.
   */
}

void EraseMaster(){
  for(int i = 0; i < 5;i++)
    EEPROM.update(i,0);
  LCD.clear();
  LCD.print("Erasing Master");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
   LCD.clear();
   LCD.print("Erasing Done");
   delay(1000);
}

bool TagPassed(){
  if(!RFID.PICC_IsNewCardPresent() || !RFID.PICC_ReadCardSerial())
    return 0;
  return 1;
}
bool MasterIsHere(){
  if(EEPROM.read(0) != MasterFlag) // Therefore no Master.
    return 0;
  return 1;
}
void ScanMaster(){
  LCD.print("No Master Found");
  LCD.setCursor(0,1);
  LCD.print("Scan Master");
  while(!TagPassed()); // Do Nothing.
  for(int i = 1;i < 5;i++)
    EEPROM.write(i, RFID.uid.uidByte[i-1]);
  LCD.clear();
  LCD.print("Scanning");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
  EEPROM.write(0,MasterFlag);
  LCD.clear();
  LCD.print("Master Added");
  delay(2000);
}

bool CheckerIsHere(){
  if(EEPROM.read(9) != CheckerFlag) // Therefore no Checker.
    return 0;
  return 1;
}

void ScanChecker(){
  LCD.clear();
  LCD.print("No Checker Found");
  LCD.setCursor(0,1);
  LCD.print("Scan Checker");
  while(!TagPassed()); // Do Nothing.
  for(int i = 5;i < 9;i++)
    EEPROM.write(i, RFID.uid.uidByte[i-5]);
  LCD.clear();
  LCD.print("Scanning");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }  
  EEPROM.write(9 ,CheckerFlag);
  LCD.clear();
  LCD.print("Checker Added");
  delay(2000);
}

void ScanTag(){
  for(int i = 0;i < 4;i++)
    Scanned[i] = RFID.uid.uidByte[i];
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
}

unsigned int CheckTag(){
  if(TagIsMaster()) return 0;
  else if(TagIsChecker()) return 1;
  else if(TagIsStudent()) return 2;
  else return 3;
}

bool TagIsMaster(){
  for(int i = 1;i < 5;i++)
    if(EEPROM.read(i) != Scanned[i-1]) return 0;
  return 1;
}

bool TagIsChecker(){
  for(int i = 5;i < 9;i++)
    if(EEPROM.read(i) != Scanned[i-5]) return 0;
  return 1;
}

bool TagIsStudent(){
  int i,j;
  for(i = 0;i < EEPROM.read(23);i++) // hna kan feh +1
  {
    int k =0; int checker = 0;
    for(j = 24+5*i;j < (24+5*(i+1))-1;j++)
      {
        if(Scanned[k++] != EEPROM.read(j)) break;
        else checker++;
      }
      if(checker == 4){
        if(EEPROM.read(j) != 1){
          EEPROM.write(j,1);
          Attendance++;
        }
      NumberOfStudent = i;
      return 1;
      }
  }
  return 0;
}
void ProgramMode(){
  LED();
  digitalWrite(Buzzer, LOW);
  LCD.clear();
  LCD.print("Admin Mode");
  LCD.setCursor(0,1);
  LCD.print("Add/Remove Tags");
  while(!TagPassed()); // Do Nothing.
  ScanTag();
  Mode = CheckTag();
  /*
   * LCD.setCursor(15,1);
   * LCD.print(Mode);
   * delay(500);
   */
  switch(Mode){
    /* Admin Mode */ case 0:                  AdminFlag = 5; break;
    /*   Checker   */ case 1: RemoveChecker();  AdminFlag = 4; break;
    /*   Remove   */ case 2: RemoveStudent(); AdminFlag = 4; break;
    /*    Add     */ case 3: AddStudent();    AdminFlag = 4; break;
  }
  delay(1000);
}

void EndProgram(){
  for(int i = 0; i < 5;i++){
  digitalWrite(Buzzer, HIGH);
  delay(300);
  digitalWrite(Buzzer, LOW);
  delay(300);
  }
  LCD.clear();
  LCD.print("Attendance");
  LCD.setCursor(0,1);
  LCD.print(Attendance);
  LCD.print(" Out of ");
  LCD.print(EEPROM.read(23));
  while(1);
}
void Welcome(){
  digitalWrite(whiteLED, HIGH);
  LCD.clear();
  LCD.print("Access Granted");
}

void Denied(){
  digitalWrite(redLED, HIGH);
  digitalWrite(Buzzer, HIGH);
  LCD.clear();
  LCD.print("Access Denied");
}

void RemoveChecker(){
  LCD.clear();
  LCD.print("Removing");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
  LCD.clear();
  LCD.print("Removal Success");
  LCD.setCursor(0,1);
  LCD.print("Scan New Checker");
  while(!TagPassed());
  for(int i = 5;i < 9;i++)
     EEPROM.update(i, RFID.uid.uidByte[i-5]);
  LCD.clear();
  LCD.print("Scanning");
  LCD.setCursor(0,1);
  for(int i = 0;i < 16;i++)
   {
      LCD.print('.');
      delay(50);
   }
   LCD.clear();
  LCD.print("New Checker Added");
  digitalWrite(Buzzer, HIGH);
}

void RemoveStudent(){
  if(EEPROM.read(23) == 1)
  {
    if(EEPROM.read(28) == 1) Attendance--;
    for(int i = 24;i < 29;i++)
      EEPROM.write(i,0);
  }
  else{
    if(EEPROM.read(28 + 5 * NumberOfStudent) == 1) Attendance--;
    for(int i = 24 + 5 * NumberOfStudent + 5;i < 24 + 5 * EEPROM.read(23);i++)
      EEPROM.write(i-5, EEPROM.read(i));
  }
  EEPROM.write(23, EEPROM.read(23)-1);
    LCD.clear();
    LCD.print("Student Removed");
    digitalWrite(Buzzer, HIGH);
}

void AddStudent(){
  int k = 0;
  for(int i = 24 + 5 * EEPROM.read(23);i < (24 + 5 *(EEPROM.read(23)+1)) - 1;i++)
    EEPROM.write(i, Scanned[k++]);
  EEPROM.write(23, EEPROM.read(23)+1);
  LCD.clear();
  LCD.print("Student Added");
  digitalWrite(Buzzer, HIGH);
}

void PrintNumberOfStudents(){
  LCD.clear();
  LCD.print(EEPROM.read(23));
  delay(2000);
}

void EmptyAttendance(){
  for(int i = 0;i < EEPROM.read(23);i++){
    EEPROM.update(28 + 5 * i, 0);
  }
}

void LED(){
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, HIGH);
  digitalWrite(whiteLED, LOW);
}

void TurnLED_Buzzer_off(){
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(whiteLED, LOW);
  digitalWrite(Buzzer, LOW);
}
/*************************************************************************************************************/
/*
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 * ************************* Commented Functions ****************************
 * **************************************************************************
 * **************************************************************************
 * **************************************************************************
 */

 void updateChecker(){
  EEPROM.update(5,49);
  EEPROM.update(6,194);
  EEPROM.update(7,41);
  EEPROM.update(8,46);
 }
