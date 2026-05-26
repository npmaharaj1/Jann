const int leftMotorForward = 2;
const int leftMotorBackward = 3;

const int rightMotorForward = 10; // 4
const int rightMotorBackward = 11; // 5
const int rightMotorScaler = 255;
int rightMotorTurn = 55;

const int sonarNorthEcho = 6;
const int sonarNorthTrigger = 7;
float sonarNorthDistance;

const int sonarEastEcho = 8;
const int sonarEastTrigger = 9;
float sonarEastDistance;

const float mazeWidth = 18.1;
const float roverWidth = 13.2;
const float targetFreeSpaceOneSide = (mazeWidth - roverWidth)/2;

void setup() {
  Serial.begin(9600);

  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);

  pinMode(sonarNorthTrigger, OUTPUT);
  pinMode(sonarNorthEcho, INPUT);
  pinMode(sonarEastTrigger, OUTPUT);
  pinMode(sonarEastEcho, INPUT);

  digitalWrite(sonarNorthTrigger, LOW);
  digitalWrite(sonarEastTrigger, LOW);

  delay(5000);
}

void resetWheels() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  analogWrite(rightMotorForward, 0);
  analogWrite(rightMotorBackward, 0);
}

float getDistance(int targetTrigger, int targetEcho) {
  digitalWrite(targetTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(targetTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(targetTrigger, LOW);

  return ((pulseIn(targetEcho, HIGH)) * 0.0343) / 2;
}

void pivotRight(int degree) {
  resetWheels();
  for (int i = 0; i < (degree/45); i++) {
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorBackward, rightMotorScaler);
    delay(650);
  }
  resetWheels();
}

void pivotLeft(int degree) {
  resetWheels();
  for (int i = 0; i < (degree/45); i++) {
    digitalWrite(leftMotorBackward, HIGH);
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(670);
  }
  resetWheels();
}

void findMinEastLeft(float previous) {
  float sonarEastDistance;

  previous = previous;

  resetWheels();
  delay(500);
  analogWrite(rightMotorForward, rightMotorScaler);
  digitalWrite(leftMotorBackward, HIGH);
  delay(130);
  resetWheels();
  sonarEastDistance = getDistance(sonarEastTrigger, sonarEastEcho);
  delay(130);
  
  if (sonarEastDistance < previous || sonarEastDistance < 0.8) {
    findMinEastLeft(sonarEastDistance);
  } else {
    if (sonarEastDistance > 1) {  
      resetWheels();
      digitalWrite(leftMotorForward, HIGH);
      analogWrite(rightMotorBackward, rightMotorScaler);
      delay(110);
      resetWheels();
    }
  }
}

void findMinNorthLeft(float previous) {
  float sonarNorthDistance;

  previous = previous;

  resetWheels();
  delay(500);
  analogWrite(rightMotorForward, rightMotorScaler);
  digitalWrite(leftMotorBackward, HIGH);
  delay(200);
  resetWheels();
  sonarNorthDistance = getDistance(sonarNorthTrigger, sonarNorthEcho);
  delay(200);
  
  if (sonarNorthDistance < previous || sonarNorthDistance < 0.5) {
    findMinNorthLeft(sonarNorthDistance);
  } else {
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorBackward, rightMotorScaler);
    delay(200);
    resetWheels();
  }
}

void loop() {

  delay(50);

  sonarNorthDistance = getDistance(sonarNorthTrigger, sonarNorthEcho);
  sonarEastDistance = getDistance(sonarEastTrigger, sonarEastEcho);

  if (sonarEastDistance > 450) {
    sonarEastDistance = 0;
  }

  Serial.println();
  Serial.print("DistanceNorth: ");
  Serial.println(sonarNorthDistance);
  Serial.print("DistanceEast: ");
  Serial.println(sonarEastDistance);

  delay(20);

  if (sonarNorthDistance < 9) {
    if (sonarEastDistance < 3.7) {
      delay(350);
      findMinEastLeft(999);
      resetWheels();
      digitalWrite(leftMotorBackward, HIGH);
      analogWrite(rightMotorForward, rightMotorScaler);
      delay(780);
      resetWheels();
      sonarEastDistance = getDistance(sonarEastTrigger, sonarEastEcho);
      findMinEastLeft(999);
      while (sonarNorthDistance > 21) {
        digitalWrite(leftMotorForward, HIGH);
        analogWrite(rightMotorForward, rightMotorScaler);
        delay(140);
        sonarNorthDistance = getDistance(sonarNorthTrigger, sonarNorthEcho);
      }
    } else { 
      resetWheels();

      digitalWrite(leftMotorBackward, HIGH);
      analogWrite(rightMotorBackward, rightMotorScaler);

      delay(175);

      resetWheels();

      
      findMinEastLeft(999);
    }
  } else if (sonarEastDistance < 0.9) {
    resetWheels();
    analogWrite(rightMotorBackward, rightMotorScaler);
    digitalWrite(leftMotorBackward, HIGH);
    delay(900);
    resetWheels();
    digitalWrite(leftMotorBackward, HIGH);
    delay(700);
    resetWheels();
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(250);
    digitalWrite(leftMotorForward, HIGH);
    delay(200);
    resetWheels();
  } else if (sonarEastDistance > 18.6 && sonarEastDistance < 38) {
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorForward, rightMotorTurn);
    delay(100);
    resetWheels();
    delay(10);
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorForward, rightMotorTurn - 9);
    delay(115);
  } else if (sonarEastDistance > 3.7 && sonarEastDistance < 40) {
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorForward, rightMotorTurn);
    delay(100);
  } else if (sonarEastDistance > 49) {
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(300);
  } else if (sonarEastDistance > 39.2) {
    resetWheels();
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(200);
    resetWheels();
    analogWrite(rightMotorBackward, rightMotorScaler);
    delay(350);
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    delay(300);
  } else if (sonarEastDistance < 2.7 && sonarEastDistance > 0.6) {
    resetWheels();
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(230);
    
  } else {
    resetWheels();
    digitalWrite(leftMotorForward, HIGH);
    analogWrite(rightMotorForward, rightMotorScaler);
    delay(10);
  }
}