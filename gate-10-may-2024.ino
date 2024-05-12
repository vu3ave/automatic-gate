// FINAL CODE FOR AUTOMATIC GATE //  First commsioned version on 10/5/2024


//Programe code for Automatic lawn gate opening/closing controller, using arduino nano
// Made: VU2JLX ,  OM JOSEPH
// date: 10 May 2024
// Reff: https://www.youtube.com/watch?v=OS_791bFRZY  
// Reff PEROGRAME CODE BY vu3ave: https://github.com/vu3ave/automatic-gate
// Made by Anish VR, near little hearts school, Kizhekkepram, NORTH PARAVUR , KERALA, INDIA (VU3AVE) CONTACT: vu3ave@gmail.com anishvrajan@gmail.com // MOBILE:+91 9995957776





/// FINAL CODE /////////////// WORKNG //////////////////////////
// Press A to close gate continuesly 
// Press B to open gate continuesly 

// WIRING DETAILS//////////////////////////////////////////////
// use two numbers of 3.3k ohm pullup resistor, connected to +5v in both inputs digital pins

// Speaker tones
// 1. Start up tone - 'OK' in telegraphy
// 2. Confirmation tone - during each key press A and B inputs
// 3. SMPS OFF tone - after completing timeout time (17 seconds) - 'AR' in telegraphy



////////////////////////////////////////////////////














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

int opening=0;
int closing=0;
int stoping=0;


// tone frequency for speaker
int notes1[] = {
  1000, 1175, 1319, 1650
};

int notes2[] = {
1668, 1319, 1175, 1000
};



/// timer variables///////////////////////////////////////////////////////////////////////////////////////////////
const long interval = 17000; //  ( 17000; )....Means 17 seconds for door opening /closing time...ADJUST IF NECESSORY
unsigned long currentMillis = 0;






void setup() {
  
   Serial.begin(9600);
  pinMode(remotepin_A, INPUT);
  pinMode(remotepin_B, INPUT);

  pinMode(Relay_FR, OUTPUT);
  pinMode(Relay_clutch_n_motor, OUTPUT);  
  pinMode(Relay_smps, OUTPUT);
  pinMode(tone_pin, OUTPUT);

digitalWrite(Relay_smps, LOW); // default smps supply are off
digitalWrite(Relay_FR, LOW); // default  FR Relay is off  //  gate in forward gear
digitalWrite(Relay_clutch_n_motor, LOW); // default  Clutch Relay is off  //  clutch is released to  free movement of gate

// INTRODUCTION TONE
intro_tone();


currentMillis = millis(); // resetting timer
}

void loop() {


/// timer handling for 24V SMPS //////////////////////////
 
 
  unsigned long loopMillis = millis();

 if (loopMillis - currentMillis  >= interval) {  

smps_status = 0;

 gate_status = 10; 








    Serial.print("24V SMPS IS: ");
    Serial.print(smps_status);
    Serial.println(" ");
  }
///////////////////////////////////////////// 




 
 //////////////sampling and determining on off
 remotepin_A_newstate_1 = digitalRead(remotepin_A);
 remotepin_B_newstate_1 = digitalRead(remotepin_B);


if(remotepin_A_newstate_1 == 1 ){ 
Serial.println("A : Not Pressed"); 
statA = 0;
}else if(remotepin_A_newstate_1 == 0 ){ 
Serial.println("A :  Pressed"); 
statA = 1;
}

delay(50);

if(remotepin_B_newstate_1 == 1 ){ 
Serial.println("B : Not Pressed"); 
statB = 0;
}else if(remotepin_B_newstate_1 == 0 ){ 
Serial.println("B :  Pressed"); 
statB = 1;
}
////////////////////////////////////////////////



////////// gate control//////////////

if( statA != Old_statA){
  Serial.println("Button A statechanged");
  Serial.println(" ");
  Serial.print("StatA is:");
  Serial.print(statA);
  Serial.println(" ");
  
button_confirm_tone();
  
currentMillis = millis(); // resetting timer
}
Old_statA = statA;





if( statB != Old_statB){

  Serial.println("Button B statechanged");
  Serial.println(" ");
  Serial.print("StatB is:");
  Serial.print(statB);
  Serial.println(" ");
  button_confirm_tone();
  
currentMillis = millis(); // resitting timer
 
 }
Old_statB = statB;


Serial.println(" ");
  Serial.print("GATE STATUS is : ");
  Serial.print(gate_status);
  Serial.println(" ");
  
////////////////////////////////////////////////////////////////////
//////////////controling relays for gate///////////////////////////




 if(gate_status == 12 && smps_status == 1){  digitalWrite(Relay_FR, HIGH); delay(50); digitalWrite(Relay_clutch_n_motor, HIGH); }
else if(gate_status == 12 && smps_status == 0){ //digitalWrite(Relay_smps, LOW);
  delay(50); digitalWrite(Relay_clutch_n_motor, LOW); delay(50); digitalWrite(Relay_FR, LOW);}

else if(gate_status == 11 && smps_status == 1){digitalWrite(Relay_FR, LOW); delay(50); digitalWrite(Relay_clutch_n_motor, HIGH);  }
else if(gate_status == 11 && smps_status == 0){digitalWrite(Relay_FR, LOW); delay(50); digitalWrite(Relay_clutch_n_motor, LOW);   }
 
 
 if(smps_status == 1){ delay(50);  digitalWrite(Relay_smps, HIGH); }else{ //digitalWrite(Relay_smps, LOW);
  delay(50);  digitalWrite(Relay_FR, LOW); }



if(gate_status == 10){// digitalWrite(Relay_smps, LOW);
  delay(50);  digitalWrite(Relay_clutch_n_motor, LOW); delay(50); digitalWrite(Relay_FR, LOW); }



//////////////tone for gate stoping//////+++
last_gate_status = gate_status;
if(last_gate_status == 11 && gate_status == 10){ gate_ar(); }
if(last_gate_status == 12 && gate_status == 10){ gate_ar(); }
if(last_smps_status == 1 && smps_status == 0){ gate_ar(); }
last_smps_status = smps_status;

//////////////tone for gate stoping//////---


//delay(500);//test delay
}// loop closing
















////////// additional functions//////////

void button_confirm_tone(){
tone(tone_pin, 1500);
delay(100); 
noTone(tone_pin); 
delay(200); 

smps_status = 1;

// Final code 10-5-2024/////////////////////////////////////////////
if(Old_statA ==0  && statA==1 ){  gate_status = 11; }


else if(Old_statB ==0  && statB==1 ){  gate_status = 12; }else{ 
  //gate_status = 10;
}
//Final code 10-5-2024//////////////////////////////////////////////








}





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
// GATE STOPPING TONE "AR"



 digitalWrite(Relay_smps, LOW);

tone(tone_pin, 1500);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 1500);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 1500);
delay(100); 
noTone(tone_pin); 
}




void intro_tone(){
// GATE PROGRME STARTING,  INRODUCTION TONE (OK)

/////////////////////////////1
tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(100);

tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(300);


tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 1500);
delay(100); 
noTone(tone_pin); 

delay(100);


tone(tone_pin, 1500);
delay(300); 
noTone(tone_pin); 


}
////////////////////////// Thank you for comming upto this end
