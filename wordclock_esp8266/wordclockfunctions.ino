
//const String clockStringGerman =  "ESPISTAFUNFVIERTELZEHNZWANZIGUVORTECHNICNACHHALBMELFUNFXCONTROLLEREINSEAWZWEIDREITUMVIERSECHSQYACHTSIEBENZWOLFZEHNEUNJUHR"; //Onno
//const String clockStringGerman =  "HETVISNEENZESTWEEDRIEZVIERVIJFZEVENACHTNEGEN-TIENELFTWAALFDERTIENVEERTIENKWARTVOOROVERTHALFEENXTWEENDRIEVIERGVIJFNZESKLZEVENMACHTRFNEGENSTIENEETWAALFIT*****ELFLNUUR*****";
const String clockStringGerman =  "HETVISNEENZESTWEEDRIEZVIERVIJFZEVENACHTNEGEN-TIENELFTWAALFDERTIENVEERTIENKWARTVOOROVERTHALFEENXTWEENDRIEVIERGVIJFNZESKLZEVENMACHTRFNEGENSTIENEETWAALFITELFLNTBGUUR&*****&";
// Onno let op Q ipv -

/**
 * @brief control the four minute indicator LEDs
 * 
 * @param minutes minutes to be displayed [0 ... 59]
 * @param color 24bit color value
 */
void drawMinuteIndicator(uint8_t minutes, uint32_t color){
  //separate LEDs for minutes in an additional row
  {
  switch (minutes%5)
    { 
      case 0:
        break;
          
      case 1:
        ledmatrix.setMinIndicator(0b1000, color);
        break;

      case 2:
        ledmatrix.setMinIndicator(0b1100, color);
        break;

      case 3:
        ledmatrix.setMinIndicator(0b1110, color);
        break;

      case 4:
        ledmatrix.setMinIndicator(0b1111, color);
        break;
    }
  }
}

/**
 * @brief Draw the given sentence to the word clock
 * 
 * @param message sentence to be displayed
 * @param color 24bit color value
 * @return int: 0 if successful, -1 if sentence not possible to display
 */
int showStringOnClock(String message, uint32_t color){
    int messageStart = 0;
    String word = "";
    int lastLetterClock = 0;
    int positionOfWord  = 0;
    int nextSpace = 0;
    int index = 0;

    // add space on the end of message for splitting
    message = message + " ";

    // empty the targetgrid
    ledmatrix.gridFlush();

    while(true){
      // extract next word from message
      word = split(message, ' ', index);
      index++;
      
      if(word.length() > 0){
        // find word in clock string
        positionOfWord = clockStringGerman.indexOf(word, lastLetterClock);
        
        if(positionOfWord >= 0){
          // word found on clock -> enable leds in targetgrid
          for(int i = 0; i < word.length(); i++){
            int x = (positionOfWord + i)%WIDTH;
            int y = (positionOfWord + i)/WIDTH;
            ledmatrix.gridAddPixel(x, y, color);
          }
          // remember end of the word on clock
          lastLetterClock = positionOfWord + word.length();
        }
        else{
          // word is not possible to show on clock
          Serial.println("word is not possible to show on clock: " + String(word));
          return -1;
        }
        //logger.logString(String(nextSpace) + " - " + String());
      }else{
        // end - no more word in message
        break;
      }
    }
    for(int i = 0; i < clockStringGerman.length(); i++){
      if (clockStringGerman.charAt(i) == '*') {
      
        int x = (i)%WIDTH;
        int y = (i)/WIDTH;
        ledmatrix.gridAddPixel(x, y, colors24bit[6]);
      }
        else if (clockStringGerman.charAt(i) == '&') {
        int x = (i)%WIDTH;
        int y = (i)/WIDTH;
        ledmatrix.gridAddPixel(x, y, colors24bit[4]); //was 1
      }
    }
    // return success
    return 0;
}

/**
 * @brief Converts the given time as sentence (String)
 * 
 * @param hours hours of the time value
 * @param minutes minutes of the time value
 * @return String time as sentence
 */
String timeToString(uint8_t hours,uint8_t minutes){
  Serial.println(hours);
  Serial.println(minutes);
  
  //ES IST
  //String message = "ES IST "; //Onno
  String message = "HET IS ";

  String minuteWords[] = {"EEN", "TWEE", "DRIE", "VIER", "VIJF", "ZES", "ZEVEN", "ACHT", "NEGEN", "TIEN", "ELF", "TWAALF", "DERTIEN", "VEERTIEN", "KWART", "ZES -TIEN", "ZEVEN -TIEN", "ACHT -TIEN", "NEGEN-TIEN"};
  //show minutes
  if(minutes > 0 && minutes < 20)
  {
    message += minuteWords[minutes-1] + " OVER ";
  }
  else if(minutes >= 20 && minutes < 30)
  {
    message += minuteWords[29 - minutes] + " VOOR HALF ";
  }
  else if(minutes == 30)
  {
    message += "HALF ";
  }
  else if(minutes > 30 && minutes < 45)
  {
    message += minuteWords[minutes - 31] + " OVER HALF ";
  }
  else if(minutes >= 45 && minutes <= 59)
  {
    message += minuteWords[59 - minutes] + " VOOR ";
  }

  //convert hours to 12h format
  if(hours >= 12)
  {
      hours -= 12;
  }
  if(minutes >= 20)
  {
      hours++;
  }
  if(hours == 0)
  {
      hours = 12;
  }
  message += minuteWords[hours - 1] + " ";

  if(minutes == 0)
  {
    message += "UUR ";
  }

  // message += "XXXXX "; //Onno
  // message += "QQQQQ "; //Onno
  
  Serial.println(message);

  return message;
}

