
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

#define LEDPIN 24
#define FANPIN 26
#define PUMPPIN 28
#define HUMPIN 30


#define button_light 37
#define button_humidity 39
#define button_pump 41

int button_light_state;
int button_humidity_state;
int button_pump_state;



const int motorIn1=32;
const int motorIn2=34;
const int ENA= 3;
int speed;
bool bool_light;
bool bool_hum;
bool bool_pump;


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);
  dht.begin();
  
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  //pinMode(ENA, OUTPUT);

  pinMode(LEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(PUMPPIN, OUTPUT);
  pinMode(HUMPIN, OUTPUT);

  pinMode(button_light, INPUT);
  pinMode(button_humidity, INPUT);
  pinMode(button_pump, INPUT);

  

  
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(FANPIN, HIGH);
  digitalWrite(PUMPPIN, HIGH);
  digitalWrite(HUMPIN, HIGH);

  speed=10;
  bool_light=false;
  bool_hum=false;;
  bool_pump=false;;

  
}

void loop() {
  // Wait a few seconds between measurements.
        analogWrite(ENA,speed);


  button_light_state = digitalRead(button_light);
  
  if(button_light_state==HIGH){
    digitalWrite(LEDPIN, LOW);
    bool_light=true;
  }else{
    digitalWrite(LEDPIN, HIGH);
    bool_light=false;
  }






  button_pump_state = digitalRead(button_pump);

  if(button_pump_state==HIGH){
    digitalWrite(PUMPPIN, LOW);
    bool_pump=true;
  }else{
    digitalWrite(PUMPPIN, HIGH);
    bool_pump=false;
  }


  
  delay(1000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    //return;
  }else{
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    Serial.print("{ ");
    Serial.print(" \"Humidity\" : ");
    Serial.print(h);
    Serial.println(" }");
    delay(200);
    Serial.print("{ \"Temperature\" : ");
    Serial.print(hic);
    Serial.println(" }");
    
    lcd.clear();
    lcd.print("Humid ");
    lcd.print("Temp");   
    lcd.setCursor(0, 1);
    lcd.print(h);
    lcd.print(" ");
    lcd.print(hic);
    lcd.print("*C");

    
    button_humidity_state = digitalRead(button_humidity);
  
    if(button_humidity_state==HIGH){
      digitalWrite(FANPIN, LOW);
      bool_hum=true;
    }else{
      
      bool_hum=false;
    }



    if(bool_hum==false){
      if(h<70){
        digitalWrite(FANPIN, LOW);
      
        digitalWrite(motorIn1,LOW);
        digitalWrite(motorIn2,LOW);
        delay(500);
      }
      else if (h>80 || hic>26){
        digitalWrite(FANPIN, HIGH);
        
        digitalWrite(motorIn1,HIGH);
        digitalWrite(motorIn2,LOW);
        delay(500);
        
      }
        
      
      
      else{
          
        digitalWrite(motorIn1,LOW);
        digitalWrite(motorIn2,LOW);
        delay(500);
      }
    }

  }
  


  delay(2000);
  lcd.clear();
  

  int light = analogRead(A1);
  Serial.print("{ \"Light\" : ");
  Serial.print(light);
  Serial.println(" }");
  lcd.print("Light: ");
  lcd.setCursor(0,1);

  if(bool_light==false){
    if(light<500){
      lcd.print("Yes Sunlight");
      digitalWrite(LEDPIN, HIGH);
    }else{
      lcd.print("No Sunlight");
      digitalWrite(LEDPIN, LOW);
    }
  }
  delay(2000);
  lcd.clear();
  

  int soil = analogRead(A2);
  Serial.print("{ \"Soil\" : ");
  Serial.print(soil);
  Serial.println(" }");
  lcd.print("Water: ");
  lcd.setCursor(0,1);

  if (bool_pump==false){
    if(soil<500){
      lcd.print("Yes!");
      digitalWrite(PUMPPIN, HIGH);
    }else{
      lcd.print("No!");
      
      digitalWrite(PUMPPIN, LOW);
      delay(5000);
      digitalWrite(PUMPPIN, HIGH);
    }
  } 
     // speed+=40;
     // if (speed>=240){
       //  speed=100;
      // }

  

}

