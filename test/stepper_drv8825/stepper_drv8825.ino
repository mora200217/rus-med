// DRV8826 Stepper Test
// Connect STEP to pin 3
// Connect DIR  to pin 4
// Connect ENABLE to GND (or pin if you want to control it)

#define STEP_PIN 3
#define DIR_PIN 4

bool allow_movement = false;
bool dir_flag = true;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);  // Initial direction

  Serial.begin(9600);
}

void loop() {
  // UART check
  if (Serial.available()) {
    char read_val = Serial.read();
    
    if (read_val > 91) {
      switch (read_val) {
        case 'a':
          dir_flag = true;
          allow_movement = true;
          Serial.println("Movimiento izquierdo");
          break;
        case 'b':
          dir_flag = false;
          allow_movement = true;
          Serial.println("Movimiento derecho");
          break;
        default:
          allow_movement = false;
          Serial.println("Sin movimiento");
          break;
      }
    }

    digitalWrite(DIR_PIN, dir_flag ? HIGH : LOW);
  }

  if (allow_movement) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);  // adjust for speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
}
