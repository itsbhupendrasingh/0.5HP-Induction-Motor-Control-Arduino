/*
 * 
 * Dimmer testing for fan
 * Dated: 9 May 2022
 * 
 */

#include <TimerOne.h>
const int sw1=6, sw2=7;
int sw1s=0, sw2s=0;
const int led1=A0, led2=A1, led3=A2, led4=A3;
int data1,data2,data3,data4;
                   
volatile int i=0;               // Variable to use as a counter
volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin = 5;                 // Output to Opto Triac

int dim2 = 0;                   // led control
int dim = 128;                  // Dimming level (0-128)  0 = on, 128 = 0ff                  
int freqStep = 75;    // This is the delay-per-brightness step in microseconds.
int onoff=0;
void setup() { 
  Serial.begin(9600); 
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep); 
  delay(1000); 
  Serial.println("Manual Control of Induction Motor using ATmega Microcontroller");
  Serial.println("Dimming Level: " + (String)dim);
  
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led2, HIGH);
    delay(1000);
    digitalWrite(led3, HIGH);
    delay(1000);
    digitalWrite(led4, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    delay(1000);
}

void zero_cross_detect() 
{    
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  digitalWrite(AC_pin, LOW);
}                                 
// Turn on the TRIAC at the appropriate time

void dim_check() 
{                   
  if(zero_cross == true) {              
    if(i>=dim) {                     
      digitalWrite(AC_pin, HIGH);  // turn on light       
      i=0;  // reset time step counter                         
      zero_cross=false;    // reset zero cross detection
    } 
    else 
    {
      i++;  // increment time step counter                     
    }
  }
}                                      

void loop() {  
  call_manual();
  if(dim==128)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }
  if(dim==96)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }
  if(dim==64)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }
  if(dim==32)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
  }
  if(dim==0)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
}

void call_manual()
{
sw1s=digitalRead(sw1);
sw2s=digitalRead(sw2);

      if(sw1s==0)
      {
      delay(5);
        sw1s=digitalRead(sw1);
        if(sw1s==1)
        {
           if (dim<127)  
           {
            dim = dim + 32;
            Serial.println(dim);

              if (dim>127) 
              {
                dim=128; // in vechiul sketch era 127
              }
             }
        }
       }

      if(sw2s==0)
      {
        delay(5);
        sw2s=digitalRead(sw2);
        if(sw2s==1)
        {
            
          if (dim>5)  
          {
             dim = dim - 32;
             
              Serial.println(dim);
            if (dim<0) 
            {
              dim=0;  // in vechiul sketch era 1
            } 
          }
        }
      }
}
