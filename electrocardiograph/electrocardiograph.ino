// This program reads the incoming signal from the electrocardiograph and outputs the heart rate

// Pin D7 is connected to the signal input
// Pin D6 is connected to a mini-speaker
// For the connection to the liquid crystal display, see the LiquidCrystal Librairy

// External variables
const int  signal = 7;    // Pin connected to the filtered signal from the circuit
unsigned long time;   
unsigned long frequency;

// Internal variables
int period = 2000;
int starttime = 2000;
int input = 0;
int lastinput = 0;
unsigned long death = 0;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup() {
  pinMode(signal, INPUT);
  lcd.createChar(1, heart);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.print("    bpm :       ");
  lcd.setCursor(0,1);
  lcd.print("You are         ");
}

void loop() {
  
  lcd.setCursor(1,0);
  time = millis();
  input = digitalRead(signal);

  if ((input != lastinput)&&(input == HIGH)) {
    // If the pin state has just changed from low to high (edge detector)
     period = time - starttime; // Compute the time between the previous beat and the one that has just been detected
     starttime = time; // Define the new time reference for the next period computing
     death = time;
     tone(6,800,100); // Output a short "bip" through the speaker
  }
  lastinput = input; // Save the current pin state for comparison at the next loop iteration

  if (period < 0) {
    frequency = 0;
  }
  else {
    frequency = 60000/period; // Compute the heart rate in beats per minute (bpm) with the period in milliseconds
    Serial.println(frequency);
  }
  
  
  if ((time - death) > 2000) { // Detect if there is no beat after more than 2 seconds
    tone(6,900); // Output a continuous tone to the speaker
    lcd.print("<30");
    lcd.setCursor(8,1);
    lcd.print("dead!   ");
  }
  
  else {
    char freq[3];
    
    if (frequency/100 == 0) {
      freq[0] = 32; // Print a space to the first character if the frequency is below 100 bpm
    } 
    else {
      freq[0] = frequency/100+48; // Sort the hundreds character and convert it in ASCII
    }
    freq[1] = (frequency/10)%10+48; // Sort the thents character and convert it in ASCII
    freq[2] = frequency%10+48; // Sort the units character and convert it in ASCII
    
    lcd.print(freq);
    lcd.setCursor(8,1);
    lcd.print("alive! ");
  }
    
}
