/* 
PREFIXES : 
L = LAMPE
V = VOLET

Lampes disponibles : 
0 --> lampe 1
1 --> lampe 2
2 --> lampe 3 
G --> Toutes les lampes

Commandes disponibles :
0 --> Extinction
1 --> Allumage
*/

/* 
Volets disponibles : 
R --> Volet chambre R / ADDRESS : 7CC356

Commandes disponibles (controle) :
M --> Monte
B --> Descend
S --> Stop
 */
 
// Variables des entrées sorties
int soundSensor = 4;
int emetteur_433_92 = 11;
// int emetteur_433_42 = 5;

// Variable du statut des lampes
boolean LampesStatus=false;

// Variable de commande
int commande [3];
int i = 0;

// Import et déclaration de l'emetteur des lampes
#include <NewRemoteTransmitter.h>
NewRemoteTransmitter transmitter(27870606, emetteur_433_92, 266);

// PARTIE NECESSAIRE A L'EMISSION SOMFY

#include <EEPROM.h>
#define PORT_TX 5 //5 of PORTD = DigitalPin 5

#define SYMBOL 640
#define HAUT 0x2
#define STOP 0x1
#define BAS 0x4
#define PROG 0x8
#define EEPROM_ADDRESS 0

#define REMOTE 0x7CC356    //<-- Change it!

unsigned int newRollingCode = 20;       //<-- Change it!
unsigned int rollingCode = 0;
byte frame[7];
byte checksum;

void BuildFrame(byte *frame, byte button);
void SendCommand(byte *frame, byte sync);

// FIN DE LA PARTIE SOMFY

void setup() {
 pinMode(soundSensor,INPUT);
 pinMode(emetteur_433_92,OUTPUT);
 // pinMode(emetteur_433_42,OUTPUT);
 Serial.begin(115200);
 
// POUR SOMFY
  DDRD |= 1<<PORT_TX; // Pin 5 an output
  PORTD &= !(1<<PORT_TX); // Pin 5 LOW

  if (EEPROM.get(EEPROM_ADDRESS, rollingCode) < newRollingCode) {
    EEPROM.put(EEPROM_ADDRESS, newRollingCode);
  }
  Serial.print("Simulated remote number : "); Serial.println(REMOTE, HEX);
  Serial.print("Current rolling code    : "); Serial.println(rollingCode);
 
}
void loop() {
while(Serial.available() && i<3){
  commande[i] = Serial.read();
  i = i + 1;
  }
  check_command();
while(!Serial.available()){ // Attente de commande
}
i=0;
delay(1);
}

void check_command() {
  if (char(commande[0]) == 'L')
  {
    lampe_send();
  }
  else if(char(commande[0]) == 'V'){
    volet_send();
  }
}

void lampe_send () {
  if(char(commande[1]) == 'G') // Cas de groupe
    {
       if(char(commande[2]) == '0' || '1')
        {
          int action = char(commande[2]) - 48;
          transmitter.sendGroup(action);
          if(action==1){
            LampesStatus=true;
          }else{
            LampesStatus=false;
          }
        }
        
    }
    else if(char(commande[1]) == '0' || '1' || '2') // Cas de pilote unite
    {
      if(char(commande[2]) == '1' || '0'){
        int lampe = char(commande[1]) - 48;
        int action = char(commande[2]) - 48;
      transmitter.sendUnit(lampe, action);
      }
    }
}

void volet_send () {
  if(char(commande[1]) == 'R') { // Cas de chambre R
  if(char(commande[2]) == 'M') { // Monte
      Serial.println("Monte"); 
      BuildFrame(frame, HAUT);
    }
    else if(char(commande[2]) == 'S') { // Stop
      Serial.println("Stop");
      BuildFrame(frame, STOP);
    }
    else if(char(commande[2]) == 'B'){ // Descend
      Serial.println("Descend");
      BuildFrame(frame, BAS);
    }
    Serial.println("");
    SendCommand(frame, 2);
    for(int i = 0; i<2; i++) {
      SendCommand(frame, 7);
    }
  }
   
}

// SCRIPT D'EMISSION SOMFY

void BuildFrame(byte *frame, byte button) {
  unsigned int code;
  EEPROM.get(EEPROM_ADDRESS, code);
  frame[0] = 0xA7; // Encryption key. Doesn't matter much
  frame[1] = button << 4;  // Which button did  you press? The 4 LSB will be the checksum
  frame[2] = code >> 8;    // Rolling code (big endian)
  frame[3] = code;         // Rolling code
  frame[4] = REMOTE >> 16; // Remote address
  frame[5] = REMOTE >>  8; // Remote address
  frame[6] = REMOTE;       // Remote address

  Serial.print("Frame         : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) { //  Displays leading zero in case the most significant
      Serial.print("0");     // nibble is a 0.
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }
  
// Checksum calculation: a XOR of all the nibbles
  checksum = 0;
  for(byte i = 0; i < 7; i++) {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // We keep the last 4 bits only


//Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.

  Serial.println(""); Serial.print("With checksum : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) {
      Serial.print("0");
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }

  
// Obfuscation: a XOR of all the bytes
  for(byte i = 1; i < 7; i++) {
    frame[i] ^= frame[i-1];
  }

  Serial.println(""); Serial.print("Obfuscated    : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) {
      Serial.print("0");
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Rolling Code  : "); Serial.println(code);
  EEPROM.put(EEPROM_ADDRESS, code + 1); //  We store the value of the rolling code in the
                                        // EEPROM. It should take up to 2 adresses but the
                                        // Arduino function takes care of it.
}

void SendCommand(byte *frame, byte sync) {
  if(sync == 2) { // Only with the first frame.
  //Wake-up pulse & Silence
    PORTD |= 1<<PORT_TX;
    delayMicroseconds(9415);
    PORTD &= !(1<<PORT_TX);
    delayMicroseconds(89565);
  }

// Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i++) {
    PORTD |= 1<<PORT_TX;
    delayMicroseconds(4*SYMBOL);
    PORTD &= !(1<<PORT_TX);
    delayMicroseconds(4*SYMBOL);
  }

// Software sync
  PORTD |= 1<<PORT_TX;
  delayMicroseconds(4550);
  PORTD &= !(1<<PORT_TX);
  delayMicroseconds(SYMBOL);
  
  
//Data: bits are sent one by one, starting with the MSB.
  for(byte i = 0; i < 56; i++) {
    if(((frame[i/8] >> (7 - (i%8))) & 1) == 1) {
      PORTD &= !(1<<PORT_TX);
      delayMicroseconds(SYMBOL);
      PORTD ^= 1<<5;
      delayMicroseconds(SYMBOL);
    }
    else {
      PORTD |= (1<<PORT_TX);
      delayMicroseconds(SYMBOL);
      PORTD ^= 1<<5;
      delayMicroseconds(SYMBOL);
    }
  }
  
  PORTD &= !(1<<PORT_TX);
  delayMicroseconds(30415); // Inter-frame silence
}

