
//Programe code for Automatic lawn gate opening/closing controller, using arduino nano
// Made: VU2JLX ,  OM JOSEPH
// date: 2 May 2024
// Reff: https://www.youtube.com/watch?v=OS_791bFRZY  
// Reff PEROGRAME CODE BY vu3ave: https://github.com/vu3ave/automatic-gate





// defining input and output pins

int remotepin_A = 2;
int remotepin_B = 3;
int Relay_smps= 10;
int Relay_FR = 11;
int Relay_clutch_n_motor = 12;
int tone_pin=13;


int smps_status = 0; // default smps power supplys are OFF
int gate_status = 10; // 10 = stoped  ,, 11 = opening  ,, 12 = closing

int last_gate_status = 10;
int last_smps_status = 0;


// sampling variables
int remotepin_A_newstate_1 = 0;
int remotepin_B_newstate_1 = 0;

// last State A and B storage variables
int statA = 0;
int statB = 0;
int Old_statA = 0;
int Old_statB = 0;




// tone frequency for speaker
int notes1[] = {
  1047, 1319, 1175, 1568
};

int notes2[] = {
1568, 1175, 1319, 1047
 
};



/// timer variables
const long interval = 24000; //  ( 24000; )....Means 24 seconds for door opening time...ADJUST IF NECESSORY
unsigned long currentMillis = 0;






void setup() {
  
   Serial.begin(9600);
  pinMode(remotepin_A, INPUT_PULLUP);
  pinMode(remotepin_B, INPUT_PULLUP);

  pinMode(Relay_FR, OUTPUT);
  pinMode(Relay_clutch_n_motor, OUTPUT);  
  pinMode(Relay_smps, OUTPUT);
  pinMode(tone_pin, OUTPUT);

digitalWrite(Relay_smps, LOW); // default smps supply are off
digitalWrite(Relay_FR, LOW); // default  FR Relay is off  //  gate in forward gear
digitalWrite(Relay_clutch_n_motor, LOW); // default  Clutch Relay is off  //  clutch is released to  free movement of gate

// INTRODUCTION TONE
intro_tone();

}

void loop() {


/// timer handling//////////////////////////
 
 
  unsigned long loopMillis = millis();

 if (loopMillis - currentMillis  >= interval) {

smps_status = 0;
    Serial.print("SMPS IS OFF.:currentmils is: ");
    Serial.print(currentMillis);
    Serial.println(" ");
  }else{
    
if(gate_status == 10){ smps_status = 0; }else{ smps_status = 1; }

    
        Serial.print("SMPS IS ON.:currentmils is: ");
    Serial.print(currentMillis);
    Serial.println(" ");
  }
///////////////////////////////////////////// 




 
 //////////////sampling and determining on off
 remotepin_A_newstate_1 = digitalRead(remotepin_A);
 remotepin_B_newstate_1 = digitalRead(remotepin_B);


if(remotepin_A_newstate_1 == 1 ){ 
Serial.println("A Offf"); 
statA = 0;
}else if(remotepin_A_newstate_1 == 0 ){ 
Serial.println("A ONN"); 
statA = 1;
}

delay(50);

if(remotepin_B_newstate_1 == 1 ){ 
Serial.println("B Offf"); 
statB = 0;
}else if(remotepin_B_newstate_1 == 0 ){ 
Serial.println("B ONN"); 
statB = 1;
}
////////////////////////////////////////////////



////////// gate control//////////////

if( statA != Old_statA){

  Serial.println("Button A statechanged....................");

button_confirm_tone();
  
currentMillis = millis(); // resetting timer

  
  if( gate_status != 10) { Serial.println("gate stop sequence starts ///// "); gate_status = 10; }
  else if( gate_status == 10 && statA == 1){  Serial.println("opening sequence starts.......");    gate_status = 11;}
  
}

Old_statA = statA;


if( statB != Old_statB){

  Serial.println("Button B statechanged....................");

  button_confirm_tone();
currentMillis = millis(); // resitting timer
 if( gate_status != 10) { Serial.println("gate stop sequence starts ///// "); gate_status = 10; }
 else if( gate_status == 10 && statB == 1){  Serial.println("CLOSING sequence starts.......");    gate_status = 12;}
 
}

Old_statB = statB;


  Serial.print("GATE STATUS: ");
  Serial.print(gate_status);
  Serial.println(" ");
  
////////////////////////////////////////////////////////////////////
//////////////controling relays for gate///////////////////////////




 if(gate_status == 12 && smps_status == 1){ delay(500); digitalWrite(Relay_FR, HIGH); gate_closing(); digitalWrite(Relay_clutch_n_motor, HIGH); }
else if(gate_status == 12 && smps_status == 0){ digitalWrite(Relay_smps, LOW); delay(500); digitalWrite(Relay_clutch_n_motor, LOW); delay(500); digitalWrite(Relay_FR, LOW);}

else if(gate_status == 11 && smps_status == 1){digitalWrite(Relay_FR, LOW); gate_opening(); digitalWrite(Relay_clutch_n_motor, HIGH);  }
else if(gate_status == 11 && smps_status == 0){digitalWrite(Relay_FR, LOW); delay(500); digitalWrite(Relay_clutch_n_motor, LOW);   }
 
 
 if(smps_status == 1){ delay(500);  digitalWrite(Relay_smps, HIGH); }else{ digitalWrite(Relay_smps, LOW); delay(500);  digitalWrite(Relay_FR, LOW); }



if(gate_status == 10){ digitalWrite(Relay_smps, LOW); delay(500);  digitalWrite(Relay_clutch_n_motor, LOW); delay(500); digitalWrite(Relay_FR, LOW); }



//////////////tone for gate stoping//////+++
last_gate_status = gate_status;
if(last_gate_status == 11 && gate_status == 10){ gate_ar(); }
if(last_gate_status == 12 && gate_status == 10){ gate_ar(); }
if(last_smps_status == 1 && smps_status == 0){ gate_ar(); }
last_smps_status = smps_status;

//////////////tone for gate stoping//////---



}


////////// additional functions//////////

void gate_opening()
{
 // GATE OPENING TONE
 
    
  for (int thisSensor = 3; thisSensor >= 0; thisSensor--) {
         tone(tone_pin, notes1[thisSensor]);
         delay(100);
         noTone(tone_pin);
         
  }
}

void gate_closing()
{
 // GATE CLOSING TONE
 
    
 for (int thisSensor = 3; thisSensor >= 0; thisSensor--) {
         tone(tone_pin, notes2[thisSensor]);
         delay(100);
         noTone(tone_pin);
  }
}

void gate_ar()
{
// GATE STOPPING TONE "OK OK"
tone(tone_pin, 900);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(100); 
noTone(tone_pin); 
}

void button_confirm_tone(){
tone(tone_pin, 1000);
delay(100); 
noTone(tone_pin); 
}




void intro_tone(){
// GATE PROGRME STARTING,  INRODUCTION TONE

/////////////////////////////1
tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(300);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(500);
//////////////////////////1

/////////////////////////////2
tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(300);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 900);
delay(300); 
noTone(tone_pin); 

delay(500);
//////////////////////////2
}
