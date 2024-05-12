// FINAL CODE FOR AUTOMATIC GATE//12-may-2024///FINAL INSTALLATION//12-5-2024 /// SINGLE CLICK ACTIVATION OF A AND B


//Programe code for Automatic lawn gate opening/closing controller, using arduino nano
// Made: VU2JLX ,  OM JOSEPH
// date: 12 May 2024
// Reff: https://www.youtube.com/watch?v=OS_791bFRZY  
// Reff PEROGRAME CODE BY vu3ave: https://github.com/vu3ave/automatic-gate
// Made by Anish VR, near little hearts school, Kizhekkepram, NORTH PARAVUR , KERALA, INDIA (VU3AVE) CONTACT: vu3ave@gmail.com anishvrajan@gmail.com // MOBILE:+91 9995957776





/// FINAL CODE /////////////// WORKNG //////////////////////////
// Press A to close gate continuesly 
// Press B to open gate continuesly 
// Start A on first click of Remote switch itself
// Start B on first click of Remote switch itself



// WIRING DETAILS//////////////////////////////////////////////
// use two numbers of 3.3k ohm pullup resistor IN PUT Pins D2 and D3, connected to +5v in both inputs digital pins 2 and 3



// SPEAKER TONES///////////////////////////////////////////////
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




int ButtonA_changed=0;
int ButtonB_changed=0;



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

  smps_status =1;

  Serial.println("Button A statechanged");
  Serial.println(" ");
  Serial.print("StatA is:");
  Serial.print(statA);
  Serial.println(" ");



// Button A Changed //Gate is Opening procedure continue//
if(smps_status == 1){  digitalWrite(Relay_FR, LOW); delay(100); digitalWrite(Relay_clutch_n_motor, HIGH); }
else if(smps_status == 0){   digitalWrite(Relay_clutch_n_motor, LOW); delay(1000); digitalWrite(Relay_FR, LOW); }

  
button_confirm_tone();



  
currentMillis = millis(); // resetting timer
}
Old_statA = statA;





if( statB != Old_statB){

  smps_status =1;

  Serial.println("Button B statechanged");
  Serial.println(" ");
  Serial.print("StatB is:");
  Serial.print(statB);
  Serial.println(" ");

// Button B Changed //Gate is Closing procedure continue//
if(smps_status == 1){  digitalWrite(Relay_FR, HIGH); delay(100); digitalWrite(Relay_clutch_n_motor, HIGH); }
else if(smps_status == 0){    digitalWrite(Relay_clutch_n_motor, LOW); delay(1000);  digitalWrite(Relay_FR, LOW); }


  
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


//Power off procedure
 if(smps_status == 1){ delay(50);  digitalWrite(Relay_smps, HIGH); }else{ delay(50); digitalWrite(Relay_smps, LOW); delay(500); }



//////////////gate stoping//////
last_gate_status = gate_status;
if(last_smps_status == 1 && smps_status == 0){ gate_ar(); }
last_smps_status = smps_status;

//////////////gate stoping//////


//delay(500);//test delay
}// loop closing
















////////// additional functions//////////

void button_confirm_tone(){
tone(tone_pin, 1700);
delay(100); 
noTone(tone_pin); 
delay(200); 

smps_status = 1;



//Final code 12-5-2024//////////////////////////////////////////////

}


















void gate_ar()
{
// GATE STOPPING TONE "AR"
//ALL RELAYS ARE OFF IN 1 Sec delay


 digitalWrite(Relay_smps, LOW);
 delay(1000);
 digitalWrite(Relay_clutch_n_motor, LOW);
 delay(1000);
 digitalWrite(Relay_FR, LOW);


 

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
