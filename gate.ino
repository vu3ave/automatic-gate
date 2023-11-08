// initial upload : 8/11/2023 by vu3ave
// defining input and output pins

int remotepin_A = 2;
int remotepin_B = 3;
int Relay_smps= 10;
int Relay_FR = 11;
int Relay_clutch_n_motor = 12;
int tone_pin=13;


int smps_status = 0; // default smps power supplys are OFF
int gate_status = 10; // 10 = stoped  ,, 11 = opening  ,, 12 = closing


// sampling variables
int remotepin_A_newstate_1 = 0;
int remotepin_B_newstate_1 = 0;

// last State A and B storage variables
int statA = 0;
int statB = 0;
int Old_statA = 0;
int Old_statB = 0;






/// timer variables
const long interval = 5000; // 5 seconds for door opening time
unsigned long currentMillis = 0;






void setup() {
  
   Serial.begin(9600);
  pinMode(remotepin_A, INPUT);
  pinMode(remotepin_B, INPUT_PULLUP);

  pinMode(Relay_FR, OUTPUT);
  pinMode(Relay_clutch_n_motor, OUTPUT);  
  pinMode(Relay_smps, OUTPUT);
  pinMode(tone_pin, OUTPUT);

digitalWrite(Relay_smps, LOW); // default smps supply are off
digitalWrite(Relay_FR, LOW); // default  FR Relay is off  //  gate in forward gear
digitalWrite(Relay_clutch_n_motor, LOW); // default  Clutch Relay is off  //  clutch is released to  free movement of gate


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
currentMillis = millis(); // resetting timer

  
  if( gate_status != 10) { Serial.println("gate stop sequence starts ///// "); gate_status = 10; }
  else if( gate_status == 10 && statA == 1){  Serial.println("opening sequence starts.......");    gate_status = 11;}
  
}

Old_statA = statA;


if( statB != Old_statB){

  Serial.println("Button B statechanged....................");
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




 if(gate_status == 12 && smps_status == 1){ delay(500); digitalWrite(Relay_FR, HIGH); delay(500); digitalWrite(Relay_clutch_n_motor, HIGH); }
else if(gate_status == 12 && smps_status == 0){ digitalWrite(Relay_smps, LOW); delay(500); digitalWrite(Relay_clutch_n_motor, LOW); delay(500); digitalWrite(Relay_FR, LOW);}

else if(gate_status == 11 && smps_status == 1){digitalWrite(Relay_FR, LOW); delay(500); digitalWrite(Relay_clutch_n_motor, HIGH);  }
else if(gate_status == 11 && smps_status == 0){digitalWrite(Relay_FR, LOW); delay(500); digitalWrite(Relay_clutch_n_motor, LOW);   }
 
 
 if(smps_status == 1){ delay(500);  digitalWrite(Relay_smps, HIGH); }else{ digitalWrite(Relay_smps, LOW); delay(500);  digitalWrite(Relay_FR, LOW); }



if(gate_status == 10){ digitalWrite(Relay_smps, LOW); delay(500);  digitalWrite(Relay_clutch_n_motor, LOW); delay(500); digitalWrite(Relay_FR, LOW); }





}



