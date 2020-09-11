#include<math.h>

#define pin 13
#define pi 3.141592653589793238462
#define frequency 50 //Take frequency as per your country/region 50 for Asia and Europe, 60 for North America
#define nano 1e-6
#define baud 9600
#define capacitor1 10
#define capacitor2 11

float phase;
float pf_max = 0;
float phase_max = 0;
float pf[3] = {pf[0], pf[1], pf[2]};
float a = min(pf[1], pf[2]);
float b = min(a, pf[3]);

void pf_calculate()
{
  for (int ctr = 0; ctr <= 4; ctr++)
  {
    phase = ((((pulseIn(pin, HIGH)) * nano) * frequency * 2) * pi);
    if (phase > phase_max)
    {
      phase_max = phase;
      pf_max = cos(phase_max);
    }
    if (phase_max > 2 * pi)
    {
      phase_max = 0;
      pf_max = 1;
    }
    if (phase_max == 0)
    {
      phase_max = 0;
      pf_max = 1;
    }
    Serial.print(phase_max);
    Serial.print("\n");
    Serial.print(pf_max);
  }
}
void cap1()
{
  digitalWrite(capacitor1, 1);
  digitalWrite(capacitor2, 0);
  pf_calculate();
  pf[0] = pf_max;
}
void cap2()
{
  digitalWrite(capacitor1, 0);
  digitalWrite(capacitor2, 1);
  pf_calculate();
  pf[1] = pf_max;
}
void cap3()
{
  digitalWrite(capacitor1, 1);
  digitalWrite(capacitor2, 1);
  pf_calculate();
  pf[2] = pf_max;
}


void pf_improve()
{
  if (pf_max <= 0.85 && sin(phase_max)>=0) //In most countries, the minimum power factor at which industries are allowed to draw power is 0.85 lagging any less and they are penaised. Lookup your power company's website and set this value accordingly 
  {                                        //sin(phase_max) allows us to know whether load is operating at a lagging power factor or leading power factor, turning on capacitors at leading power factor makes no sense
    if (pf[0] == b)
    {
      cap1();
    }
    if (pf[1] == b)
    {
      cap2();
    }
    if (pf[2] == b)
    {
      cap3();
    }
  }
}

void setup()
{
  pinMode(pin, INPUT);
  pinMode(capacitor1, OUTPUT);
  pinMode(capacitor2, OUTPUT);
  Serial.begin(baud);
}

void loop()
{
 pf_calculate();
 pf_improve();
}
