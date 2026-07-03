// Handling the blink levels of Status LED 

const int ledPin =  13;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long SlowBlinkpreviousMillis = 0;        // will store last time LED was updated
unsigned long FastBlinkpreviousMillis = 0;

// constants won't change:
const long blinkinterval = 1000;           // interval at which to blink (milliseconds)
const long Fastblinkinterval = 100;           // interval at which to blink (milliseconds)

void Blinksetup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
}
  
void SlowBlink() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
unsigned long currentMillis = millis();
    if (currentMillis - SlowBlinkpreviousMillis >= blinkinterval) {
    // save the last time you blinked the LED
    SlowBlinkpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      
      ledState = HIGH;
    } else {
      
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
 }
}

void FastBlink() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
unsigned long currentMillis = millis();
    if (currentMillis - FastBlinkpreviousMillis >= Fastblinkinterval) {
    // save the last time you blinked the LED
    FastBlinkpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      
      ledState = HIGH;
    } else {
      
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
 }
}
