/* 

hello .. display stuff 


*/

uint8_t MENU_CURRENT = 2; 
int16_t SCALE_SEL = 0;
uint8_t X_OFF = 110; // display x offset
uint8_t OFFSET = 2;  // display y offset
uint8_t UImode = 0;
uint8_t MENU_REDRAW = 0;
const uint16_t TIMEOUT = 5000; // time out menu (in ms)

/* number of scales */
const int8_t MAXSCALES = (sizeof(scales)/7);
const int8_t MENU_ITEMS = 6;

/* names for scales go here (names shouldn't be longer than 12 characters) */
const String abc[MAXSCALES] =    {
  
  "locrian nat.", //0
  "locrian",      //1
  "dorian",       //2
  "pent. minor",  //3
  "minor",        //4
  "scriabin",     //5
  "marva",        //6
  "indian",       //7
  "aolean",       //8
  "phrygian",     //9
  "gamelan",      //10
  "hex sus",      //11
  "purvi",        //12
  "bhairav",      //13
  "pelog",        //14
  "whatever",     //15
  "etc",          //16
  "xxx",          //17
  "zzz"           //18
};


/*  names for menu items */
char *menu_strings[MENU_ITEMS] = {

   "", 
   "", 
   "offset", 
   "asr_index", 
   "#/scale", 
   "att/mult" 
};
                  
/* pseudo .x for display */                                 
const String map_param5[] =    {"0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.0",
                                "1.1", "1.2", "1.3", "1.4", "1.5", "1.6", "1.7", "1.8", "1.9", "2.0"};

/* ASR parameters: scale, octave, offset, delay, #/scale, attenuation */ 
int16_t asr_display_params[] = {0, 0, 0, 0, 0, 0};
/* limits for menu parameters */
const int16_t param_limits[][2] = {{0,0}, {-5,5}, {-999,999}, {0,63}, {2,MAXNOTES}, {7,26}}; 


/* ----------- time out UI ---------------- */ 

void timeout() {
 
  
  if (millis() - LAST_UI > TIMEOUT) UImode = SCREENSAVER; 
  else if (millis() - LAST_TRIG > TRIG_LENGTH) {        
         MENU_REDRAW = 1;
         display_clock = false;
  }  
} 

/* -----------  loop  --------------------- */ 

void UI() {
  
  if (  MENU_REDRAW != 0 ) {
        u8g.firstPage();  
        do {
    
          draw(); 
    
        } while( u8g.nextPage() );
        MENU_REDRAW = 0;
  }
}  

/* -----------  splash  ------------------- */  

void hello() {
  
  u8g.setFont(u8g_font_6x12);
  u8g.setColorIndex(1);
  u8g.firstPage();  
        do {
    
            for(int i = 0; i < 4; i++ ) { 
              u8g.drawBox(i*37, 24, 10, 24);
            }
    
        } while( u8g.nextPage() ); 
}  


/* -----------   draw menu  --------------- */ 

void draw(void) { 
  
  switch(UImode) {

      case SCREENSAVER: { // draw some vertical bars

            uint8_t x, y, width = 10;
            for(int i = 0; i < 4; i++ ) { 
                x = i*37;
                y = asr_outputs[i] >> 10; 
                y++; 
                u8g.drawBox(x, 64-y, width, width); // replace second 'width' with y for bars.
            }
            break; 
      } 
      
      case MENU: {  // draw main menu
       
            makemenu();
            break;    
      }
      
      case CALIBRATE: {
           calibrate();
           break;
      } 
   } 
}
  
/* --------------------- main menu loop ------------------------  */

void makemenu() {
  
  
        uint8_t i, h;
        u8g_uint_t w, d;
        u8g.setFontRefHeightText();
        u8g.setFontPosTop();
        h = OFFSET + u8g.getFontAscent()-u8g.getFontDescent();
        w = u8g.getWidth();
  
        /* draw user menu */
        for(i = 0; i < MENU_ITEMS; i++) {   
       
            u8g.setDefaultForegroundColor();
            /* print cursor */          
            if (i == MENU_CURRENT) {              
                  u8g.drawBox(0, i*h, w, h);           
                  u8g.setDefaultBackgroundColor();
            }
            u8g.drawStr(10, i*h, menu_strings[i]);  
            /* print parameters 2-5 */      
            if (i == 5) {  
                u8g.setPrintPos(X_OFF,i*h); 
                uint8_t x = asr_params[5]-7;
                u8g.print(map_param5[x]);       
            } 
            else if (i > 1)  {                            
                if (asr_display_params[i] < 0) u8g.setPrintPos(X_OFF-6, i*h);
                else u8g.setPrintPos(X_OFF,i*h);
                u8g.print(asr_display_params[i]);
            }  
            /* print parameters 0-1 */
            else if (!i) {                          
           
               /*  print display clock ? */ 
               if (display_clock) {               
                   u8g.setPrintPos(0,OFFSET);
                   u8g.print('\xb7');
               }
              /*  print scale */     
               if ((SCALE_SEL == asr_params[0]) || (!SCALE_CHANGE)) {      
                   u8g.setPrintPos(10, OFFSET); 
                   u8g.print(">> " + abc[SCALE_SEL]); 
               }
               else  {     
                   u8g.setPrintPos(10, OFFSET);
                   u8g.print("<< " + abc[SCALE_SEL]);
               }
               /* print octave +/- */
               if ((asr_params[1]) >= 0) {            
                   u8g.setPrintPos(X_OFF-6, OFFSET); 
                   u8g.print("+");
                   u8g.setPrintPos(X_OFF, OFFSET); 
                   u8g.print(asr_params[1]);
               }
               else {     
                   u8g.setPrintPos(X_OFF-6, OFFSET); 
                   u8g.print(asr_params[1]);
               }     
          }
          
      u8g.setDefaultForegroundColor();           
    }
}