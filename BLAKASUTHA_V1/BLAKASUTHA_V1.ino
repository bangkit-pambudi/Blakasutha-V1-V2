/*  PSX Controller Decoder Library (Psx.pde)
	Written by: Kevin Ahrendt June 22nd, 2008
	
	Controller protocol implemented using Andrew J McCubbin's analysis.
	http://www.gamesx.com/controldata/psxcont/psxcont.htm
	
	Shift command is based on tutorial examples for ShiftIn and ShiftOut
	functions both written by Carlyn Maw and Tom Igoe
	http://www.arduino.cc/en/Tutorial/ShiftIn
	http://www.arduino.cc/en/Tutorial/ShiftOut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Servo.h>
#include <Psx.h>                                          // Includes the Psx Library 
                                                          // Any pins can be used since it is done in software
#define dataPin 22
#define cmndPin 24
#define attPin 26
#define clockPin 28

#define pwm_kiri_atas1 2
#define pwm_kiri_atas2 3
#define pwm_kiri_bawah1 4
#define pwm_kiri_bawah2 5
#define pwm_kanan_bawah1 6
#define pwm_kanan_bawah2 7
#define pwm_kanan_atas1 8
#define pwm_kanan_atas2 9

#define relay1 49
#define relay2 51

#define p_servo1 52
#define p_servo2 38
#define p_servo3 48




Servo servo1, servo2, servo3;
Psx Psx;                                                  // Initializes the library

unsigned int data = 0;                                    // data stores the controller response

void setup()
{
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 10);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds.
                                                          // too small delay may not work (under 5)
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  
  pinMode(pwm_kiri_atas1, OUTPUT);
  pinMode(pwm_kiri_atas2, OUTPUT);
  pinMode(pwm_kiri_bawah1, OUTPUT);
  pinMode(pwm_kiri_bawah2, OUTPUT);
  pinMode(pwm_kanan_bawah1, OUTPUT);
  pinMode(pwm_kanan_bawah2, OUTPUT);
  pinMode(pwm_kanan_atas1, OUTPUT);
  pinMode(pwm_kanan_atas2, OUTPUT);

  servo1.attach(p_servo1,200,500);
  servo2.attach(p_servo2);
  servo3.attach(p_servo3);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);

  //check aervo
  /*while(true){
    servo3.write(0);
    delay(1000);
    servo3.write(180);
    delay(1000);
  }*/
  Serial.begin(9600);
}

const int pwm_speed = 220;

//Kiri Atas
void reset_kiri_atas(){
  analogWrite(pwm_kiri_atas1, 0);
  analogWrite(pwm_kiri_atas2, 0);
}

void kiri_atas_cw(int pwm){
  reset_kiri_atas();
  analogWrite(pwm_kiri_atas1, 0);
  analogWrite(pwm_kiri_atas2, pwm);
}

void kiri_atas_acw(int pwm){
  reset_kiri_atas();
  analogWrite(pwm_kiri_atas1, pwm);
  analogWrite(pwm_kiri_atas2, 0);
}
//-----------------------------------------------

//Kiri Bawah
void reset_kiri_bawah(){
  analogWrite(pwm_kiri_bawah1, 0);
  analogWrite(pwm_kiri_bawah2, 0);
}

void kiri_bawah_cw(int pwm){
  reset_kiri_bawah();
  analogWrite(pwm_kiri_bawah1, 0);
  analogWrite(pwm_kiri_bawah2, pwm);
}

void kiri_bawah_acw(int pwm){
  reset_kiri_bawah();
  analogWrite(pwm_kiri_bawah1, pwm);
  analogWrite(pwm_kiri_bawah2, 0);
}
//-----------------------------------------------

//Kanan Bawah
void reset_kanan_bawah(){
  analogWrite(pwm_kanan_bawah1, 0);
  analogWrite(pwm_kanan_bawah2, 0);
}

void kanan_bawah_cw(int pwm){
  reset_kanan_bawah();
  analogWrite(pwm_kanan_bawah1, 0);
  analogWrite(pwm_kanan_bawah2, pwm);
}

void kanan_bawah_acw(int pwm){
  reset_kanan_bawah();
  analogWrite(pwm_kanan_bawah1, pwm);
  analogWrite(pwm_kanan_bawah2, 0);
}
//-----------------------------------------------

//Kanan Atas
void reset_kanan_atas(){
  analogWrite(pwm_kanan_atas1, 0);
  analogWrite(pwm_kanan_atas2, 0);
}

void kanan_atas_cw(int pwm){
  reset_kanan_atas();
  analogWrite(pwm_kanan_atas1, 0);
  analogWrite(pwm_kanan_atas2, pwm);
}

void kanan_atas_acw(int pwm){
  reset_kanan_atas();
  analogWrite(pwm_kanan_atas1, pwm);
  analogWrite(pwm_kanan_atas2, 0);
}
//-----------------------------------------------



int integral = 0;
int tmp_data = 0;
void check_integral( int data){
  if(data != tmp_data){
    tmp_data = data;
    integral = 0;
  }
}

void loop()
{
  //check motor
  /*while(true)
  {
      kiri_atas_cw(200);
      kiri_bawah_cw(150);
      kanan_bawah_cw(100);
      kanan_atas_cw(50);
      delay(5000);
      kiri_atas_acw(200);
      kiri_bawah_acw(150);
      kanan_bawah_acw(100);
      kanan_atas_acw(50);
      delay(5000);
      }
  */
  data = Psx.read();                                      // Psx.read() initiates the PSX controller and returns
                                                          // the button data


  if(integral < pwm_speed){
    integral+= 10;
  }
  
  //check stick
  Serial.println(data);

  if(data == 4608){
    Serial.println("Solenoid mendorong shagai");
    digitalWrite(relay1, LOW);
  }else{
    digitalWrite(relay1, HIGH);
  }

  if(data == 5120){
    Serial.println("Solenoid mundur");
    digitalWrite(relay2, LOW);
  }else{
    digitalWrite(relay2, HIGH);
  }

  
  //-------------------------------------------
  //Control motor
  if(data == 8){
    Serial.println("Robot Maju");
    check_integral(data);
    kiri_atas_acw(integral + 10);
    kiri_bawah_acw(integral + 2);
    kanan_bawah_cw(integral + 5);
    kanan_atas_cw(integral - 2);
  }else
  if(data == 2){
    Serial.println("Robot Mundur");
    check_integral(data);
    kiri_atas_cw(integral);
    kiri_bawah_cw(integral + 7);
    kanan_bawah_acw(integral);
    kanan_atas_acw(integral);    
  }else
  if(data == 1){
    Serial.println("Robot Geser Kiri");
    check_integral(data);
    kiri_atas_cw(integral);
    kiri_bawah_acw(integral + 7);
    kanan_bawah_acw(integral + 5);
    kanan_atas_cw(integral); 
  }else
  if(data == 4){
    Serial.println("Robot Geser Kanan");
    check_integral(data);
    kiri_atas_acw(integral);
    kiri_bawah_cw(integral);
    kanan_bawah_cw(integral);
    kanan_atas_acw(integral); 
  }else
  if(data == 12){
    Serial.println("Robot Maju Serong Kanan");
    check_integral(data);
    reset_kiri_atas();
    reset_kiri_bawah();
    reset_kanan_bawah();
    reset_kanan_atas();
    kanan_bawah_cw(integral);
    kiri_atas_acw(integral);
  }else
  if(data == 9){
    Serial.println("Robot Maju Serong Kiri");
    check_integral(data);
    reset_kiri_atas();
    reset_kiri_bawah();
    reset_kanan_bawah();
    reset_kanan_atas();
    kiri_bawah_acw(integral);
    kanan_atas_cw(integral);
  }else
  if(data == 6){
    Serial.println("Robot Mundur Serong Kiri");
    check_integral(data);
    reset_kiri_atas();
    reset_kiri_bawah();
    reset_kanan_bawah();
    reset_kanan_atas();
    kiri_bawah_cw(integral);
    kanan_atas_acw(integral);
  }else
  if(data == 3){
    Serial.println("Robot Mundur Serong Kanan");
    check_integral(data);
    reset_kiri_atas();
    reset_kiri_bawah();
    reset_kanan_bawah();
    reset_kanan_atas();
    kanan_bawah_acw(integral);
    kiri_atas_cw(integral);
  }else
  if(data == 1024){
    Serial.println("Robot Putar Kanan");
    check_integral(data);
    kiri_atas_acw(pwm_speed - 170);
    kiri_bawah_acw(pwm_speed - 170);
    kanan_bawah_acw(pwm_speed - 170);
    kanan_atas_acw(pwm_speed - 170); 
  }else
  if(data == 256){
    Serial.println("Robot Putar Kiri");
    kiri_atas_cw(pwm_speed - 170);
    kiri_bawah_cw(pwm_speed - 170);
    kanan_bawah_cw(pwm_speed - 170);
    kanan_atas_cw(pwm_speed - 170);
  }else
  if(data == 512){
    Serial.println("Robot Putar Kanan");
    check_integral(data);
    kiri_atas_acw(integral);
    kiri_bawah_acw(integral);
    kanan_bawah_acw(integral);
    kanan_atas_acw(integral); 
  }else
  if(data == 2048){
    Serial.println("Robot Putar Kiri");
    check_integral(data);
    kiri_atas_cw(integral);
    kiri_bawah_cw(integral);
    kanan_bawah_cw(integral);
    kanan_atas_cw(integral);
  }else{
    reset_kiri_atas();
    reset_kiri_bawah();
    reset_kanan_bawah();
    reset_kanan_atas();
    integral = 0;
  }
  delay(10);
}
