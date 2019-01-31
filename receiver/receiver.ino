#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include <SoftwareSerial.h>

int RX_PIN = 3;
int RX_ID = 3;
int TX_ID;
typedef struct veriler 
{
  int    TX_ID;      
  int    PWM1Data;
 
 
};
void setup() {
 Serial.begin(9600);
  pinMode(8,OUTPUT); 
  vw_setup(4000);
  vw_set_rx_pin(RX_PIN);
  vw_rx_start();
 
}
void loop()
{
  struct veriler alinan_veri; 
  uint8_t data_boyutu = sizeof(alinan_veri);
  vw_wait_rx();// data okuması başlar
  boolean v = vw_get_message((uint8_t *)&alinan_veri, &data_boyutu);
  
  Serial.println(v);

    if (alinan_veri.TX_ID == RX_ID) 
    {
      if(alinan_veri.PWM1Data!=0){
        Serial.println("Motion detected!");
        digitalWrite(8,1);
        playTone(3000, 150);
        delay(150);
      }
      else{
       Serial.println("Motion ended!");
       digitalWrite(8,0);
       playTone(0, 0);
       delay(300);
      }
    }
  }


void playTone(long duration, int freq) 
{
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) 
    {
    digitalWrite(8,HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(8, LOW);
    delayMicroseconds(period / 2);
    elapsed_time += (period);
   }
}
