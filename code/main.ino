#include <LEDMatrixMagic.h>
#include <Snake.h>
#include <SPI.h>

int DIN = 11; // Data In
int CLK = 10; // Clock
int CS = 9;   // Slave Select

LEDMatrixMagic m = LEDMatrixMagic(DIN, CLK, CS, 4);
Game *g;

ISR(TIMER1_COMPA_vect) {
  g->do_step();
}

#define W 2
#define D 3
#define S 4
#define A 5

char get_input() {
  if (!digitalRead(W))
    return 'w';
  if (!digitalRead(A))
    return 'a';
  if (!digitalRead(S))
    return 's';
  if (!digitalRead(D))
    return 'd';
  return ' ';
}

void setup() {
  Serial.begin(9600);

  pinMode(W, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);
  pinMode(S, INPUT_PULLUP);
  pinMode(D, INPUT_PULLUP);

  for (int d = 0; d < m.matrix_count(); d++) {
    m.shutdown(d, false); // bring the Max out of Shutdown Mode and into Normal Operation Mode
    m.set_intensity(d, 1); // set intensity to the lowest level
    m.clear(d); // clear the display
  }
  
  g = Game::get_game(&m);

  // timer1 init 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31249;            // compare match register 16MHz/256/2Hz-1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void loop() {
  g->get_input(get_input());

  // read button input here - for debugging purposes
  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   char c = Serial.read();
  //   if (c != '\n') {
  //     // Serial.println(c);
  //     // g->do_step();
  //     // g->renderASCII();
  //   }
  // }
}  
