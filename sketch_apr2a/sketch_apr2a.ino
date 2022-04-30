


int current_dir = 0;

#define debounceDelay 15

typedef struct button {
  int buttonPin;
  long *lastDebounceTime; 
  int *lastButtonState, *buttonState;
} button_t;

void analogDebounceInput(button_t *b) {
  int reading = analogRead(buttonPin);
  int returned = 0;
  
  // If the switch changed, due to noise or pressing:
  if (reading != *lastButtonState) {
    // reset the debouncing timer
    b->lastDebounceTime = millis();
  }

  if ((millis() - b->lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != b->buttonState) {
      b->buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (b->buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  b->lastButtonState = reading;  
}

button_t buttons[4];

void setup() {
  buttons[0].buttonPin = A0;
  buttons[1].buttonPin = A1;
  buttons[2].buttonPin = A4;
  buttons[3].buttonPin = A5;



  Serial.begin(9600);

}

void loop() {
  int l, r, u, d;
  r = analogRead(A0); 
  u = analogRead(A1); 
  d = analogRead(A4); 
  l = analogRead(A5); 

  if (r == 0) {
    current_dir = 0;
    Serial.println("RIGHT");
  } else if (u == 0) {
    Serial.println("UP");
    current_dir = 1;
  } else if (d == 0) {
    Serial.println("DOWN");
    current_dir = 2;
  } else if (l == 0) {
    Serial.println("LEFT");
    current_dir = 3;
  }

  

  delay(500);
}
