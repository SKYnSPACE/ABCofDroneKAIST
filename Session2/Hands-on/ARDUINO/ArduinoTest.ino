int redLEDPinNumber = 13;
int redLEDPinState = LOW;

int greenLEDPinNumber = 8;
int greenLEDPinState = LOW;

int switchPinNumber = 2;
int switchState = HIGH; 
// HIGH is the switched-off state due to a pull-up resistor.

unsigned long timer = millis();
const long interval = 1000; //[ms]

void setup() 
{
  Serial.begin(9600);
 
  pinMode(redLEDPinNumber, OUTPUT);
  pinMode(greenLEDPinNumber, OUTPUT);
  
  pinMode(switchPinNumber, INPUT_PULLUP);
  // Enable the internal pull-up resistor.
}

void loop()
{
  
  switchState = digitalRead(switchPinNumber);
  Serial.println(switchState);
  if (switchState == HIGH) 
  {
    digitalWrite(greenLEDPinNumber, LOW);
  } 
  else 
  {
    digitalWrite(greenLEDPinNumber, HIGH);
  }
  
  
  if ((timer + interval) < millis())
  {
    if (redLEDPinState == LOW)
    {
      redLEDPinState = HIGH;
    } 
    else
    {
      redLEDPinState = LOW;
    }
    digitalWrite(redLEDPinNumber, redLEDPinState);
    
    timer = millis();
  }
  
  
}
