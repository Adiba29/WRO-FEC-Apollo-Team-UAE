
#include <Pixy2.h>
#include <Servo.h>

Pixy2 pixy;
Servo myservo;

/////////CUSTOM DELAY VARIABLES//////
int period;
unsigned long time_now;

//////////MOTOR DRIVER PINS///////
int in1 = 24; 
int in2 = 25;
int enA = 2;

int in3 = 26;
int in4 = 27;
int enB = 3;
//////////////////////////////

//////////////////ULTRASONIC//////////////////////
#define echo1Pin 11
#define trig1Pin 10
#define echo2Pin 5
#define trig2Pin 4
#define echo3Pin 7
#define trig3Pin 6
#define echo4Pin 33
#define trig4Pin 32
#define echo5Pin 31
#define trig5Pin 30
long left_duration, leftUL_duration, right_duration, rightUL_duration, front_duration;
int left_dist, leftUL_dist, right_dist, rightUL_dist, front_dist;
/////////////////////////////////////////////////

int sped = 200;
int low_Sped = 190;
int counts = 0;
int num = 4;
int center_Pos = 110;
int flagg = 1;
float dist;

/////Push Button Variables////////
int pb = 40;
int pbState = 0;
int pbi = 0;

int loopcount = 0;

void setup() {


  Serial.begin(115200); //pixy baud rate

  pinMode(40, INPUT_PULLUP);
   
  pinMode(13, OUTPUT);
  pinMode(trig1Pin, OUTPUT);
  pinMode(echo1Pin, INPUT);
  
  pinMode(trig2Pin, OUTPUT);
  pinMode(echo2Pin, INPUT);
  
  pinMode(trig3Pin, OUTPUT);
  pinMode(echo3Pin, INPUT);

  
  
  myservo.attach(9);
  myservo.write(center_Pos);
  delay(500);
  myservo.write(60);
  delay(500);
  myservo.write(center_Pos);

  pinMode(enA, OUTPUT); //motor control components
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pixy.init();          //setup pixy
  
}

void loop() {
//  delay(1000);
  ///////////PUSH BUTTON CODE//////////////
  pbState  = digitalRead(pb);
  if(pbState == LOW){
    pbi = 1;
    counts = 0;
    myservo.write(center_Pos);
    Serial.print("Button was pushed. i is ");
    Serial.println(pbi);
  }else {
    Serial.println("Button is LOW");
  }

  /////////////Push Button Start//////////////
  while(pbi == 1){
    pbi = 0;

   int starttime = millis();
   int endtime = starttime;
   
   while ((endtime - starttime) <= 500) // do this loop for up to 1000mS
   {
    // code here
    Serial.println("Moving speed initial stage...");
    
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

//    delay(500);
//    move_Straight();
      
//    loopcount = loopcount+1;
    endtime = millis();
   }

    /////////Main Code/////////
    while(counts < num){
//      delay(500);
      if(counts == num) {
        final_Stage();
        digitalWrite (enA, LOW);
        digitalWrite (enB, LOW);
      }else{ move_Straight();}
    
      get_Pings();
    
      Serial.print("The front Distance Is:");
      Serial.println(front_dist);
    
      Serial.print("right Distance: ");
      Serial.println(right_dist);

      Serial.print("left Distance: ");
      Serial.println(left_dist);  
      Serial.println(" ");

  
  
      Serial.println(" \n\n ");
      
      pixy.ccc.getBlocks();

      if(pixy.ccc.numBlocks){
        Serial.print("Distance to the nearest block detected is: ");
        pixy.ccc.blocks[0].print();
        Serial.println("");
          if(leftUL_dist < 11){
            myservo.write(60);
            Serial.println("vv Left ULTRAsonic detected and moving right, Left ultrasonic dist: ");
            Serial.println(leftUL_dist);
          }
          else if(rightUL_dist < 11){
            myservo.write(160);
            Serial.println("vv Right ULTRAsonic detected and moving left. Right ultrasonic dist: ");
            Serial.println(rightUL_dist);
          }

          else if(front_dist < 15 && front_dist != 0){
//            get_Pings();
            Serial.print("STOPPING, front dist is:___ ");
            Serial.println(front_dist);
            front_Stop();
          }
          
          else if(left_dist < 15 && left_dist != 0){
            Serial.println("VERY NEAR TO LEFT__");
//            get_Pings();
//            pixy.ccc.getBlocks();
            Serial.print("vv Turning right, left dist is: ");
            Serial.println(left_dist);
            myservo.write(80);
          }
          else if(left_dist > 20 && left_dist < 25){
            Serial.println("AWAY FPROM LEFT____");
//            get_Pings();
//            pixy.ccc.getBlocks();
            Serial.print("vv Turning left, left dist is: ");
            Serial.println(left_dist);
            myservo.write(140);
          }
          else if(left_dist >= 25){
            Serial.println("VERY VERY AWAY FPROM LEFT____");
//            get_Pings();
//            pixy.ccc.getBlocks();
            Serial.print("vv Turning left, left dist is: ");
            Serial.println(left_dist);
            myservo.write(120);
          }          
          
          else{
            myservo.write(center_Pos);
          }
   


//      Serial.println("Again print the detected blocks");
//      pixy.ccc.blocks[0].print();
//      Serial.println("");
    ////////ORANGE LINE TURN RIGHT////////
      if (pixy.ccc.blocks[0].m_signature == 3 && pixy.ccc.blocks[0].m_y > 180 && left_dist <=30 && left_dist != 0 ){  // && pixy.ccc.blocks[0].m_x < 55
        Serial.println("Detected ORANGE LINE, turning right");
//        delay(250);
        while((right_dist < 25 && right_dist != 0)||front_dist > 30){
          get_Pings();
          Serial.println("Keep going forward");
          Serial.println("Right dist      Front dist");
          Serial.println(String(right_dist)+"\t\t "+String(front_dist));
          Serial.println("\n");
        }
        while(right_dist > 16 || right_dist == 0){
          get_Pings();
          Serial.print("Turning right until blue detected or right distance is close. Rigth dist: ");
          Serial.println(right_dist);
          pixy.ccc.getBlocks();
          pixy.ccc.blocks[0].print();
          Serial.println("");
          myservo.write(60);
          //detect blue line to stop turning left
          if(pixy.ccc.blocks[0].m_signature == 4 && pixy.ccc.blocks[0].m_y > 180){  //chnanged from 160 t0 200 for m_x ... && pixy.ccc.blocks[0].m_x < 200
            pixy.ccc.blocks[0].print();
            Serial.println("Detected blue line. Will stop turning after the delay...");
//            delay(350);
            period = 350;
            time_now = millis();
            while(millis()< time_now + period){
              //wait approx. period ms
              if(rightUL_dist < 11){
                Serial.println("right ultrasonic detected during blue after turn delay before going straight");
                Serial.println("Right ultrasonic dist: " + String(rightUL_dist));
                break;
              }
            }
            break;
          }
          else if(front_dist < 6 and front_dist != 0){
            Serial.println("front dist too close. Stopping!!");
            Serial.print("STOPPINGgggggg, front dist is:___ ");
            Serial.println(front_dist);
            front_Stop();
            break;
          }
          else if(rightUL_dist < 11 ){
            Serial.println("Right ULTRAsonic detected too close. Stopping right turn immediately and TURNING LEFT! Right ultrasonic dist: ");
            Serial.println(rightUL_dist);
            myservo.write(160);
            period = 300;
            time_now = millis();
            while(millis() < time_now + period){
              //wait approx. period ms
            }
            break;
          }
        }
        counts++;
        Serial.print("Count incremented. count is: ");
        Serial.println(counts);
        if(counts == num){
          final_Stage();
        }else{
          myservo.write(center_Pos);
        }
      }
     }
      else {

        if(leftUL_dist < 11){
          myservo.write(60);
          Serial.println("Left ULTRAsonic detected and moving right, Left ultrasonic dist: ");
          Serial.println(leftUL_dist);
        }
        else if(rightUL_dist < 11){
          myservo.write(160);
          Serial.println("Right ULTRAsonic detected and moving left. Right ultrasonic dist: ");
          Serial.println(rightUL_dist);
        }

        else if(front_dist < 15 && front_dist != 0){
          Serial.print("Stopping, front dist is: ");
          Serial.println(front_dist);
          front_Stop();
        }
     
        else if(left_dist < 15 && left_dist != 0 && front_dist > 40){
          Serial.println("VERY NEAR TO LEFT");
          while(left_dist < 15 && front_dist > 40){
            get_Pings();
            pixy.ccc.getBlocks();
            Serial.print("Turning right, left dist is: ");
            Serial.println(left_dist);
            myservo.write(80);
            
          }
//          myservo.write(center_Pos);
        }
  
        else if(left_dist > 20 && left_dist < 25 && front_dist > 40){
          Serial.println("AWAY FROM LEFT");
          while(left_dist > 20 && front_dist > 40 && front_dist > 40){
            get_Pings();
            pixy.ccc.getBlocks();
            Serial.print("Turning left, left dist is: ");
            Serial.println(left_dist);
            myservo.write(130);  ////changed from 140 to 130 
            
          }
//          myservo.write(center_Pos);
        }

        else if (left_dist >= 25 && left_dist != 0 && front_dist > 40){
          Serial.println(" VERY VERY AWAY FROM LEFT");
          Serial.print("Turning left, left dist is: ");
          Serial.println(left_dist);
          while(left_dist >= 25 && left_dist < 50 && front_dist > 40){
            get_Pings();
            pixy.ccc.getBlocks();
            Serial.print("Turning left, left dist is: ");
            Serial.println(left_dist);
            myservo.write(120);  ////changed from 140 to 130
            
          }
        }
    
        else if(right_dist < 13 && right_dist != 0 && front_dist > 40){
          Serial.println("VERY NEAR TO RIGHT");
          while(right_dist < 15 && front_dist > 40){
            get_Pings();
            pixy.ccc.getBlocks();
            Serial.print("Turning left, right dist is: ");
            Serial.println(right_dist);
            myservo.write(140);
          }
//          myservo.write(center_Pos);
        }     
        
        else{
          myservo.write(center_Pos);
          Serial.println("Nothing detected..");
        }

        
      }
    }
  }
  
  analogWrite(enA, LOW);
  analogWrite(enB, LOW);
  
}


void move_Straight(){
  
  analogWrite(enA, sped);
  analogWrite(enB, sped);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 //delay(10);
}

void move_Reverse(){
  
  analogWrite(enA, sped);
  analogWrite(enB, sped);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 //delay(10);
}

void move_Slow(){
  
  analogWrite(enA, low_Sped);
  analogWrite(enB, low_Sped);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

}

int final_Stage(){
  
  int starttime, endtime, loopcount;
  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <=2000) // do this loop for up to 1000mS
  {
  // code here
  get_Pings();
    if (left_dist < 15 && left_dist != 0) {
      myservo.write(60);
    }
    else if (right_dist < 15 && right_dist != 0) {
      myservo.write(160);
    }
    else{
      myservo.write(center_Pos);
    }
      
    loopcount = loopcount+1;
    endtime = millis();
  }
  Serial.write(center_Pos);
  move_Reverse();
//  delay(500);
  period = 500;
  time_now = millis();
  while(millis()< time_now + period){
    //wait approx. period ms
  }
  //serial.print (loopcount,DEC);
}

void get_Pings(){
//    Serial.println("Beginning get_Pings() funciton");
    digitalWrite(trig1Pin, LOW);
    delayMicroseconds(2);
//    for(volatile int i = 0; i<1; i++){
//      asm(""); //...this is delay for 1.94 seconds
//    } 
    digitalWrite(trig1Pin, HIGH);
    delayMicroseconds(10);
//    for(volatile int i = 0; i<8; i++){
//    //This is delay for 10.25 microseconds
//    }
    digitalWrite(trig1Pin, LOW);
    left_duration = pulseIn(echo1Pin, HIGH);
    left_dist = left_duration * 0.034 / 2;
    if (left_dist > 400){
      left_dist = 0;
    }
  
    digitalWrite(trig2Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig2Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2Pin, LOW);
    right_duration = pulseIn(echo2Pin, HIGH);
    right_dist = right_duration * 0.034 / 2;
    if(right_dist > 400){
      right_dist = 0;
    }
  
    digitalWrite(trig3Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig3Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig3Pin, LOW);
    front_duration = pulseIn(echo3Pin, HIGH);
    front_dist = front_duration * 0.034 / 2;
    if(front_dist > 400){
      front_dist = 0;
    }

    digitalWrite(trig4Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig4Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig4Pin, LOW);
    leftUL_duration = pulseIn(echo4Pin, HIGH);
    leftUL_dist = leftUL_duration * 0.034 / 2;
    if(leftUL_dist > 400){
      leftUL_dist = 0;
    }
    
    digitalWrite(trig4Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig4Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig4Pin, LOW);
    rightUL_duration = pulseIn(echo4Pin, HIGH);
    rightUL_dist = rightUL_duration * 0.034 / 2;
    if(rightUL_dist > 400){
      rightUL_dist = 0;
    }
    

//    Serial.println("Ending get_Pings() function");

    return left_dist, right_dist, front_dist, leftUL_dist, rightUL_dist;
}

void front_Stop(){
  get_Pings();
  //    pixy.ccc.getBlocks();
  //    Serial.print("Front obstacle detect, Stopping the car.... ");
  //    Serial.print("   front dist: ");
  //    Serial.println(front_dist);
        counts = num;
  //    myservo.write(60);
        move_Reverse();
//        delay(500);
        period = 500;
        time_now = millis();
        while(millis()< time_now + period){
          //wait approx. period ms
        }
  //    myservo.write(center_Pos);
}
