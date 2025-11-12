const int buttonPin = 2;       
const int RLedPin = 9;         
const int GLedPin = 10;        
const int BLedPin = 11;        
int buttonState = 0;           

//按鈕狀態控制
bool buttonPressed = false;    
unsigned long pressingTime = 0;
const int longPressInterval = 2000; 

//顏色變化控制
int lightNum = 0;          
int RLightColor = 0;       
int GLightColor = 0;       
int BLightColor = 0;       
int RCurrentColor = 0;    
int GCurrentColor = 0;    
int BCurrentColor = 0;     

//模式控制
// 模式：0=靜態、1=固定閃爍、2=呼吸燈、3=七彩漸變、4=隨機七彩閃爍
int currentMode = 0;       
unsigned long blinkTimer = 0;  
const int fixedBlinkInterval = 500; 
bool blinkOn = true;           


const int fadeAmount = 2;  
int fadeDirection = 1;     


int huePhase = 0;          
const int hueSpeed = 2;    
unsigned long hueTimer = 0; 
const int hueInterval = 10; 

 
unsigned long randomTwinkleTimer = 0; 
const int twinkleSpeed = 5;          
int twinkleDirection = 0; 
int targetR = 0;
int targetG = 0;
int targetB = 0;

// -----------------------------------------------------

void setup() {
  pinMode(buttonPin, INPUT);     
  pinMode(RLedPin, OUTPUT);
  pinMode(GLedPin, OUTPUT);
  pinMode(BLedPin, OUTPUT);
 
  randomSeed(analogRead(0)); 
}

// -----------------------------------------------------

void loop() {
  checkButton();               
  updateLEDColor();            
  setRGBLEDColor(RCurrentColor, GCurrentColor, BCurrentColor); 
}


void checkButton() {
  buttonState = digitalRead(buttonPin);  

  if (buttonState == LOW && !buttonPressed) {  
    pressingTime = millis();                   
    buttonPressed = true;                      
  }

  if (buttonState == HIGH && buttonPressed) {  
    unsigned long currentTime = millis();      
    if (currentTime - pressingTime < longPressInterval) {
      changeLEDColor();  // 短按改變顏色
    } else {
      changeMode();      // 長按改變顯示模式
    }
    buttonPressed = false;                     
  }
}

// 短按時切換 LED 顏色
void changeLEDColor() {
  if (currentMode == 0 || currentMode == 1 || currentMode == 2) {
    lightNum = lightNum + 1;      
    if (lightNum >= 5) lightNum = 0;  

    if (lightNum == 0) { RLightColor = 0;   GLightColor = 0;   BLightColor = 0;   }   // 關燈
    if (lightNum == 1) { RLightColor = 0;   GLightColor = 255; BLightColor = 255; }   // 青色
    if (lightNum == 2) { RLightColor = 255; GLightColor = 0;   BLightColor = 255; }   // 紫色
    if (lightNum == 3) { RLightColor = 255; GLightColor = 255; BLightColor = 0;   }   // 黃色
    if (lightNum == 4) { RLightColor = 0;   GLightColor = 0;   BLightColor = 255; }   // 藍色

    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }
}

// 長按時切換模式
void changeMode() {
  currentMode = currentMode + 1;  
  if (currentMode >= 5) currentMode = 0;  // 總共 5 種模式 (0~4)

  // 模式 1：進入固定閃爍模式
  if (currentMode == 1) { 
    blinkTimer = 0;
    blinkOn = true;
  }

  // 模式 2：進入呼吸燈模式
  if (currentMode == 2) {
    fadeDirection = 1;
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }
  
  // 模式 3：進入七彩漸變模式
  if (currentMode == 3) {
    huePhase = 0;
    hueTimer = millis();
  }
  
  // 模式 4：進入隨機七彩閃爍模式 
  if (currentMode == 4) {
    randomTwinkleTimer = 0; 
    twinkleDirection = 0; 
    RCurrentColor = 0;
    GCurrentColor = 0;
    BCurrentColor = 0;
  }
}

void updateLEDColor() {
  unsigned long currentTime = millis();
  
  if (currentMode == 0) {
    // 模式 0：靜態顏色
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }

  else if (currentMode == 1) {
    // 模式 1：固定閃爍模式 
    if (currentTime - blinkTimer > fixedBlinkInterval) {
      blinkOn = !blinkOn;    
      blinkTimer = currentTime; 
    }

    if (blinkOn) {  
      RCurrentColor = RLightColor;
      GCurrentColor = GLightColor;
      BCurrentColor = BLightColor;
    } else {      
      RCurrentColor = 255; 
      GCurrentColor = 255; 
      BCurrentColor = 255; 
    }
  }

  else if (currentMode == 2) {
    // 模式 2：呼吸燈模式
    bool negativeDir = false;  

    if (RLightColor == 0) {
      RCurrentColor = RCurrentColor + fadeDirection * fadeAmount;
      if (RCurrentColor > 255 || RCurrentColor < 0) negativeDir = true;
      RCurrentColor = constrain(RCurrentColor, 0, 255);
    }

    if (GLightColor == 0) {
      GCurrentColor = GCurrentColor + fadeDirection * fadeAmount;
      if (GCurrentColor > 255 || GCurrentColor < 0) negativeDir = true;
      GCurrentColor = constrain(GCurrentColor, 0, 255);
    }

    if (BLightColor == 0) {
      BCurrentColor = BCurrentColor + fadeDirection * fadeAmount;
      if (BCurrentColor > 255 || BCurrentColor < 0) negativeDir = true;
      BCurrentColor = constrain(BCurrentColor, 0, 255);
    }

    if (negativeDir) fadeDirection = -fadeDirection;
    delay(10);  
  }
  
  else if (currentMode == 3) {
    // 模式 3：七彩漸變模式 
    if (currentTime - hueTimer >= hueInterval) {
      
      huePhase = (huePhase + hueSpeed) % 1530; 
      
      if (huePhase < 255) { RCurrentColor = 255; GCurrentColor = huePhase; BCurrentColor = 0; }
      else if (huePhase < 510) { RCurrentColor = 255 - (huePhase - 255); GCurrentColor = 255; BCurrentColor = 0; }
      else if (huePhase < 765) { RCurrentColor = 0; GCurrentColor = 255; BCurrentColor = huePhase - 510; }
      else if (huePhase < 1020) { RCurrentColor = 0; GCurrentColor = 255 - (huePhase - 765); BCurrentColor = 255; }
      else if (huePhase < 1275) { RCurrentColor = huePhase - 1020; GCurrentColor = 0; BCurrentColor = 255; }
      else { RCurrentColor = 255; GCurrentColor = 0; BCurrentColor = 255 - (huePhase - 1275); }
      
      hueTimer = currentTime;
    }
  }
  
  else if (currentMode == 4) {
    // 模式 4：隨機七彩閃爍模式 
    
    if (twinkleDirection == 0) {
        if (random(0, 100) < 5) { 
            twinkleDirection = 1; 
          
            targetR = random(50, 256); 
            targetG = random(50, 256); 
            targetB = random(50, 256); 
           
            RCurrentColor = 0;
            GCurrentColor = 0;
            BCurrentColor = 0;
        }
    }

    if (twinkleDirection != 0 && (currentTime - randomTwinkleTimer >= 10)) { 
      
        if (twinkleDirection == 1) { 
            if (RCurrentColor < targetR) RCurrentColor += twinkleSpeed;
            if (GCurrentColor < targetG) GCurrentColor += twinkleSpeed;
            if (BCurrentColor < targetB) BCurrentColor += twinkleSpeed;
        } else { 
            RCurrentColor -= twinkleSpeed;
            GCurrentColor -= twinkleSpeed;
            BCurrentColor -= twinkleSpeed;
        }
       
        RCurrentColor = constrain(RCurrentColor, 0, 255);
        GCurrentColor = constrain(GCurrentColor, 0, 255);
        BCurrentColor = constrain(BCurrentColor, 0, 255);
        
        if (twinkleDirection == 1 && (RCurrentColor >= targetR && GCurrentColor >= targetG && BCurrentColor >= targetB)) {
             delay(random(30, 80)); 
             twinkleDirection = -1; 
        }
         
        if (twinkleDirection == -1 && RCurrentColor == 0 && GCurrentColor == 0 && BCurrentColor == 0) {
            twinkleDirection = 0; 
        }
        
        randomTwinkleTimer = currentTime; 
    }
  }
}

void setRGBLEDColor(int r, int g, int b) {
  analogWrite(RLedPin, r);  
  analogWrite(GLedPin, g);  
  analogWrite(BLedPin, b);  
}