#include <Wire.h>
#include <picobricks.h>   


#define TRIG_PIN   26
#define ECHO_PIN   27


#define IR_RX_PIN  17


#define SLOW       120
#define FAST       255

motorDriver motor;
IRPico     irReceiver(IR_RX_PIN);

uint32_t ir_code = 0;


float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return (duration > 0) ? (duration / 2.0) / 29.1 : -1;
}


void stopMotors() {
  motor.dc(1, 0, 0);
  motor.dc(2, 0, 0);
}
void forward() {
  motor.dc(1, FAST, 0);
  motor.dc(2, FAST, 0);
}
void backward() {
  motor.dc(1, FAST, 1);
  motor.dc(2, FAST, 1);
}
void leftTurn() {
  motor.dc(1, SLOW, 0);
  motor.dc(2, SLOW, 1);
}
void rightTurn() {
  motor.dc(1, SLOW, 1);
  motor.dc(2, SLOW, 0);
}

void setup() {
  Wire.begin();    
  Serial.begin(115200);           

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  
  float cm      = getDistance();
  bool  blocked = (cm > 0 && cm < 8.0);

  
  if (irReceiver.decode()) {
    ir_code = irReceiver.getCode();
    Serial.println(ir_code);
    if (ir_code == button_up) {
      if (!blocked) {
        forward();
        unsigned long start = millis();
        
        while (millis() - start < 5000) {
          float cm2 = getDistance();
          if (cm2 > 0 && cm2 < 8.0) break;
          delay(10);
        }
        stopMotors();
      } else {
        stopMotors();
      }
    }
    else if (ir_code == button_down) {
      backward();
      delay(5000);
      stopMotors();
    }
    else if (ir_code == button_left) {
      leftTurn();
      delay(5000);
      stopMotors();
    }
    else if (ir_code == button_right) {
      rightTurn();
      delay(5000);
      stopMotors();
    }
  }

  
  delay(10);
}
