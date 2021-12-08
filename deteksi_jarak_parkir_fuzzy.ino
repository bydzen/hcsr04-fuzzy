// Calling fuzzy library
#include <Fuzzy.h>

// Create fuzzy objects
Fuzzy *fuzzy = new Fuzzy();

// Ultrasonic Sensor (HC-SR04)
int triggerPIN = 6;
int echoPIN = 7;

// LED
int ledRedPIN = 2;
int ledOrangePIN = 3;
int ledGreenPIN = 4;

// Buzzer
int buzzerPIN = 8;

void setup() {
  // Sensor Ultrasonic (HC-SR04)
  pinMode(triggerPIN, OUTPUT);
  pinMode(echoPIN, INPUT);

  // LED
  pinMode(ledRedPIN, OUTPUT);
  pinMode(ledOrangePIN, OUTPUT);
  pinMode(ledGreenPIN, OUTPUT);

  // Serial begin passes the value 9600 to the speed parameter
  Serial.begin(9600);

  // Initialization of the fuzzy input with the distance variable.
  FuzzyInput *distance = new FuzzyInput(1);

  // Setting fuzzy with distance range on each variable
  FuzzySet *sangatDekat = new FuzzySet(0, 0, 10, 20);
  FuzzySet *dekat = new FuzzySet(10, 20, 30, 40);
  FuzzySet *sedang = new FuzzySet(30, 40, 50, 60);
  FuzzySet *jauh = new FuzzySet(50, 60, 70, 80);
  FuzzySet *sangatJauh = new FuzzySet(70, 80, 100, 100);

  // Adding each variable into the input distance
  distance->addFuzzySet(sangatDekat);
  distance->addFuzzySet(dekat);
  distance->addFuzzySet(sedang);
  distance->addFuzzySet(jauh);
  distance->addFuzzySet(sangatJauh);

  // Adding distance as fuzzy input
  fuzzy->addFuzzyInput(distance);

  // Initialization of the fuzzy output with the delaySpeed variable.
  FuzzyOutput *delaySpeed = new FuzzyOutput(1);

  // Setting fuzzy with delay speed range on each variable
  FuzzySet *d125 = new FuzzySet(0, 0, 62.5, 125);
  FuzzySet *d250 = new FuzzySet(115, 160, 205, 250);
  FuzzySet *d500 = new FuzzySet(240, 326.7, 413.3, 500);
  FuzzySet *d750 = new FuzzySet(490, 576.7, 663.3, 750);
  FuzzySet *d1000 = new FuzzySet(740, 826.7, 913.3, 1000);

  // Adding each variable into the output delaySpeed
  delaySpeed->addFuzzySet(d125);
  delaySpeed->addFuzzySet(d250);
  delaySpeed->addFuzzySet(d500);
  delaySpeed->addFuzzySet(d750);
  delaySpeed->addFuzzySet(d1000);

  // Adding delaySpeed as fuzzy output
  fuzzy->addFuzzyOutput(delaySpeed);

  // Added the first fuzzy rule for very close range.
  FuzzyRuleAntecedent *ifDistanceSangatDekat = new FuzzyRuleAntecedent();
  ifDistanceSangatDekat->joinSingle(sangatDekat);
  FuzzyRuleConsequent *thenSpeedD125 = new FuzzyRuleConsequent();
  thenSpeedD125->addOutput(d125);
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifDistanceSangatDekat, thenSpeedD125);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Added the second fuzzy rule for close range.
  FuzzyRuleAntecedent *ifDistanceDekat = new FuzzyRuleAntecedent();
  ifDistanceDekat->joinSingle(dekat);
  FuzzyRuleConsequent *thenSpeedD250 = new FuzzyRuleConsequent();
  thenSpeedD250->addOutput(d250);
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifDistanceDekat, thenSpeedD250);
  fuzzy->addFuzzyRule(fuzzyRule02);
  
  // Added the third fuzzy rule for medium range.
  FuzzyRuleAntecedent *ifDistanceSedang = new FuzzyRuleAntecedent();
  ifDistanceSedang->joinSingle(sedang);
  FuzzyRuleConsequent *thenSpeedD500 = new FuzzyRuleConsequent();
  thenSpeedD500->addOutput(d500);
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifDistanceSedang, thenSpeedD500);
  fuzzy->addFuzzyRule(fuzzyRule03);
  
  // Added the fourth fuzzy rule for far range.
  FuzzyRuleAntecedent *ifDistanceJauh = new FuzzyRuleAntecedent();
  ifDistanceJauh->joinSingle(jauh);
  FuzzyRuleConsequent *thenSpeedD750 = new FuzzyRuleConsequent();
  thenSpeedD750->addOutput(d750);
  FuzzyRule *fuzzyRule04 = new FuzzyRule(4, ifDistanceJauh, thenSpeedD750);
  fuzzy->addFuzzyRule(fuzzyRule04);
  
  // Added the fifth fuzzy rule for very far range.
  FuzzyRuleAntecedent *ifDistanceSangatJauh = new FuzzyRuleAntecedent();
  ifDistanceSangatJauh->joinSingle(sangatJauh);
  FuzzyRuleConsequent *thenSpeedD1000 = new FuzzyRuleConsequent();
  thenSpeedD1000->addOutput(d1000);
  FuzzyRule *fuzzyRule05 = new FuzzyRule(5, ifDistanceSangatJauh, thenSpeedD1000);
  fuzzy->addFuzzyRule(fuzzyRule05);
}

void loop()
{
  // Sensor Ultrasonic (HC-SR04)
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse
  digitalWrite(triggerPIN, LOW);
  delayMicroseconds(2);
  //delay(2);
  digitalWrite(triggerPIN, HIGH);
  delayMicroseconds(10);
  //delay(10);
  digitalWrite(triggerPIN, LOW);

  // Duration and distane
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending of
  // the ping to the reception of its echo off an object
  int duration = pulseIn(echoPIN, HIGH);

  // Calculate the distance (in centimeters)
  // Convert the time into a distance
  // Save value to inputFuzzy
  int inputFuzzy = (duration/2.0) / 29.1;

  // Print the distance on the Serial Monitor
  Serial.print("Distance    : ");
  Serial.println(inputFuzzy);

  // Set the inputFuzzy
  fuzzy->setInput(1, inputFuzzy);

  // Doing fuzzification
  fuzzy->fuzzify();

  // Getting the fuzzy output with defuzzification
  float outputFuzzy = fuzzy->defuzzify(1);

  // Turns off the LED and tone when the distance is more than 100 cm
  if (inputFuzzy > 100) {
    digitalWrite(ledRedPIN, LOW);
    digitalWrite(ledOrangePIN, LOW);
    digitalWrite(ledGreenPIN, LOW);
    noTone(buzzerPIN);
    Serial.print("Delay       : ");
    Serial.println(1000);
    Serial.println("Buzzer      : Off");
    Serial.println();
    delay(1000);
  // Informing that distance is very dangerous
  // With constant red LED and buzzer sound
  } else if (inputFuzzy <= 20) {
    tone(buzzerPIN, 1500, 999);
    digitalWrite(ledRedPIN, HIGH);
    digitalWrite(ledOrangePIN, LOW);
    digitalWrite(ledGreenPIN, LOW);
    Serial.print("Delay       : ");
    Serial.println(outputFuzzy);
    Serial.println("Buzzer      : Continously");
    Serial.println();
    delay(125);
  // Conditions when inputFuzzy is below 100 cm and above 20 cm
  } else {
    // Displays flashing red and orange LEDs
    if (inputFuzzy <= 40) {
      digitalWrite(ledRedPIN, HIGH);
      digitalWrite(ledOrangePIN, HIGH);
      digitalWrite(ledGreenPIN, LOW);
    // Displays flashing orange LED
    } else if (inputFuzzy <= 60) {
      digitalWrite(ledRedPIN, LOW);
      digitalWrite(ledOrangePIN, HIGH);
      digitalWrite(ledGreenPIN, LOW);
    // Displays flashing orange and green LEDs
    } else if (inputFuzzy <= 80) {
      digitalWrite(ledRedPIN, LOW);
      digitalWrite(ledOrangePIN, HIGH);
      digitalWrite(ledGreenPIN, HIGH);
    // Displays flashing green LED  
    } else {
      digitalWrite(ledRedPIN, LOW);
      digitalWrite(ledOrangePIN, LOW);
      digitalWrite(ledGreenPIN, HIGH);
    }

    // Sounds a buzzer with a frequency of 1.5khz 
    // and the duration is half of the outputFuzzy
    tone(buzzerPIN, 1500, outputFuzzy / 2);

    // First delay (half of outputFuzzy) to flash all LEDs
    delay(outputFuzzy / 2);

    // Turn off all LEDs to flash
    digitalWrite(ledRedPIN, LOW);
    digitalWrite(ledOrangePIN, LOW);
    digitalWrite(ledGreenPIN, LOW);

    // Second delay (half of outputFuzzy) to flash all LEDs
    delay(outputFuzzy / 2);

    // Print the delay time from the outputFuzzy
    Serial.print("Delay       : ");
    Serial.println(outputFuzzy);
    Serial.println("Buzzer      : On");
    Serial.println();
  }
}
