double flow; //Liters of passing water volume
unsigned long pulse_freq;

void setup()
{
  pinMode(2, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, pulse, RISING); // Setup Interrupt
  
}

void loop ()
{
    flow = .00225 * pulse_freq;
    Serial.print(flow, DEC);
    Serial.println("L");
    delay(500);
  
}
void pulse () // Interrupt function

{
  pulse_freq++;
}
