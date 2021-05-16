int ec_pin = 34;
int ec_power = 15;
int ec_value;
int sensorValue;

const int nutrient_pump_pin1 = 33;
const int nutrient_pump_pin2 = 32;
const int water_pump_pin = 25;

float slope_ec_graph = 0.0077;
float y_intercept_ec_graph = 3.738;

int floatswitch = 4;
int floatswitch_value;

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "x8NfYL6Ahkw81gestIpyBxTOKdK_TaWi";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Tenda_1F4450";
char pass[] = "12345678";

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(ec_pin, INPUT);
  pinMode(ec_power, OUTPUT);

  pinMode(nutrient_pump_pin1, OUTPUT);
  digitalWrite(nutrient_pump_pin1, HIGH);

  pinMode(nutrient_pump_pin2, OUTPUT);
  digitalWrite(nutrient_pump_pin2, HIGH);

  pinMode(water_pump_pin, OUTPUT);
  digitalWrite(water_pump_pin, LOW); //LOW IS CLOSE

  pinMode(floatswitch, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop()
{
  Blynk.run();
  water_pump_and_monitor();
  get_ec();
  delay (6000);
  pump_nutrient(nutrient_pump_pin1);
  pump_nutrient(nutrient_pump_pin2);
  delay (1000);
}


void get_ec()
{
  digitalWrite(ec_power, HIGH);
  sensorValue = analogRead(ec_pin);
  sensorValue = analogRead(ec_pin);
  ec_value = ((1000 / sensorValue) - y_intercept_ec_graph) / slope_ec_graph;
  Serial.print("Sensor Value:");
  Serial.println(sensorValue);
  Serial.print("EC Value:");
  Serial.println(ec_value);
  digitalWrite(ec_power, LOW);

}



void pump_nutrient(const int pump_pin) {
  int target_value = 1200;
  int change_per_dose = 100;
  int error = 100;
  int pump_on_time = 2;
  int wait_time = 5;

  if (ec_value <= (target_value - change_per_dose - error))
  {
    digitalWrite(pump_pin, LOW);
    Serial.print("Nutrients Too Low, Pumping_Nutrients");
    Serial.print('\n');
    delay(pump_on_time * 1000);
    digitalWrite(pump_pin, HIGH);
    Serial.print("Nutrients Pumping Finished");
    Serial.print('\n');
    delay(wait_time * 1000);
  }
}


void water_pump_and_monitor() {

  floatswitch_value = digitalRead(floatswitch);
  floatswitch_value = digitalRead(floatswitch);
  delay(1000);
  Serial.print("Water Level:");
  Serial.println(floatswitch_value);
  if ( floatswitch_value == 0 )
  { Serial.print("Water Level Too Low, Please Add Water Till Water Mark");
    digitalWrite(water_pump_pin, HIGH); // HIGH is off
    Serial.print('\n');
  } else {
    Serial.print("Water Level is ok");
    digitalWrite(water_pump_pin, LOW);
    Serial.print('\n');

  }
}
