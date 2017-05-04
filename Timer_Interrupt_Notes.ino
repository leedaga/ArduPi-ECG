/*
  These notes put together by Joel Murphy for Pulse Sensor Amped, 2015
  The code that this section is attached to uses a timer interrupt
  to sample the Pulse Sensor with consistent and regular timing.
  The code is setup to read Pulse Sensor signal at 500Hz (every 2mS).
  The reasoning for this can be found here:
  http://pulsesensor.com/pages/pulse-sensor-amped-arduino-v1dot1
  
  There are issues with using different timers to control the Pulse Sensor sample rate.
  Sometimes, user will need to switch timers for access to other code libraries.
  Also, some other hardware may have different timer setup requirements. This page
  will cover those different needs and reveal the necessary settings. There are two
  part of the code that will be discussed. The interruptSetup() routine, and
  the interrupt function call. Depending on your needs, or the Arduino variant that you use,
  check below for the correct settings.
  
  
  ******************************************************************************************
  ARDUINO UNO, Pro 328-5V/16MHZ, Pro-Mini 328-5V/16MHz (or any board with ATmega328P running at 16MHz)
  
 >> Timer2
  
    Pulse Sensor Arduino UNO uses Timer2 by default.
    Use of Timer2 interferes with PWM on pins 3 and 11.
    There is also a conflict with the Tone library, so if you want tones, use Timer1 below.
    
      void interruptSetup(){     
        // Initializes Timer2 to throw an interrupt every 2mS.
        TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
        TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
        OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
        TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
        sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
      } 
  
    use the following interrupt vector with Timer2
    
      ISR(TIMER2_COMPA_vect)
      
 >> Timer1
    
    Use of Timer1 interferes with PWM on pins 9 and 10.
    The Servo library also uses Timer1, so if you want servos, use Timer2 above.
    
      void interruptSetup(){     
        // Initializes Timer1 to throw an interrupt every 2mS.
        TCCR1A = 0x00; // DISABLE OUTPUTS AND PWM ON DIGITAL PINS 9 & 10
        TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
        TCCR1C = 0x00; // DON'T FORCE COMPARE
        TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
        ICR1 = 16000;  // TRIGGER TIMER INTERRUPT EVERY 2mS  
        sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED     
      } 
      
    Use the following ISR vector for the Timer1 setup above
    
      ISR(TIMER1_OVF_vect)
      
 >> Timer0
 
    DON'T USE TIMER0! Timer0 is used for counting delay(), millis(), and micros(). 
                      Messing with Timer0 is highly unadvised!
  
  ******************************************************************************************
  ARDUINO Fio, Lilypad, ProMini328-3V/8MHz (or any board with ATmega328P running at 8MHz)
  
  >> Timer2
  
    Pulse Sensor Arduino UNO uses Timer2 by default.
    Use of Timer2 interferes with PWM on pins 3 and 11.
    There is also a conflict with the Tone library, so if you want tones, use Timer1 below.
    
      void interruptSetup(){     
        // Initializes Timer2 to throw an interrupt every 2mS.
        TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
        TCCR2B = 0x05;     // DON'T FORCE COMPARE, 128 PRESCALER 
        OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
        TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
        sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
      } 
  
    use the following interrupt vector with Timer2
    
      ISR(TIMER2_COMPA_vect)
      
 >> Timer1
    
    Use of Timer1 interferes with PWM on pins 9 and 10.
    The Servo library also uses Timer1, so if you want servos, use Timer2 above.
    
      void interruptSetup(){     
        // Initializes Timer1 to throw an interrupt every 2mS.
        TCCR1A = 0x00; // DISABLE OUTPUTS AND PWM ON DIGITAL PINS 9 & 10
        TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
        TCCR1C = 0x00; // DON'T FORCE COMPARE
        TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
        ICR1 = 8000;  // TRIGGER TIMER INTERRUPT EVERY 2mS  
        sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED     
      } 
      
    Use the following ISR vector for the Timer1 setup above
    
      ISR(TIMER1_OVF_vect)
      
 >> Timer0
 
    DON'T USE TIMER0! Timer0 is used for counting delay(), millis(), and micros(). 
                      Messing with Timer0 is highly unadvised!
  
  
  ******************************************************************************************
  ARDUINO Leonardo (or any board with ATmega32u4 running at 16MHz) 
  
  >> Timer1
  
    Use of Timer1 interferes with PWM on pins 9 and 10.
    
      void interruptSetup(){
          TCCR1A = 0x00;
          TCCR1B = 0x0C; // prescaler = 256
          OCR1A = 0x7C;  // count to 124  
          TIMSK1 = 0x02; 
          sei();
      }
 
  The only other thing you will need is the correct ISR vector in the next step.
       
      ISR(TIMER1_COMPA_vect)
  
  
  ******************************************************************************************
  ADAFRUIT Flora, ARDUINO Fio v3 (or any other board with ATmega32u4 running at 8MHz)
  
  >> Timer1
  
    Use of Timer1 interferes with PWM on pins 9 and 10.
    
      void interruptSetup(){
          TCCR1A = 0x00;
          TCCR1B = 0x0C; // prescaler = 256
          OCR1A = 0x3E;  // count to 62
          TIMSK1 = 0x02; 
          sei();
      }
 
  The only other thing you will need is the correct ISR vector in the next step.
       
      ISR(TIMER1_COMPA_vect)
  ******************************************************************************************
  ADAFRUIT Gemma (or any other board with ATtiny85 running at 8MHz)
  
    NOTE: Gemma does not do serial communication! 
          Comment out or remove the Serial code in the Arduino sketch!
  
  Timer1
  
    Use of Timer1 breaks PWM output on pin D1
    
      void interruptSetup(){     
        TCCR1 = 0x88;      // Clear Timer on Compare, Set Prescaler to 128 TEST VALUE
        GTCCR &= 0x81;     // Disable PWM, don't connect pins to events
        OCR1C = 0x7C;      // Set the top of the count to  124 TEST VALUE
        OCR1A = 0x7C;      // Set the timer to interrupt after counting to TEST VALUE
        bitSet(TIMSK,6);   // Enable interrupt on match between TCNT1 and OCR1A
        sei();             // Enable global interrupts     
      } 
    The only other thing you will need is the correct ISR vector in the next step.
    
      ISR(TIMER1_COMPA_vect)
  
  
  ******************************************************************************************
  
  IF YOU DON'T SEE THE MICROCONTROLLER YOU ARE USING, BUT YOU WANT A QUICK AND DIRTY SOLUTION
  
  So many new micros are coming out that it's kind of mind boggling. We will add to this list with 
  code that uses interupts when we can, but if your micro is not listed here, and you are not willing
  or able to grab a hardware timer yourself, here is a shortcut that will 'work'.
  It won't have the tight timing of a hardware interrupt, but it just might be good enough. 
  The code below will set up a microsecond timer and 'trigger' every 2mS (or so). 
  You will need to change the name of the funcion in the Interrupts tab from
  'ISR(TIMER2_COMPA_vect)'
  to
  'void getPulse()'
  and also comment out the entire interruptSetup() function in the interrupts tab in order for this to work.
  
  Happy Hacking!
  
  
  
  // FIRST, CREATE VARIABLES TO PERFORM THE SAMPLE TIMING FUNCTION
  unsigned long lastTime;
  unsigned long thisTime;
  void setup(){
    pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
    pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
    Serial.begin(115200);             // we agree to talk fast!
    // ADD THIS LINE IN PLACE OF THE interruptSetup() CALL
    lastTime = micros();              // get the time so we can create a software 'interrupt'
    // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE, 
    // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
    //   analogReference(EXTERNAL);   
  } //end of setup()
 
  //IN THE LOOP, ADD THE CODE THAT WILL DO THE 2mS TIMING, AND CALL THE getPulse() routine.
  void loop(){
  
    serialOutput() ;       
    
    thisTime = micros();            // GET THE CURRENT TIME
    if(thisTime - lastTime > 2000){ // CHECK TO SEE IF 2mS HAS PASSED
      lastTime = thisTime;          // KEEP TRACK FOR NEXT TIME
      getPulse();                   //CHANGE 'ISR(TIMER2_COMPA_vect)' TO 'getPulse()' IN THE INTERRUPTS TAB!
    }
    
  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.     
        QS = false;                      // reset the Quantified Self flag for next time    
  }
     
  ledFadeToBeat();                      // Makes the LED Fade Effect Happen 
  delay(20);                             //  take a break
}
  
  
  
  ******************************************************************************************
  
  
  
  
  
  ******************************************************************************************
  
  
  
  
  
  ******************************************************************************************
  
  
  
  
  
  ******************************************************************************************
  
  
  
  
  
  ******************************************************************************************
  
  
  
  
  
  ******************************************************************************************
  
  
  
  
*/
