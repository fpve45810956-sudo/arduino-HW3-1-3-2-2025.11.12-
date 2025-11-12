const int LED_PIN = 3;      
const int BUTTON_PIN = 2;   
int ledMode = 0; 

int buttonState = 0;        
int lastButtonState = LOW;  
long lastDebounceTime = 0;  
long debounceDelay = 50;    

long lastFlashTime = 0;    
int ledState = LOW;        

const int SLOW_DELAY = 500;
const int MEDIUM_DELAY = 250;
const int FAST_DELAY = 100;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600); 
  Serial.println("Arduino LED 控制啟動!");
  Serial.println("當前模式: 0 - 恆亮");

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  handleButtonPress();
  switch (ledMode) {
    case 0: // 恆亮 (Initial State: 衡亮)
      digitalWrite(LED_PIN, HIGH);
      break;
    case 1: // 慢閃爍 (Slow Flash)
      flashLED(SLOW_DELAY);
      break;
    case 2: // 不快不慢的閃爍 (Medium Flash)
      flashLED(MEDIUM_DELAY);
      break;
    case 3: // 快閃爍 (Fast Flash)
      flashLED(FAST_DELAY);
      break;

    default:
      digitalWrite(LED_PIN, LOW); 
      break;
  }
}

void handleButtonPress() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
       ledMode = (ledMode + 1) % 4;
        ledState = LOW; 
        digitalWrite(LED_PIN, LOW);
        lastFlashTime = millis(); 

        Serial.print("切換模式: ");
        Serial.println(ledMode);
      }
    }
  }
 
  lastButtonState = reading;
}

void flashLED(int delayTime) {
  if (millis() - lastFlashTime >= delayTime) {
    lastFlashTime = millis(); 

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
   
    digitalWrite(LED_PIN, ledState);
  }
}
