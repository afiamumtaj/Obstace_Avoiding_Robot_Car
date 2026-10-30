#include <AFMotor.h>
#include <Servo.h>

Servo servo;

AF_DCMotor LF(1);
AF_DCMotor LB(2);
AF_DCMotor RF(3);
AF_DCMotor RB(4);

#define trigPin  A0  
#define echoPin  A1  

void setup() {
    servo.attach(10);  // Attach the servo
    pinMode(trigPin, OUTPUT);  // Declare "trigPin" as "Output Pin". 
    pinMode(echoPin, INPUT);   // Declare "echoPin" as "Input Pin". 
    Serial.begin(9600);        // Start serial communication
}

void loop() {
    float distance = search();  // Measure distance
    Serial.print("Distance = ");
    Serial.println(distance);

    // Adjust to avoid zero distance readings
    if (distance > 0) {
        if (distance <= 25) {
            RobotStop();
            delay(200);

            if (distance <= 30) {  // If obstacle found within 30 cm
                RobotStop();  // Robot stops
                delay(500);
                servo.write(5);  // Move servo to 5 degrees
                delay(500);
                
                float RightDistance = search();  // Measure right distance
                delay(100);
                servo.write(180);  // Move servo to 180 degrees
                delay(500);
                
                float LeftDistance = search();  // Measure left distance
                delay(100);
                servo.write(90);  // Reset servo to center position
                delay(300);

                // Decide which direction to turn based on distances
                if (LeftDistance > RightDistance) {
                    Motor_Speed(140);
                    RobotBackward();
                    delay(300);
                    RobotStop();
                    delay(100);
                    RobotLeft();
                    delay(500);
                    RobotStop();
                    delay(100);
                } else {
                    Motor_Speed(140);
                    RobotBackward();
                    delay(300);
                    RobotStop();
                    delay(100);
                    RobotRight();
                    delay(500);
                    RobotStop();
                    delay(100);             
                }
            }
        } else if (distance <= 60) {
            Motor_Speed(170);
            RobotForward();
        } else {
            Motor_Speed(200);
            RobotForward();
        }
    } else {
        Serial.println("Invalid reading");
        RobotStop(); // Stop the robot if distance is invalid
    }

    delay(100);  // Add a small delay before the next loop iteration
}

float search() {
    float duration = 0.00;
    float CM = 0.00;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
    CM = (duration / 58.82);  // Convert time to distance in cm

    // Return 0 if distance is invalid
    if (CM > 0 && CM < 400) {  // Range check
        return CM;
    } else {
        return 0;  // Invalid distance reading
    }
}

void RobotForward() {
    LF.run(FORWARD);
    RF.run(FORWARD);
    LB.run(FORWARD);
    RB.run(FORWARD);
}

void RobotBackward() {
    LF.run(BACKWARD);
    LB.run(BACKWARD);
    RF.run(BACKWARD);
    RB.run(BACKWARD);
}

void RobotLeft() {
    LF.run(BACKWARD);
    LB.run(BACKWARD);
    RF.run(FORWARD);
    RB.run(FORWARD);
}

void RobotRight() {
    LF.run(FORWARD);
    LB.run(FORWARD);
    RF.run(BACKWARD);
    RB.run(BACKWARD);
}

void RobotStop() {
    LF.run(RELEASE);
    LB.run(RELEASE);
    RF.run(RELEASE);
    RB.run(RELEASE);
}

void Motor_Speed(int mtrspd) {
    LF.setSpeed(mtrspd);
    LB.setSpeed(mtrspd);
    RF.setSpeed(mtrspd);
    RB.setSpeed(mtrspd);
}
