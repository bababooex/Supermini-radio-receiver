//This is the first version of the code, so needs some fixes, but should work as is for now


//all libraries (controlling tuner, keypad and display) 
#include "MxL5007T.h"
#include "Keypad.h"
#include "SevSeg.h"
//nametags
SevSeg sevseg;
MxL5007T tuner;
//used these atmega 328p pins in my receiver, change these as you like
byte numDigits = 4;
byte digitPins[] = {2, 17, 15, 13};
byte segmentPins[] = {1, 16, 20, 3, 5, 0, 21, 4};

bool resistorsOnSegments = false;
byte hardwareConfig = COMMON_ANODE;
bool updateWithDelays = false;
bool leadingZeros = false;
bool disableDecPoint = false;

// Keypad setup for 3x4 keypad
const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 10, 11, 12 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Starting frequency, I will change this to first saved frequency
int frec1 = 468;   // integer MHz
int frec2 = 7;     // decimal part (0–9)
float frec = 468.7;

unsigned long holdStart = 0;
unsigned long scanDelay = 300;  // ms
bool inputMode = false;
String newFrec = "";

// setting frequency function
void setFrequency(float freq) {
  // keep user-visible frequency as is
  frec = freq;

  // clamp user-visible value (for display only)
  if (frec < 44.0) frec = 44.0;
  if (frec > 999.9) frec = 999.9;

  frec1 = (int)frec;
  frec2 = (int)((frec - frec1) * 10 + 0.5);

  // if frequency offset correction
  const long IF_expected = 10700000L;   // 10.7 MHz
  const long IF_tuner    = 9191500L;    // 9.1915 MHz 
  const long correction  = IF_expected - IF_tuner; // 1.5085 MHz

  long freqHz = (long)(freq * 1000000.0);
  long tunerFreqHz = freqHz - correction;

  // tuner and lcd limit set
  if (tunerFreqHz < 44000000L) tunerFreqHz = 44000000L;
  if (tunerFreqHz > 999900000L) tunerFreqHz = 999900000L;
  // finally set the tuner frequency and bandwidth (6MHz is enough)
  tuner.setFrequencyAndBw((uint32_t)tunerFreqHz, 6);
}


void showFrequency() {
  // Always show frequency as xxx.x
  int displayVal = frec1 * 10 + frec2;
  sevseg.setNumber(displayVal, 1); // 1 decimal place
}

void showInputProgress() {
  // Display placeholder ___._ filled progressively
  int len = newFrec.length();
  char buf[5] = {' ', ' ', ' ', ' ', '\0'};

  // fill entered digits
  for (int i = 0; i < len && i < 4; i++) {
    buf[i] = newFrec[i];
  }
  // replace blanks with underscore
  for (int i = len; i < 4; i++) {
    buf[i] = '_';
  }

  // Show with decimal point after 3rd digit
  int tempVal = atoi(newFrec.c_str());
  if (len == 0) {
    sevseg.setChars("____");  // start empty
  } else {
    sevseg.setNumber(tempVal, (len >= 4) ? 1 : 0);
  }
}

void enterFrequencyMode() {
  inputMode = true;
  newFrec = "";
  sevseg.setChars("____"); // show underscores
}

void processInputKey(char key) {
  if (key >= '0' && key <= '9') {
    if (newFrec.length() < 4) {
      newFrec += key;
      showInputProgress();
    }
    if (newFrec.length() == 4) {
      // Got full frequency
      int mhz = newFrec.substring(0, 3).toInt();
      int dec = newFrec.substring(3).toInt();
      setFrequency(mhz + dec / 10.0);
      inputMode = false;
    }
  } else if (key == '#') {
    // backspace or cancel
    if (newFrec.length() > 0) {
      newFrec.remove(newFrec.length() - 1);
      showInputProgress();
    } else {
      // cancel input if empty
      inputMode = false;
      showFrequency();
    }
  }
}
// init everything
void setup() {
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  tuner.init(MxL_XTAL_24_MHz);

  setFrequency(frec);
}

// Timing
unsigned long lastStep = 0;
void loop() {
  sevseg.refreshDisplay();

  // Always read analog buttons, because I dont have enough pins, so I use ADC
  int upVal = analogRead(A7);
  int downVal = analogRead(A6);
  bool upPressed   = (upVal < 400);
  bool downPressed = (downVal < 400);

  // Show display 
  if (!inputMode) {
    showFrequency();
  } else {
    showInputProgress();
  }

  // Handle keypad 
  char customKey = customKeypad.getKey();
  if (customKey) {
    if (inputMode) {
      processInputKey(customKey);
    } else {
      if (customKey == '*') {
        enterFrequencyMode();
      }
    }
  }

  // button handler
  if (!inputMode && (upPressed || downPressed)) {
    if (holdStart == 0) { 
      holdStart = millis();
      lastStep = 0; 
    }

    unsigned long now = millis();
    if (now - lastStep >= scanDelay) {
      lastStep = now;

      if (upPressed) {
        frec2++;
        if (frec2 > 9) { frec1++; frec2 = 0; }
        setFrequency(frec1 + frec2 / 10.0);
      }
      if (downPressed) {
        frec2--;
        if (frec2 < 0) { frec1--; frec2 = 9; }
        setFrequency(frec1 + frec2 / 10.0);
      }
    }
  } else {
    // No button pressed
    holdStart = 0;
    scanDelay = 300;
  }

  // Speed frequency change
  if (holdStart > 0) {
    unsigned long held = millis() - holdStart;
    int accelDelay = 300 - held / 10;
    if (accelDelay < 50) accelDelay = 50;
    scanDelay = accelDelay;
  }
}


