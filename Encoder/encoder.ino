#define outputA 6 //Signal A
#define outputB 7 //Signal B


int counter=0;
int aState;
int aLastState;


void setup() {
 pinMode(outputA,OUTPUT);
 pinMode(outputB,OUTPUT);
 Serial.begin(9600);

 aLastState=digitalRead(outputA);

}

void loop() {
  aState=digitalRead(outputA);
  if(aState!=aLastState){
      if(digitalRead(outputB) != aState){
        counter++;
      }else{
        counter--;
      }
      aLastState=aState;
  }
  Serial.print("Encoder position: ");
  Serial.println(counter);
  delay(60);
}
