const int noisePin = A0;
const int inputPin = A1;
const int maxDobbelPin = A2;

const int d1 = 2; //dsPin 12
const int d2 = 3; //dsPin 9
const int d3 = 4; //dsPin 8!
const int d4 = 5; //dsPin 6
const int a = 6; //dsPin 11
const int b = 7; //dsPin 7!
const int c = 8; //dsPin 4
const int d = 12; //dsPin 2
const int e = 10; //dsPin 1
const int f = 11; //dsPin 10
const int g = 9; //dsPin 5

const byte disp0 = B11111100;
const byte disp1 = B01100000;
const byte disp2 = B11011010;
const byte disp3 = B11110010;
const byte disp4 = B01100110;
const byte disp5 = B10110110;
const byte disp6 = B10111110;
const byte disp7 = B11100000;
const byte disp8 = B11111110;
const byte disp9 = B11110110;
const byte dispD = B01111010;
const byte dispN = B00101010;

const byte dispNull = 0B0000000;

const int dobbelstenen[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000};
const int maxPotentiometerWaarde = 1012;
const float knipperSnelheid = 0.4;

const int dChar = -2;

int lastInputPinState = HIGH;
int getal = 0;
int dobbelsteen = 12;
bool dVoorGetal = true;

void setup() {
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);

  pinMode(d1,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(d3,OUTPUT);
  pinMode(d4,OUTPUT);

  randomSeed(analogRead(noisePin) ^ micros());
}


void loop() {
  if(lastInputPinState == LOW && digitalRead(inputPin) == HIGH) {
    animatie();
  }
  lastInputPinState = digitalRead(inputPin);

  if(dobbelsteen != berekenDobbelsteen()) {
    dobbelsteen = berekenDobbelsteen();
    dVoorGetal = true;
    getal = dobbelsteen;
  }
  displayInt(getal);
}

int berekenDobbelsteen() {
  const int waarde = constrain(maxPotentiometerWaarde - analogRead(maxDobbelPin),0,maxPotentiometerWaarde);
  float procent = waarde / (float)maxPotentiometerWaarde * 100.0;
  const int aantalDobbelstenen = sizeof(dobbelstenen) / sizeof(dobbelstenen[0]);

  for(int i = 1; i <= aantalDobbelstenen; i += 1) {
    if(procent <= i*100.0/aantalDobbelstenen) {
      return dobbelstenen[i-1];
    }
  }
}


void animatie() {
  dVoorGetal = false;
  int maxInclusief = dobbelsteen;
  unsigned long startTijd = millis();
  unsigned long vorigeUpdate = startTijd;
  float animatieDuur = 2000;
  float animatieVerhouding = 0.0; // 0 is nog niet begonnen, 1 is helemaal klaar
  float delayTijd = 10;

  while (animatieVerhouding < 1.0) {
    unsigned long nu = millis();
    unsigned long verstreken = nu - startTijd;

    animatieVerhouding = (float)verstreken / animatieDuur;
    if(animatieVerhouding >= 1.0) {
      break;
    }

    float tijdKwadraat = animatieVerhouding * animatieVerhouding;

    delayTijd = 10 + tijdKwadraat * 500;

    if (nu - vorigeUpdate >= delayTijd) {
      vorigeUpdate = nu;
      getal = random(1, maxInclusief + 1);
    }

    displayInt(getal);
  }
  dobbelsteen = berekenDobbelsteen();
}

void displayInt(const int getal) {
  byte digit1 = floor(getal/1000);
  byte digit2 = floor((getal % 1000)/100);
  byte digit3 = floor((getal % 100)/10);
  byte digit4 = getal % 10;

  if(getal == 10000) digit1 = 0;

  const bool digit1Leeg = getal != 10000 && digit1 == 0;
  const bool digit2Leeg = digit1Leeg && digit2 == 0;
  const bool digit3Leeg = digit2Leeg && digit3 == 0;
  // digit4 is nooit leeg
  
  getalLocatie(1);
  if(digit1Leeg) {
    if(!digit2Leeg && dVoorGetal) {
      displayDigit(dChar);
    } else {
      displayDigit(-1);
    }
  } else {
    displayDigit(digit1);
  }
  delay(1/knipperSnelheid);
  clear();

  getalLocatie(2);
  if(digit2Leeg) {
    if(!digit3Leeg) {
      if(dVoorGetal) {
        displayDigit(dChar);
      } else {
        displayDigit(-1);
      }
    } else {
      displayDigit(-1);
    }
  } else {
    displayDigit(digit2);
  }
  delay(1/knipperSnelheid);
  clear();

  getalLocatie(3);
  if(digit3Leeg) {
      if(dVoorGetal) {
        displayDigit(dChar);
      } else {
        displayDigit(-1);
      }
  } else {
    displayDigit(digit3);
  }
  delay(1/knipperSnelheid);
  clear();

  getalLocatie(4);
  displayDigit(digit4);
  delay(1/knipperSnelheid);
  clear();
}

void getalLocatie(const byte locatie) {
  if(locatie == 1) digitalWrite(d1, LOW);
  if(locatie == 2) digitalWrite(d2, LOW);
  if(locatie == 3) digitalWrite(d3, LOW);
  if(locatie == 4) digitalWrite(d4, LOW);

  if(locatie != 1) digitalWrite(d1, HIGH);
  if(locatie != 2) digitalWrite(d2, HIGH);
  if(locatie != 3) digitalWrite(d3, HIGH);
  if(locatie != 4) digitalWrite(d4, HIGH);
}

byte getalNaarByte(const int getal) {
  switch(getal) {
    case 0:
      return disp0;
    case 1:
      return disp1;
    case 2:
      return disp2;
    case 3:
      return disp3;
    case 4:
      return disp4;
    case 5:
      return disp5;
    case 6:
      return disp6;
    case 7:
      return disp7;
    case 8:
      return disp8;
    case 9:
      return disp9;
    case -2:
      return dispD;
    default:
      return dispNull;
  }
}

void displayDigit(const int digit) {
  int digitByte = getalNaarByte(digit);
digitalWrite(a, (digitByte & B10000000) ? HIGH : LOW);
digitalWrite(b, (digitByte & B01000000) ? HIGH : LOW);
digitalWrite(c, (digitByte & B00100000) ? HIGH : LOW);
digitalWrite(d, (digitByte & B00010000) ? HIGH : LOW);
digitalWrite(e, (digitByte & B00001000) ? HIGH : LOW);
digitalWrite(f, (digitByte & B00000100) ? HIGH : LOW);
digitalWrite(g, (digitByte & B00000010) ? HIGH : LOW);
  // negeer dp
}

void clear() {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);

  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
}