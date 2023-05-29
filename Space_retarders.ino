/* Klon igre Space Invaders, dovoljno pojednostavljen da bude "igriv" u Tinkercadu */

#include <Adafruit_NeoPixel.h>

// Tinkercad radi brze s 3 krace trake nego s jednom dugom.
// Takodjer, ima smisla osvjezavati samo dijelove "ekrana".

Adafruit_NeoPixel strip[3] = {
  Adafruit_NeoPixel(10, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(10, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(10, 5, NEO_GRB + NEO_KHZ800)
};

bool reset = false;
bool firing = false;
int joystick;
int rndCol;		// Stupac u kojem Space invader leti
int row;		// Visina letjelice
int player=1;	// 0 = lijevo, 1 = sredina, 2 = desno
int oldYer=1;	// Prethodna pozicija playera
int clock=100;	// "tick" simulatora, umjesto millis()
int cycle;		// Broj proteklih clockova

void setup() {
  attachInterrupt(0, fire, FALLING);
  pinMode(A0, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  strip[0].begin();
  strip[1].begin(); 
  strip[2].begin();
  intro();
}

void loop() {
  rndCol = random(3);	
  row = 0;
  cycle = 0;
  reset = false;

  while(!reset) {

    // Pozicioniranje playera
    joystick = analogRead(A0);
    if (joystick < 700)
      player = 0;
    else if (joystick > 860)
      player = 2;
    else
      player = 1;

     // Ako se igrac pomaknuo, iscrtava "ekran"
    if (player != oldYer) {
      strip[rndCol].setPixelColor(row,255,0,0);
      refreshScreen();      
    }

    // Pamti prethodnu poziciju playera
    oldYer = player;

    // Svaki nulti ciklus iscrtava novi polozaj
    if (cycle == 0) {
      strip[rndCol].setPixelColor(row,255,0,0);
      strip[rndCol].show();
    }

    // Svaki n-ti ciklus pomice invadera
    if(cycle >= 5) {
      cycle = -1;
      strip[rndCol].setPixelColor(row,0,0,0);
      strip[rndCol].show();
      tone(6,60,10);
      row++;
    }

    // Dno ekrana...
    if(row > 8) {
      row = -2;
      rndCol = random(3);
    }

    // Inicira iduci ciklus
    delay(clock);
    cycle++;
  }
}

void refreshScreen() {
  strip[0].setPixelColor(9,0,0,0);
  strip[1].setPixelColor(9,0,0,0);
  strip[2].setPixelColor(9,0,0,0);
  strip[player].setPixelColor(9,0,0,255);
  strip[0].show();
  strip[1].show();
  strip[2].show();
}

void fire() {
  // Izbjegava interrupt na interrupt
  if (firing == false){
  	firing = true;
    lameShootingFX();

    for (int i=8; i>-2; i--) {
      if(i!=8)
        strip[player].setPixelColor(i+1,0,0,0);
      strip[player].setPixelColor(i,255,255,0);       
      strip[player].show();
      delay(clock/2);
      if (player == rndCol) {	// Pogodak
        reset=true;
        if (i==row+1) {
          strip[player].setPixelColor(i-1,255,0,255);
          strip[player].setPixelColor(i,0,0,0);
          strip[player].show();
          killAudioFX();
          strip[player].setPixelColor(i-1,0,0,0);
          strip[player].show();
          break;
        }
      }
    }
  }
  firing = false;
}

void lameShootingFX() {
  for (int i=0; i<100; i++) {
    digitalWrite(6, HIGH);
    delayMicroseconds(i*10);
    digitalWrite(6, LOW);
    delayMicroseconds(i*10);
  }
}

void killAudioFX(){
  for (int i=80; i<90; i--) {
    digitalWrite(6, HIGH);
    delayMicroseconds(i*1110);
    digitalWrite(6, LOW);
    delayMicroseconds(i*20);
    if (i==10)
      break;
  }
}

void intro() {
  float t = 1.2;
  float q = 100*t;
  float trip = 150*t;
  float half = 200*t;
  float three = 300*t;
  float six = 600*t;
  float four = 400*t;
  int p = 10*t;

  tone(6,175,three-p); delay(three+p);
  tone(6,233,q-p); delay(q+p);
  tone(6,311,six-p); delay(six+p);
  tone(6,294,half-p); delay(half+p);
  tone(6,233,trip-2*p); delay(trip+2*p);
  tone(6,196,trip-2*p); delay(trip+2*p);
  tone(6,262,q-2*p); delay(q+2*p);
  tone(6,349,four-p); delay(four+p);
}