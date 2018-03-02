/*MatrixLED Display*/
# define DISP_USAGE_0 0x00
# define DISP_USAGE_1 0x01
# define DISP_USAGE_2 0x03
# define DISP_USAGE_3 0x07
# define DISP_USAGE_4 0x0f
# define DISP_USAGE_5 0x1f
# define DISP_USAGE_6 0x3f
# define DISP_USAGE_7 0x7f
# define DISP_USAGE_8 0xff

/*Serial Definitions*/
# define MAX_LENGTH 62
char dat[MAX_LENGTH];   // Serial strings
int count = 0;          // Serial strings count
int now_view[8] = {0,0,0,0,0,0,0,0};
int new_view[8] = {6,21,36,51,61,71,81,91};

/*Port Definitions*/
const int Max7219_pinCLK = 9;
const int Max7219_pinCS  = 8;
const int Max7219_pinDIN = 7;

void Write_Max7219_byte(unsigned char DATA)
{
  unsigned char i;
  digitalWrite(Max7219_pinCS, LOW);
  for (i = 8; i >= 1; i--)
  {
    digitalWrite(Max7219_pinCLK, LOW);
    digitalWrite(Max7219_pinDIN, DATA & 0x80); // Extracting a bit data
    DATA = DATA << 1;
    digitalWrite(Max7219_pinCLK, HIGH);
  }
}

void Write_Max7219(unsigned char address, unsigned char dat)
{
  digitalWrite(Max7219_pinCS, LOW);
  Write_Max7219_byte(address);     //address，code of LED
  Write_Max7219_byte(dat);         //data，figure on LED
  digitalWrite(Max7219_pinCS, HIGH);
}

void Init_MAX7219(void)
{
  Write_Max7219(0x09, 0x00);       //decoding ：BCD
  Write_Max7219(0x0a, 0x00);       //brightness
  Write_Max7219(0x0b, 0x07);       //scanlimit；8 LED
  Write_Max7219(0x0c, 0x01);       //power-down mode：0，normal mode：1
  Write_Max7219(0x0f, 0x00);       //test display：1；EOT，display：0
//  
//  Write_Max7219(0x01,DISP_USAGE_1);//Init views
//  Write_Max7219(0x02,DISP_USAGE_2);
//  Write_Max7219(0x03,DISP_USAGE_3);
//  Write_Max7219(0x04,DISP_USAGE_4);
//  Write_Max7219(0x05,DISP_USAGE_5);
//  Write_Max7219(0x06,DISP_USAGE_6);
//  Write_Max7219(0x07,DISP_USAGE_7);
//  Write_Max7219(0x08,DISP_USAGE_8);
}

void updateView() {
  for(int line = 0 ;line < 8; line++) {
    if(new_view[line] > now_view[line]) {
      if(new_view[line] > now_view[line]+5) {
        now_view[line] += 5;        
      } else {
        now_view[line]++;
      }
    } else if (new_view[line] < now_view[line]){
      if(new_view[line] < now_view[line]-5) {
        now_view[line] -= 5;        
      } else {
        now_view[line]--;
      }
    }
    Write_Max7219(line+1, getDispUsage(now_view[line]));
  }
}

void writeCpuUsage(char* usage, int usage_length) {
  char char_usage[6];
  int char_usage_now = 0;
  int str_now = 0;
  
  for(int line = 0 ;line < 8; line++) {
    char_usage[0] = '0';
    char_usage_now = 0;

    for(char_usage_now = 0 ;str_now < usage_length && char_usage_now < 6 ; str_now++) {
      if(usage[str_now] >= 48 &&   // 0  
         usage[str_now] <= 57 ) {  // 9
           char_usage[char_usage_now] = usage[str_now];
           char_usage_now++;
       } else if(usage[str_now] == '.' ||
                 usage[str_now] == ',' ||
                 usage[str_now] == ']' ) {
         //Ignore the decimal point
         while(usage[str_now] != ',' &&
               usage[str_now] != ']') {
           str_now++;
         }
         str_now++;
         break;
       } 
    }
    char_usage[char_usage_now+1] = '\0';
//    Write_Max7219(line, getDispUsage(atoi(char_usage)));
    new_view[line] = atoi(char_usage);
  }
}

unsigned char getDispUsage(int usage)
{
  if(usage < 5) 
    return DISP_USAGE_0;
  else if (usage < 20)
    return DISP_USAGE_1;
  else if (usage < 35) 
    return DISP_USAGE_2;
  else if (usage < 50) 
    return DISP_USAGE_3;
  else if (usage < 60)
    return DISP_USAGE_4;
  else if (usage < 70) 
    return DISP_USAGE_5;
  else if (usage < 80) 
    return DISP_USAGE_6;
  else if (usage < 90) 
    return DISP_USAGE_7;
  
  return DISP_USAGE_8;
}

void setup() {
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  delay(50);
  Init_MAX7219();
  
  Serial.begin(9600);
}

void loop() {
  String usage;
  while (Serial.available() > 0) {
    dat[count] = Serial.read();
    if (count > MAX_LENGTH - 2 || dat[count] == ']') {
      dat[count] = '\0';
      writeCpuUsage(dat, count);
      count = 0;
    } else {
      count++;
    }
  }
  updateView();
  delay(10);
}
