
//****************************************************************
//*  Name    : blink_serial                                      *
//*  Author  : Goldy Levi                                        *
//*  Date    : 19/03/18                                          *
//*  Version : 1.1                                               *
//*  Notes   : receives 0/1 through serial input and turns off/on *
//*            the LED                                           *
//****************************************************************

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  while (Serial.available() == 0) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.begin(9600);
}

void serialEvent()
{
   while(Serial.available()) 
   {
      int ch = Serial.read();
      Serial.write(ch);
      if (ch != -1) {
                  switch(ch) { 
                        case '0':                // turn off D13 when receiving "0"
                        digitalWrite(13, 0); 
                        break; 
                        case '1':                // turn on D13 when receiving "1" 
                        digitalWrite(13, 1); 
                        break; 
                  } 
            } 
   }
}
void loop() { 
}
