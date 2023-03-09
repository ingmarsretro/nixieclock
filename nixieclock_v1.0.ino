 //nixieclock first release v1.0 19.10.2021
 //by ingmarsretro 
 //microcontroller Atmega328
 // der 4094 ist ein 8bit shiftregister
 // je 4 bit davon werden für eine nixieröhre verwendet -> ein bcd zu decimal decoderchip steuert die ziffern an
 // also zeigen je zwei röhren eigentlich hex an (da a-f aber nicht dargestellt wird und die anzeige zur basis 10 anzeigen soll
 // muss das beachtet werden
 
 // ansteuerung der nixies über taktung (pwm)
 // die anzeige an den röhren wird in hex dargestellt -> also der dezimal lesbare anteil der hexcodierung
 
 int countsek = 0;int displ_sek;
 int countmin = 0;int displ_min;  
 int countstd = 0;int displ_hr;  
 int a = 0; int dezs = 0; // -16 daher dass (dezs=dezs+16) und es soll ja bei 0 anfangen und nicht bei 10
 int b = 0; int dezm = 0; //256dec entsr 100 hex also werden 00 angezeigt
 int c = 0; int dezh = 0;  //256dec entsr 100 hex also werden 00 angezeigt 

long myTimer = 0;
long myTimeout = 1000;

    //pinzuordnung für schieberegisteransteuerung 4094
    int str0 = 8;  //Pin connected to STR1(pin 1) of HEF4094
    int str1 = 9;  //Pin connected to STR2(pin 1) of HEF4094
    int str2 = 10; //Pin connected to STR3(pin 1) of HEF4094
    int clk  = 12; //Pin connected to CP  (pin 3) of HEF4094
    int dat = 11; //Pin connected to D   (pin 2) of HEF4094
    int oe = 2;    //pin output enable 

    //eingänge für die bedienung 
    int tasth = A0;
    int tastm = A1; // auch als DDRC = B11111100 pin pc0 und pc1 als input
 
void setup() {
              pinMode(tasth, INPUT); pinMode(tastm, INPUT);  // taster s2 und s3  an PC0(pin23) und PC1(pin24)
              pinMode(str0, OUTPUT); pinMode(str1, OUTPUT); pinMode(str2, OUTPUT);
              pinMode(clk, OUTPUT); pinMode(dat, OUTPUT); pinMode(oe, OUTPUT);
        
              Serial.begin(9600); //   serial debug initialisieren 

              //erstmal alle outputs auf low
              digitalWrite(oe, LOW);
              digitalWrite(str0, LOW); digitalWrite(str1, LOW); digitalWrite(str2, LOW);
              digitalWrite(clk, LOW);
              }



void anzeige(int clk, int displ_sek, int displ_min, int displ_hr) 
              {
              digitalWrite(str0, HIGH);                 //latch HIGH to send data
              shiftOut(dat, clk, MSBFIRST, displ_sek);  //send data
              digitalWrite(str0, LOW);                  //latch LOW to stop sending data
   
              digitalWrite(str1, HIGH);                 //latch HIGH to send data
              shiftOut(dat, clk, MSBFIRST, displ_min);  //send data
              digitalWrite(str1, LOW);                  //latch LOW to stop sending data
              digitalWrite(str2, HIGH);                 //latch HIGH to send data
              shiftOut(dat, clk, MSBFIRST, displ_hr);  //send data
              digitalWrite(str2, LOW);                  //latch LOW to stop sending data 
              delay(5);                  // röhren ein und anzeigen
        
                    // Alle Röhren ohne Anzeige durch Ansteuern von hexAA = 170 dec
                    digitalWrite(str0, HIGH);                 //latch HIGH to send data
                    shiftOut(dat, clk, MSBFIRST, 170); //send data
                    digitalWrite(str0, LOW);                  //latch LOW to stop sending data
                    digitalWrite(str1, HIGH);                 //latch HIGH to send data
                    shiftOut(dat, clk, MSBFIRST, 170); //send data
                    digitalWrite(str1, LOW);                  //latch LOW to stop sending data
                    digitalWrite(str2, HIGH);                 //latch HIGH to send data
                    shiftOut(dat, clk, MSBFIRST, 170); //send data
                    digitalWrite(str2, LOW);                  //latch LOW to stop sending data
                    delay(5);                  // alle röhren aus 
              }

void loop() 
            {
              // Serial.print("\n\r");
              digitalWrite(oe, HIGH); //chip enable für alle ein
       
                    if (digitalRead(A1)== LOW){countmin++;b++;delay(150);}  //minutenwert hochzählen
                    if (digitalRead(A0)== LOW){countstd++;c++;delay(150);} // stundenwert hochzählen

                    if (millis() > myTimeout + myTimer ) {  //sekundentakt erzeugen
                    myTimer = millis();
                    countsek++; a++;
            }
         
              // sekundenzähler     dezimal
                if (a==10) {a=0; dezs=dezs+6;}    
                if (countsek>59) { countsek=0; countmin++; b++; dezs=0;}
         
                          displ_sek=countsek+dezs; 
             // minutenzähler 
               if (b==10) {b=0; dezm=dezm+6;}      
               if (countmin>59) { countmin=0; countstd++; c++; dezm=0;}
         
                          displ_min=countmin+dezm; 
             //stundenzähler    
               if (c==10) {c=0; dezh=dezh+6;}
               if (countstd>23) { countstd=0; dezh=0; c=0;}
                          
                          displ_hr=countstd+dezh; 
         
                 anzeige(clk,displ_sek,displ_min,displ_hr);                     
         
                                Serial.print("\t");
                                Serial.print(countmin); Serial.print("\t");
                                Serial.print(countstd); Serial.print("\t");
                        //      Serial.print(sek_value); Serial.print("\t");
                        //      Serial.print(min_value); Serial.print("\t");
                        //      Serial.print(std_value); 
                                Serial.print("\n\r");
                          }
