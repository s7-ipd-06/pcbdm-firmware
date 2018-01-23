void setTimers(){
 cli();//stop interrupts

//set timer0 interrupt at 2kHz
/*
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 19;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) ;//| (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
*/
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
//set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 99;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  
  TCCR3A = 0;// set entire TCCR1A register to 0
  TCCR3B = 0;// same for TCCR1B
  TCNT3  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR3A = 19;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR3B |= (1 << WGM32);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR3B |= (1 << CS31) ;//| (1 << CS30);  
  // enable timer compare interrupt
  TIMSK3 |= (1 << OCIE3A);
sei();//allow interrupts 
  
}
volatile long cmpx = 50000;
volatile long cmpy = 50000;

volatile long pcx;
volatile boolean statex=false;
volatile boolean stoppedx = true;

volatile long pcy;
volatile boolean statey=false;
volatile boolean stoppedy = true;


volatile long countx;
volatile long county;


ISR(TIMER3_COMPA_vect){//100K 
 pcx++;
 pcy++;
}



volatile long oldspeedx = 0;
volatile long desspeedx = 0;

volatile long oldspeedy = 0;
volatile long desspeedy = 0;

void setspeedX(int freq){
  desspeedx = freq;
}
void setspeedY(int freq){
  desspeedy = freq;
}
const int accel = 20;

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if(desspeedx == oldspeedx){
    
  }else{
    
  if(desspeedx > 0){
    if(desspeedx>oldspeedx+accel){
      forcespeedx(oldspeedx+accel);
      oldspeedx+=accel;
    }else{
       forcespeedx(desspeedx);
       oldspeedx = desspeedx;
    }
  }else{
    if(desspeedx<oldspeedx-accel&&desspeedx!=0){
      forcespeedx(oldspeedx-accel);
      oldspeedx-=accel;
    }else{
       forcespeedx(desspeedx);
       oldspeedx = desspeedx;
    }
  }}
  
  
  if(desspeedy == oldspeedy){
    
  }else{
    
  if(desspeedy > 0){
    if(desspeedy>oldspeedy+accel){
      forcespeedy(oldspeedy+accel);
      oldspeedy+=accel;
    }else{
       forcespeedy(desspeedy);
       oldspeedy = desspeedy;
    }
  }else{
    if(desspeedy<oldspeedy-accel&&desspeedy!=0){
      forcespeedy(oldspeedy-accel);
      oldspeedy-=accel;
    }else{
       forcespeedy(desspeedy);
       oldspeedy = desspeedy;
    }
  }}
  /*
  Serial.print("X: ");
  Serial.print(countx);
  Serial.print("\t");
  Serial.println(pcx);
  
  Serial.print("Y: ");
  Serial.print(county);
  Serial.print("\t");
  Serial.println(pcy);
  countx = 0;
  county = 0;
  */
}
  
ISR(TIMER2_COMPA_vect){//20K
  
  if(stoppedx){
    pcx=0;
  }else{
    if(pcx>=cmpx){
      pcx-=cmpx;
      if(statex){
        countx++;
      }
      statex = !statex;
      digitalWrite(PULSEPINX,statex);
    }
  } 
  if(stoppedy){
    pcy=0;
    return;
  }else{
    if(pcy>=cmpy){
      pcy-=cmpy;
      if(statey){
        county++;
      }
      statey = !statey;
      digitalWrite(PULSEPINY,statey);
    }
  } 
}
const float base = 100000/2;
void forcespeedx(int freq){
  pcx = 0;
  if(freq == 0){
     stoppedx = true; 
  }else{
    stoppedx = false;
  }
  if(freq<0){
    freq = -freq;
    digitalWrite(DIRPINX,HIGH);
  }else{
    digitalWrite(DIRPINX,LOW);
  }
  
  float f = (float)freq;
  int d = base/freq;
  if(d<5){
    d=5;
  }
  if(d > 500000){
    d = 50000;
  }
  cmpx = d;
 // Serial.print("New X comp value: ");
 // Serial.println(d);
}
void forcespeedy(int freq){
  pcy = 0;
  if(freq == 0){
     stoppedy = true; 
  }else{
    stoppedy = false;
  }
  if(freq<0){
    freq = -freq;
    digitalWrite(DIRPINY,HIGH);
  }else{
    digitalWrite(DIRPINY,LOW);
  }
  
  float f = (float)freq;
  int d = base/freq;
  if(d<5){
    d=5;
  }
  if(d > 500000){
    d = 50000;
  }
  cmpy = d;
  //Serial.print("New Y comp value: ");
  //Serial.println(d);
}
