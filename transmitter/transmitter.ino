#include <VirtualWire.h>
#include <VirtualWire_Config.h>

int pwm1;
int pwm2;
int TX_PIN = 11;
int TX_ID = 3; 
 
typedef struct veriler
{
int    TX_ID;     
int    PWM1Data;
};
 
void setup() {
 Serial.begin(9600);
  pinMode(3,INPUT); 
 
  vw_setup(4000);
  vw_set_tx_pin(TX_PIN);
 
              }
void loop()
{
  pwm1=analogRead(3); 
  pwm1=map(pwm1,0,1023,0,255); 
 Serial.println(pwm1);

  struct veriler payload;
  
  
  payload.TX_ID = 3; 
  payload.PWM1Data =pwm1; 
 
  vw_send((uint8_t *)&payload, sizeof(payload)); 
  vw_wait_tx();
}
