#include <lpc214x.h>
#include <string.h>

#define CLEAR (IO0SET = 0x000F0000)
#define SET1 IO0CLR = 1 << 16
#define SET2 IO0CLR = 1 << 17
#define SET3 IO0CLR = 1 << 18
#define SET4 IO0CLR = 1 << 19
#define C0(IO1PIN & 1 << 19)
#define C1(IO1PIN & 1 << 18)
#define C2(IO1PIN & 1 << 17)
#define C3(IO1PIN & 1 << 16)
#define LED_ON IO0CLR |= 1 << 31
#define LED_OFF IO0SET |= 1 << 31
#define Check0                                                                 \
  if (C0 == 0) {                                                               \
    col = 0;                                                                   \
    break;                                                                     \
  }
#define Check1                                                                 \
  if (C1 == 0) {                                                               \
    col = 1;                                                                   \
    break;                                                                     \
  }
#define Check2                                                                 \
  if (C2 == 0) {                                                               \
    col = 2;                                                                   \
    break;                                                                     \
  }
#define Check3                                                                 \
  if (C3 == 0) {                                                               \
    col = 3;                                                                   \
    break;                                                                     \
  }
#define ENTER '0'
#define WAIT                                                                   \
  while (C0 == 0 || C1 == 0 || C2 == 0 || C3 == 0)                             \
    ;
#define RS_ON (IO0SET = 1 U << 20)
#define RS_OFF (IO0CLR = 1 U << 20)
#define EN_ON (IO1SET = 1 U << 25)
#define EN_OFF (IO1CLR = 1 U << 25)

unsigned int x = 0;
long int count = 10;
char buf[5];
char ch, keys[5], password[5] = "1234";
unsigned char len = 0;
unsigned int i = 0;

void delay(int x) {
  for (int i = 0; i < x; i++)
    for (int y = 0; y < 10000; y++)
      ;
}
static void delay_us(unsigned int count) {
  unsigned int j = 0, i = 0;
  for (j = 0; j < count; j++) {
    for (i = 0; i < 10; i++)
      ;
  }
}
static void LCD_SendCmdSignals(void) {
  RS_OFF;
  EN_ON;
  delay_us(100);
  EN_OFF;
}
static void LCD_SendDataSignals(void) {
  RS_ON;
  EN_ON;
  delay_us(100);
  EN_OFF;
}
static void LCD_SendHigherNibble(unsigned char dataByte) {
  // send the D7,6,5,D4(uppernibble) to P0.16 to P0.19
  IO0CLR = 0X000F0000;
  IO0SET = ((dataByte >> 4) & 0x0f) << 16;
}
static void LCD_CmdWrite(unsigned char cmdByte) {
  LCD_SendHigherNibble(cmdByte);
  LCD_SendCmdSignals();
  cmdByte = cmdByte << 4;
  LCD_SendHigherNibble(cmdByte);
  LCD_SendCmdSignals();
}
static void LCD_DataWrite(unsigned char dataByte) {
  LCD_SendHigherNibble(dataByte);
  LCD_SendDataSignals();
  dataByte = dataByte << 4;
  LCD_SendHigherNibble(dataByte);
  LCD_SendDataSignals();
}
static void LCD_Reset(void) {
  /* LCD reset sequence for 4-bit mode*/
  LCD_SendHigherNibble(0x30);
  LCD_SendCmdSignals();
  delay(100);
  LCD_SendHigherNibble(0x30);
  LCD_SendCmdSignals();
  delay_us(200);
  LCD_SendHigherNibble(0x30);
  LCD_SendCmdSignals();
  delay_us(200);
  LCD_SendHigherNibble(0x20);
  LCD_SendCmdSignals();
  delay_us(200);
}
static void LCD_Init(void) {
  delay(100);
  LCD_Reset();
  LCD_CmdWrite(0x28u); // Initialize the LCD for 4-bit 5x7 matrix type
  LCD_CmdWrite(0x0Eu); // Display ON cursor ON
  LCD_CmdWrite(0x01u); // Clear the LCD
  LCD_CmdWrite(0x80u); // go to First line First Position
}
void LCD_DisplayString(const char *ptr_string) {
  while ((*ptr_string) != 0)
    LCD_DataWrite(*ptr_string++);
}
unsigned char getAlphaCode(unsigned char alphachar) {
  switch (alphachar) {
  case 'f':
    return 0x8e;
  case 'i':
    return 0xf9;
  case 'r':
    return 0xce;
  case 'e':
    return 0x86;
  case 'h':
    return 0x89;
  case 'l':
    return 0xc7;
  case 'p':
    return 0x8c;
  case ' ':
    return 0xff;
  case '1':
    return 0xf9;
  case '2':
    return 0xa4;
  case '3':
    return 0xb0;
  case '4':
    return 0x99;
  case '5':
    return 0x92;
  case '6':
    return 0x82;
  case '7':
    return 0xf8;
  case '8':
    return 0x80;
  case '9':
    return 0x90;
  case '0':
    return 0xc0;
  default:
    break;
  }
  return 0xff;
}
void alphadisp7SEG(char *buf) {
  unsigned char i, j;
  unsigned char seg7_data, temp = 0;
  for (i = 0; i < 5; i++) {
    seg7_data = getAlphaCode(*(buf + i));

    for (j = 0; j < 8; j++) {
      temp = seg7_data & 0x80;

      if (temp == 0x80)
        IOSET0 |= 1 << 19;
      else
        IOCLR0 |= 1 << 19;

      IOSET0 |= 1 << 20;
      delay(1);
      IOCLR0 |= 1 << 20;
      seg7_data = seg7_data << 1;
    }
  }

  IOSET0 |= 1 << 30;
  delay(1);
  IOCLR0 |= 1 << 30;
  return;
}
__irq void Timer0_ISR(void) // an ISR program
{
  // 7seg logic
  sprintf(buf, "%05lu", count);
  alphadisp7SEG(&buf[0]);
  count--;
  if (count == 0) {
    count = 10;
  }
  delay(600);
  T0IR = 0x01; // clear match0 interrupt, and get ready for the next interrupt
  VICVectAddr = 0x00000000; // End of interrupt
}
char getKey() {
  unsigned char lookup_table[4][4] = {{'0', '1', '2', '3'},
                                      {'4', '5', '6', '7'},
                                      {'8', '9', 'a', 'b'},
                                      {'c', 'd', 'e', 'f'}};
  unsigned int row = 0, col = 0;
  do {
    row = 0;
    CLEAR;
    SET1;
    Check0;
    Check1;
    Check2;
    Check3;

    row = 1;
    CLEAR;
    SET2;
    Check0;
    Check1;
    Check2;
    Check3;

    row = 2;
    CLEAR;
    SET3;
    Check0;
    Check1;
    Check2;
    Check3;

    row = 3;
    CLEAR;
    SET4;
    Check0;
    Check1;
    Check2;
    Check3;
  } while (1);

  delay(50); // debouncing
  WAIT;
  delay(50); // debouncing

  return lookup_table[row][col];
}

int readSensor(int sen_no) {
  int result = 0;
  IO1DIR |= 1 << 24;
  IO1CLR |= 1 << 24; // enable sensor logic
  switch (sen_no) {
  case 1:
    result = IO1PIN & (1 << 22);
    break;
  case 2:
    result = IO1PIN & (1 << 23);
    break;
  default:
    result = 0;
  }
  IO1SET = 1 << 24;
  return result;
}

int main() {
  int result = 0;
  IO1DIR |= 1 U << 25;
  IO0DIR |= 1 U << 31 | 1 U << 19 | 1 U << 20 | 1 U << 30 | 0x00FF0000;

  LED_OFF;

  LCD_Reset();
  LCD_Init();
  delay(100);
  LCD_CmdWrite(0x80);
  LCD_CmdWrite(0x80);

  T0TCR = 0x00;    // stop the timer, to initialize different registers
  T0MCR = 0x0003;  // Enable Interrupt and reset timer after match
  T0TC = 0x00;     // make TC = 0
  T0MR0 = 1500000; // generates 1s

  // load interrupt related registers , assigning Timer0 to IRQ slot 4
  VICVectAddr4 = (unsigned long)Timer0_ISR; // set the timer ISR vector address
  VICVectCntl4 = 0x0000024;                 // set the channel
  VICIntEnable = 0x0000010;                 // enable the timer0 interrupt
  T0TCR = 0x01;                             // start the timer

  LCD_DisplayString("Enter Pass: ");

  while (1) {
    while ((result = readSensor(2)) != 0) {
      char ch;
      do {
        i = 0;
        VICIntEnable = 0x00000000;
        T0TCR = 0x00; // start the timerdelay(10);
        IO0SET |= 1 U << 16 | 1 U << 17 | 1 U << 18 | 1 U << 19;
        while (1) {
          if ((ch = getKey()) == ENTER)
            break;
          keys[i++] = ch;
        }
        keys[i] = '\0';

        if ((strcmp(keys, password)) == 0) {
          LCD_CmdWrite(0x80); // go to First line First Position
          LCD_DisplayString("BOMB DIFFUSED!");
          LED_ON;
          delay(2000);
          return 0;
        }
        LCD_CmdWrite(0x80);
        LCD_DisplayString("BOMB EXPLODED!");

      } while (1);
    }
  }
  return 0;
}
