#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>


// Gamma table generated from gamma/gamma.py
const unsigned int PROGMEM gamma_table[255] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 31, 31, 32, 33, 33, 34, 35, 36, 37, 37, 38, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 74, 75, 76, 78, 79, 80, 81, 83, 84, 86, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 102, 103, 105, 107, 108, 110, 111, 113, 115, 116, 118, 120, 122, 123, 125, 127, 129, 131, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 171, 173, 175, 177, 179, 182, 184, 186, 188, 191, 193, 195, 198, 200, 203, 205, 208, 210, 213, 215, 218, 220, 223, 225, 228, 230, 233, 236, 238, 241, 244, 247, 249, 252, 255, 
};

const int ledPin = 0;
const int buttonPin = 2;
int buttonState = 0;
int correctedValue = 0;


void setup()  {
  // Enable Fast PWM - https://www.re-innovation.co.uk/docs/fast-pwm-on-attiny85/
  TCCR0A = 2<<COM0A0 | 2<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 | 1<<CS00;

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
} 

void loop()  {
  showPwm(0);

  // Increment brightness if sensor active (user's hand is near) and continue increasing if sensor remains active
  waitButtonPress();
  for (int fadeValue = 1 ; fadeValue <= 85; fadeValue += 1) {
    showPwm(fadeValue);
    _delay_ms(4);
  }
  _delay_ms(500);

  waitButtonPress();
  for (int fadeValue = 86 ; fadeValue <= 170; fadeValue += 1) {
    showPwm(fadeValue);
    _delay_ms(4);
  }
  _delay_ms(500);

  waitButtonPress();
  for (int fadeValue = 171 ; fadeValue <= 254; fadeValue += 1) {
    showPwm(fadeValue);
    _delay_ms(4);
  }
  _delay_ms(500);

  // Don't turn the light off until the sensor has deactivated (user removed their hand)
  waitButtonRelease();

  waitButtonPress();
  for (int fadeValue = 253 ; fadeValue >= 1; fadeValue -= 1) {
    showPwm(fadeValue);
    _delay_ms(2);
  }
  _delay_ms(500);

  // Wait for sensor deactivating again before switching light back on
  waitButtonRelease();
}

bool waitButtonPress() {
  while (true) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      _delay_ms(100);
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW) {
        return true;
      }
    }
  }
}

bool waitButtonRelease() {
  while (true) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      _delay_ms(100);
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        return true;
      }
    }
  }
}

void showPwm(int val) {
  correctedValue = pgm_read_byte(&gamma_table[val]);
  analogWrite(ledPin, correctedValue);
}

