#include <ArduinoBoardManager.h>

ArduinoBoardManager arduino = ArduinoBoardManager();

const bool DEBUG = false;

bool isInited = false;
int digitals[80];
int numDigitals;
int analogs[80];
int numAnalogs;
int pins[160];
int numPins;
const char* buf;

char* board = arduino.BOARD_NAME;

void start(){
  // use board type to get GPIO info
  if (!strcmp(board,"UNO")){
    numDigitals = 14; // digitals 0->13 := 0->13
    numAnalogs = 6; // analogs 14->19 := A0->A5
    for (int i = 0; i<numDigitals; i++){
      digitals[i] = i;
      pins[i] = i;
      if (DEBUG){
        Serial.print("Digital ");
        Serial.println(digitals[i]);
      }
      pinMode(pins[i], INPUT);
      digitalWrite(pins[i], LOW);
    }
    for (int i = 0; i<numAnalogs; i++){
      analogs[i] = i+14;
      pins[i] = analogs[i];
      if (DEBUG){
        Serial.print("Analog ");
        Serial.println(analogs[i]);
      }
      pinMode(pins[i], INPUT);
      digitalWrite(pins[i], LOW);
    }
  }
  else if (!strcmp(board,"MEGA")){
    numDigitals = 54; // digitals 0->53 := 0->53
    numAnalogs = 16; // analogs 97->82 := A0->A15
    for (int i = 0; i<numDigitals; i++){
      digitals[i] = i;
      pins[i] = i;
      if (DEBUG){
        Serial.print("Digital ");
        Serial.println(digitals[i]);
      }
      pinMode(pins[i], INPUT);
      digitalWrite(pins[i], LOW);
    }
    for (int i = numAnalogs; i>=82; i--){
      analogs[i] = i+82;
      pins[i] = analogs[i];
      if (DEBUG){
        Serial.print("Analog ");
        Serial.println(analogs[i]);
      }
      pinMode(pins[i], INPUT);
      digitalWrite(pins[i], LOW);
    }
  }

  numPins = numDigitals + numAnalogs;
}

void send(char cmd, int tx){
  char _send[100];
  sprintf(_send, "&%c%d&%c", cmd, tx, cmd);
  Serial.println(_send);
}

void setup() {
  // Serial connection
  Serial.begin(115200);
  Serial.setTimeout(5);

  start();
  
  // 10-bit analog reads
//  if (!strcmp(arduino.BOARD_NAME, "MEGA"))
//    analogReadResolution(10);

  // wait for board verification request
  while (1){
    if (Serial.available()>1 && Serial.read()=='&'){
      if (Serial.peek()=='^'){ // get second header character
        Serial.read();  //junk the character
        if (DEBUG) Serial.println("received");
        char _send[100];
        sprintf(_send, "&^%s&^", board);
        Serial.println(_send); // send verification / board type
        break; // break out of while loop
      }
    }
  }
  
} // end setup()


void loop() {
  while (Serial.available()>1){
    if (Serial.read()=='&'){
      if (DEBUG) Serial.println("&");
      int p, v;
      char input = Serial.read();
      switch(int(input)){
        case ')': // analog read
          p = Serial.parseInt();
          send(input,analogRead(p));
          if (DEBUG){
            Serial.print("analog read ");
            Serial.println(p);
          }
        break;
        case '(': // digital read
          p = Serial.parseInt();
          send(input,digitalRead(p));
          if (DEBUG){
            Serial.print("digital read ");
            Serial.println(p);
          }
        break;
        case '@': // analog write
          p = Serial.parseInt();
          v = Serial.parseInt();
          analogWrite(p,v);
          if (DEBUG){
            Serial.print("analog write ");
            Serial.println(p);
            Serial.println(v);
          }
        break;
        case '#': // digital write
          p = Serial.parseInt();
          v = Serial.parseInt();
          analogWrite(p,v!=0);
          if (DEBUG){
            Serial.print("digital write ");
            Serial.println(p);
            Serial.println(v);
          }
        break;
        case '$': // pwm
          p = Serial.parseInt();
          v = Serial.parseInt();
          if (DEBUG){
            Serial.print("pwm write ");
            Serial.println(p);
            Serial.println(v);
          }
        break;
        case '^': // verify
          char tx[100];
          sprintf(tx, "&^%s&^", board);
          Serial.println(tx); // send verification / board type
          if (DEBUG) Serial.println("verify");
        break;
        case '%': // pin mode
          p = Serial.parseInt();
          v = Serial.parseInt();
          pinMode(p,v!=0);
          if (DEBUG){
            Serial.print("pin mode ");
            Serial.println(p);
            Serial.println(v);
          }
        break;
        default: // anything else
          send('!',int(input));
          if (DEBUG){
            Serial.print("Invalid cmd ");
            Serial.println(input);
          }
        break;
      } // end switch(input)
    } // end if '&'
  } // end while Serial.available()
} // end loop

