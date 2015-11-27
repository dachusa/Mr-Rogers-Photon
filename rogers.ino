#include "SparkFunMicroOLED/SparkFunMicroOLED.h"
/*
    Released under the MIT License (http://opensource.org/licenses/MIT)
*/

#define SPEAKER_PIN D2
#define PIN_OLED_RST D6  // Connect RST to pin 6
#define PIN_OLED_DC  D5  // Connect DC to pin 5 (required for SPI)
#define PIN_OLED_CS  A2  // Connect CS to pin A2 (required for SPI)
MicroOLED oled(MODE_SPI, PIN_OLED_RST, PIN_OLED_DC, PIN_OLED_CS);

const int songLength = 132;
const int BPM = 85;

// a space represents a rest
char notes[] =  "CBCBCBCBDCBA"          
                "BABABACBAGCGAG"        
                "CGAB"                  
                "CBCBCBDCBA"            
                "BABABACBAGCGAG"        
                "CFFF"                  
                "FFFFFFFFFGGAA"         
                "AAAAABBBBBBBCB"        
                "BCBCBCDCBAABABABCBAG"  
                "CFGADGABEDCEGF"        
                "DDCDDCDCBAEGF";        

char octave[] = { 
                5,5,5,5,5,5,5,5,5,5,5,5,                    
                5,5,5,5,5,5,5,5,5,4,4,4,5,4,                
                4,4,5,5,                                    
                5,5,5,5,5,5,5,5,5,5,                        
                5,5,5,5,5,5,5,5,5,4,4,4,5,4,                
                4,4,4,4,                                    
                4,4,4,4,4,4,4,4,4,4,4,4,4,                  
                5,5,5,5,5,5,5,5,5,5,5,5,5,5,                
                5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,    
                4,4,4,5,4,4,5,5,4,5,5,4,4,4,                
                4,5,5,4,5,5,5,5,5,5,4,4,4                   
};
                
//space is natural
//b is flat
//# is sharp
char accidentals[] =  "          b "          
                "b b b  b      "        
                "   b"                  
                "        b "            
                "b b b  b      "        
                "    "                  
                "     ####  bb"         
                "     bbbbb   b"        
                "        b  b b b b  "  
                "       b      "        
                "        b    ";        
                
                

// Beats is an array of values for each note and rest.
// A "1" represents a 12th of a note, 24 a half-note, etc.  3 for 16th, 3 for tripplet
// Don't forget that the rests (spaces) need a length as well.

int beats[] = { 
                9,3,4,4,4,4,4,4,9,3,9,3,                    
                4,4,4,4,4,4,9,3,9,3,6,30,6,6,               
                6,30,9,3,                                   
                4,4,4,4,4,4,9,3,9,3,                        
                4,4,4,4,4,4,9,3,9,3,6,30,6,6,               
                6,30,6,6,                                   
                9,3,4,4,4,9,3,9,3,12,6,24,6,                
                9,3,4,4,4,4,4,4,9,3,12,6,18,12,             
                4,4,4,4,4,4,4,4,4,12,4,4,4,4,4,4,4,4,4,12,  
                4,4,4,12,4,4,4,12,9,3,9,3,6,18,             
                9,3,12,9,3,24,4,4,4,9,3,3,21                
};

int beatCount = 12;
int beatDuration = (60 * 1000) / beatCount / BPM;

void setup() 
{
    oled.begin();
    
    oled.clear(PAGE);
    oled.setFontType(1); 
    oled.setCursor(0, 0);
    oled.print("Hello.");
    
    oled.display();
    delay(2000); 
    
	pinMode(SPEAKER_PIN, OUTPUT);

	int i, duration, freq;

  for (i = 0; i < songLength; i++)
  {
    oled.clear(PAGE);
    oled.setFontType(1);
    oled.setCursor(0, 0);
    
    duration = beats[i] * beatDuration;

    if (notes[i] == ' ')
    {
        oled.print("Rest " + String(duration));
        oled.display();
        delay(duration);
    }
    else           
    {
        freq = frequency(octave[i], notes[i], accidentals[i]);
        oled.print(String(notes[i]) + String(accidentals[i]) + String(octave[i]) + " ");
        oled.display();
        tone(SPEAKER_PIN, freq, duration);
        delay(duration);
    }
    delay(beatDuration/2);
  }
}


void loop() 
{
//do nothing 
}


int frequency(int octave, char note, char accidental) 
{
  // This function takes an octave, a note character (A-G), and the accidental and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 7;  // number of notes we're storing

  char key[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  
  //Frequency for the natural key
  double frequencies[] = {27.5, 30.87, 32.7, 36.71, 41.2, 43.65, 49};
  //Frequency for the sharp key (or next note if no sharp)
  double sharpfrequencies[] = {29.14, 32.7, 34.65, 38.89, 43.65, 46.25, 51.91};
  //Frequency for the flat key (or the lower note if no flat)
  double flatfrequencies[] = {51.91, 29.14, 30.87, 34.65, 38.89, 41.2, 46.25}; 

  //Manually created a power calculation to avoid including the full math handler for one simple method
  int octaveAdjust = 2;
  if(octave == 0){
      octaveAdjust = 1;
  }else{
      for(i = 1; i < octave; i++){
          octaveAdjust = octaveAdjust * 2;
      }
  }
  
  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.
  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
      if(key[i] == note){
          switch(accidental){
              case '#':
                  return (int)(octaveAdjust * sharpfrequencies[i] + .5);
                  break;
              case 'b':
                  return (int)(octaveAdjust * flatfrequencies[i] + .5);
                  break;
              default:
                  return (int)(octaveAdjust * frequencies[i] + .5);
          }
      }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}