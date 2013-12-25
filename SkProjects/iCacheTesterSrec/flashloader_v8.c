/*******************************************************************************
 * Copyright (c) 2006 SkyTraq Technology, Inc. Hsinchu, Taiwan
 * All Rights Reserved.
 *
 * file name: flashloader.c
 * Initial:
 * Current:
 ******************************************************************************/

//---------- head file inclusions -------------------------
#include "flashloader_v8.h"
//#define FIFOACK
#define DL_BYTES 8192
#define WAITTIME 200000
//#define WRITE_TEST
//#define WRITE_TAG
#define CUSTOMER_ID 0x0a
#define PRODUCT_ID 0x01
#define CALC_CPU_CLOCK 1
#define LONG_OUTPUT    0
//#define UART_INIT_BAUDRATE  7 //0: 4800, 1:9600, 2:19200; 3:38400; 4:57600; 5:115200; 6: 230400: 7:460800, 8:921600
typedef volatile unsigned long Register;
//---------- local definitions ----------------------------
#define BUILD_MONTH  (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? "01" : "06") \
                    : __DATE__ [2] == 'b' ? "02" \
                    : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? "03" : "04") \
                    : __DATE__ [2] == 'y' ? "5" \
                    : __DATE__ [2] == 'l' ? "7" \
                    : __DATE__ [2] == 'g' ? "8" \
                    : __DATE__ [2] == 'p' ? "9" \
                    : __DATE__ [2] == 't' ? "10" \
                    : __DATE__ [2] == 'v' ? "11" : "12")
                      

#define REV_ID ""
//---------- local types ----------------------------------

//---------- static global variables ----------------------
U32 uart_offset = 0;
U08 chipmode = 0;

U32 mybaud = UART_INIT_BAUDRATE;
int eraseTag = 0;
int writeTag = 0;
U32 tagAdd = 0;
U32 tagVal = 0;


//------------------- Customize define -------------------
#if defined(_SKYTRAQ_)
 #undef CALC_CPU_CLOCK
 #undef LONG_OUTPUT
 #undef REV_ID

 #define CALC_CPU_CLOCK 1 
 #define LONG_OUTPUT    0
 #define REV_ID "STQ01"

#elif defined(_HILO_)
 #undef CALC_CPU_CLOCK
 #undef LONG_OUTPUT
 #undef REV_ID

 #define CALC_CPU_CLOCK 0
 #define LONG_OUTPUT    1
 #define REV_ID "HLO01"

#else
 #error "Must define _SKYTRAQ_ or other vender name."
#endif
//---------- static function declarations -----------------
void InitSystem();
void InitFlashWriteMode();
void InitUart(int baud);
U32 GetInt(const int start, unsigned char* buf);
void DebugOutput0(const char *title, U32 data, U08 addDollor, U08 addNewLine);
void DebugOutput(const char *title, U32 data);
void DebugOutput2(const char *title, U32 addr, U32 result);
void DebugOutput3(const char *title, U32 way, U32 addr, U32 result);
void DisableCache();
void gpio_set_output(S32 gpio);
void gpio_high(S32 gpio);
void gpio_low(S32 gpio);
#if(CALC_CPU_CLOCK)
void CalcCpuClock();
#endif
//---------- functions definition --------------------------------------------//
char _LOADER_VER_[30];
void GetVersion(void);

static inline int sprintf(U08* p1, U08* p2, const char *title, U32 way, U32 addr, U32 result)
{
  gps1sputs("$ER:", 4, mybaud);
  gps1sputs(title, 0, mybaud);
  DebugOutput0(" ram", way, 0, 0);
  if (way == 0)
  {
    gps1sputs("0", 1, mybaud);
  }
  DebugOutput0(" addr ", addr, 0, 0);
  DebugOutput0(" ", result, 0, 1);

  return 0;
}

static inline void UART_PutLine(U08 a, U08* b, int len)
{
  gpio_low(2);
  return;
}

static inline unsigned int get_asi2(unsigned int addr)
{
  unsigned int tmp;

  asm volatile(" lda [%1]2, %0 "
      : "=r"(tmp)
      : "r"(addr)
  );
  return tmp;
}

static inline void set_asi2(unsigned int addr, unsigned int data)
{
  asm volatile(" sta %1, [%0]2 "
      :
      : "r"(addr),"r"(data)
  );
}

static inline unsigned int get_idata(unsigned int addr)
{
  unsigned int tmp;

  asm volatile(" lda [%1]0xD, %0 "
      : "=r"(tmp)
      : "r"(addr)
  );
  return tmp;
}

static inline void set_idata(unsigned int addr, unsigned int data)
{
  asm volatile(" sta %1, [%0]0xD "
      :
      : "r"(addr),"r"(data)
  );
}

#define SCAN_PATTERN
#define CHECKER_PATTERN
#define MC_PATTERN
#define MC_PLUS_PATTERN
//#define PSE_ON_OFF
//#define FAKE_TEST

int main()
{
  InitSystem();
  gps1sputs("END\0", 4, mybaud);
  
  GetVersion();
  gps1sputs(_LOADER_VER_, 0, mybaud);
 
  *((volatile U32*) 0x2000f078) = *((volatile U32*) 0x2000f078) | 0x102; //DIS_UART2_OUT, GPIO2 default used for UART2 Out.
  gpio_set_output(2);
  gpio_high(2);
#if(CALC_CPU_CLOCK)
  CalcCpuClock();
#endif

  if (0)
  {
    U32 i = 0;

    //PLL setting for V8 IC
    U32 tmp = *(volatile U32*) 0x2000f05C;
    tmp &= ~0xFF;
    tmp |= (10 * 4); // pllclk = 16.368*25 failed, freq too high!!
    tmp |= BIT14; //pll reset
    *(volatile U32*) 0x2000f05C = tmp;
    tmp &= ~BIT14;
    *(volatile U32*) 0x2000f05C = tmp; //pll reset deassert
    while (1) //polling until pll ready
    {
      tmp = *(volatile U32*) 0x2000f05C;
      if (tmp & BIT15)
        break;
      i++;
      if (0 && i >= 0x1fffffff)
        break;
    }
    *(volatile U32*) 0x2000F00C = (((*(volatile U32*) 0x2000F00C & 0xfffffff0UL)) | 0x13); // change CPU clock to pllclk/2
  }

  //InitUart(5);

  U08 buf[DL_BYTES];
  WD_disable();
  //For Glonass
  *(volatile U32*) 0x2000F00C = (((*(volatile U32*) 0x2000F00C & 0xfffffff0UL)) | 0x19); // change CPU clock to pllclk/3
  //For GPS
  //  *(volatile U32*)0x2000F00C = (((*(volatile U32*)0x2000F00C & 0xfffffff0UL)) | 0x13) ; // change CPU clock to pllclk/2
  U32 count = 0;

  *(volatile U32*) 0x20004c18 = (0x3cffffffUL); // enable all TM/PSE clocks
  *(volatile U32*) 0x20004c3c = (0x0008ffffUL); // enable GAL PSE mode
  *(volatile U32*) 0x20004c40 = (0xffffffffUL); // enable GAL TM clock

  // coh_it, incoh_it...
  *(volatile U32*) 0x20004d0c = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004d3c = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004d6c = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004d9c = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004dcc = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004e0c = (0xffff60ffUL); // enable PSE
  *(volatile U32*) 0x20004e3c = (0xffff60ffUL); // GAL enable PSE
  *(volatile U32*) 0x20004e6c = (0xffff60ffUL); // GAL enable PSE

  // search start freq offset
  *(volatile U32*) 0x20004d10 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004d40 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004d70 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004da0 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004dd0 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004e10 = (0x00000000UL); // enable PSE
  *(volatile U32*) 0x20004e40 = (0x00000000UL); // GAL enable PSE
  *(volatile U32*) 0x20004e70 = (0x00000000UL); // GAL enable PSE

  for (;;)
  {
    int way, line, word, ilinesz, nlines, nways, itaglow, loop, result;
    unsigned int cctrl, iccr, tmp = 0, addr;
    int len = 0;


    iccr = get_asi2(8); /* IC config register */
    ilinesz = (1 << ((iccr >> 16) & 7));
    /* number of lines = 2^WSIZE * 256 words per KiB / words per line */
    nlines = 1 << (((iccr >> 20) & 0xF) + 8 - ((iccr >> 16) & 7));
    nways = ((iccr >> 24) & 3) + 1;
    itaglow = 10 + ((iccr >> 20) & 0xF);

    /* Disable i-cache */
    cctrl = get_asi2(0);
    set_asi2(0, cctrl & ~0x03);
#if(!LONG_OUTPUT)
    DebugOutput("C,", count);   
#endif

    for (loop = 0; loop < 20; loop++)
//    for (loop = 0; loop < 100; loop++)
    {
#if(LONG_OUTPUT)
      if(loop % 10 == 0)
      {
        DebugOutput("iCache_Test_CountAAAAAAAAAAAAAA,", count);   
      } 
#endif
      if (loop % 4 == 0)
//    if (loop == 40 )
      {
        *(volatile U32*) 0x20004c18 = (0x00000000UL); // disable all TM/PSE clocks
      }
      else
//    else if (loop == 60 )
      {
        *(volatile U32*) 0x20004c18 = (0x3cffffffUL); // enable all TM/PSE clocks
        *(volatile U32*) 0x20004d18 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004d48 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004d78 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004da8 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004dd8 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004e18 = (0x00000001UL); // enable PSE
        *(volatile U32*) 0x20004e48 = (0x00000001UL); // GAL enable PSE
        *(volatile U32*) 0x20004e78 = (0x00000001UL); // GAL enable PSE
      }
      
#ifdef FAKE_TEST     
      for(len=0; len<2000; len++)
      { 
         asm("nop;");
      }
      continue;
#endif
      ////Marge Start//////////////////////////////////////////////////////////////
#ifdef SCAN_PATTERN
      // 0000
      tmp = 0x00000000UL;
      /* For each way */
      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, tmp);

          }

        }
      }

      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "SCAN UP R0", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
          }

        }
      }

      // FFFF
      tmp = 0xFFFFFFFFUL;
      /* For each way */
      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, tmp);

          }

        }
      }

      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "SCAN DN R1", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
          }

        }
      }
#endif
#ifdef CHECKER_PATTERN
      // AA55
      tmp = 0xAA55AA55UL;
      /* For each way */
      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, tmp);
          }

        }
      }

      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "CHECK UP RAA55", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
          }

        }
      }

      // 55AA
      tmp = 0x55AA55AAUL;
      /* For each way */
      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, tmp);
          }

        }
      }

      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "CHECK DN R55AA", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
          }

        }
      }
#endif
#ifdef MC_PATTERN
      // March C-
      // 0000
      tmp = 0x00000000UL;
      /* For each way */
      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, tmp);
          }

        }
      }

      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC- UP R0W1", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, 0xFFFFFFFFUL);
          }

        }
      }

      tmp = 0xFFFFFFFFUL;
      for (way = 0; way < nways; way++)
      {
        /* For each cache line */
        for (line = 0; line < nlines; line++)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = 0; word < ilinesz; word++)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC- UP R1W0", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, 0x00000000UL);
          }

        }
      }

      tmp = 0x00000000UL;
      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC- DN R0W1", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, 0xFFFFFFFFUL);
          }

        }
      }

      tmp = 0xFFFFFFFFUL;
      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC- DN R1W0", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
            /* Store using diagnostic accesses */
            set_idata(addr + word * 4, 0x00000000UL);
          }

        }
      }

      tmp = 0x00000000UL;
      for (way = nways - 1; way >= 0; way--)
      {
        /* For each cache line */
        for (line = nlines - 1; line >= 0; line--)
        {

          addr = (way << itaglow) | (line << 4);

          /* For each data word in the cache line */
          for (word = ilinesz - 1; word >= 0; word--)
          {
            /* Read back and check */
            result = get_idata(addr + word * 4);
            if (tmp != result)
            {
              len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC- DN R0", way, (line << 2 | word), result);
              UART_PutLine(0, buf, len);
              //asm(" ta 1");
            }
          }

        }
      }
#endif

#ifdef MC_PLUS_PATTERN
      // March C+
      { // MC+ "ffff" Begin
        // 0000
        tmp = 0x00000000UL;
        /* For each way */
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, tmp);
            }

          }
        }

        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xFFFFFFFFUL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xFFFFFFFFUL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xFFFFFFFFUL;
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x00000000UL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x00000000UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x00000000UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xFFFFFFFFUL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xFFFFFFFFUL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xFFFFFFFFUL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x00000000UL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x00000000UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x00000000UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }
      } // MC+ "ffff" End

      { // MC+ "aa55" Begin
        // 0000
        tmp = 0xaa55aa55;
        /* For each way */
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, tmp);
            }

          }
        }

        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x55aa55aa);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x55aa55aa)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x55aa55aa;
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xaa55aa55);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xaa55aa55UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xaa55aa55UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x55aa55aaUL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x55aa55aaUL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x55aa55aaUL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xaa55aa55UL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xaa55aa55UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xaa55aa55UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }
      } // MC+ "ffff" End

      { // MC+ "5555" Begin
        // 0000
        tmp = 0x55555555UL;
        /* For each way */
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, tmp);
            }

          }
        }

        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xaaaaaaaaUL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xaaaaaaaaUL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xaaaaaaaaUL;
        for (way = 0; way < nways; way++)
        {
          /* For each cache line */
          for (line = 0; line < nlines; line++)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = 0; word < ilinesz; word++)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x55555555UL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x55555555UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ UP R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x55555555UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0xaaaaaaaaUL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0xaaaaaaaaUL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0W1R1-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0xaaaaaaaaUL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R1", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
              /* Store using diagnostic accesses */
              set_idata(addr + word * 4, 0x55555555UL);

              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (result != 0x55555555UL)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R1W0R0-R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }

        tmp = 0x55555555UL;
        for (way = nways - 1; way >= 0; way--)
        {
          /* For each cache line */
          for (line = nlines - 1; line >= 0; line--)
          {

            addr = (way << itaglow) | (line << 4);

            /* For each data word in the cache line */
            for (word = ilinesz - 1; word >= 0; word--)
            {
              /* Read back and check */
              result = get_idata(addr + word * 4);
              if (tmp != result)
              {
                len = sprintf(buf, "%-.20s ram%x addr %x %x\r\n", "MC+ DN R0", way, (line << 2 | word), result);
                UART_PutLine(0, buf, len);
                //asm(" ta 1");
              }
            }

          }
        }
      } // MC+ "5555" End
#endif
    } //for (loop = 0; loop < 20; loop++)
    //Marge End//////////////////////////////////////////////////////////////
    count++;
  } //for(;;)
  gps1sputs("END", 0, mybaud);
  Error4:
  //while(1);
  gps1sputs("Error4", 0, mybaud);
  while (1)
    ;
}

void WD_disable()
{
  *((volatile unsigned long *) (0x20001014)) = ((*((volatile unsigned long *) (0x20001014))) | 0x00000001);
}

unsigned long iord(unsigned long addr)
{
  volatile unsigned long *ioreg32 = (volatile unsigned long *) REG_BASE_MEMORYIO_ADDR;
  unsigned long out;

  //disable_irq(7);

  out = ioreg32[addr / 4];

  //enable_irq(7);
  return (out);

}
void iowr(unsigned long addr, unsigned long data)
{
  volatile unsigned long *ioreg32 = (volatile unsigned long *) REG_BASE_MEMORYIO_ADDR;

  //  disable_irq(7);

  ioreg32[addr / 4] = data;

  //enable_irq(7);

}

int sysProgramDataPoll(FLASH_DEF * pFA, /* programmed address to poll */
FLASH_DEF value /* data programmed to poll address */
)
{
  int retVal = 1;

  int vBit; /* programmed value of DQ7 */

  /* is this big/little endian compatible ? */
  vBit = 1; //Q7(value);

  while (Q7(*pFA) != vBit)
  {

    //if (Q6(*pFA) == 1)  /* suspend ? */
    //{
    // return 0;
    //}
  }

  return (retVal);
}

int sysClearStatusReg(FLASH_DEF * pFA)
{
  *pFA = JS_CMD_CLEAR_STATUS_REG;
  //*pFA = value;
}

int unlockblock(FLASH_DEF * pFA, /* Sector start address */
unsigned short flashType /* type of flash memory on-board */
)
{
  int retVal = 1;
  //flashType=JS28F256P30T;
  switch (flashType) {

  case (A29L400AUV):
  case (FJ29LV400B):
  case (A29LV400AT):
  case (A29LV800AT):
    break;
  case (JS28F256P30T):
    //pFA = FLASH_CAST (addr);
    *pFA = JS_CMD_BLOCK_UNLOCK;
    *pFA = JS_DATA_BLOCK_UNLOCK;
    *pFA = 0x00ff;

    break;

  default:
    retVal = 3;
  }
  return retVal;
}

static void SPI_S_set_rd_buff_ready()
{
  *((volatile unsigned long *) (SPI_S_MAIN)) = *((volatile unsigned long *) (SPI_S_MAIN)) | 0x4;
}

void gps1sputs(const char *s, int s_len, U32 mybaud)
{
  if (mybaud < 9)
  {
    unsigned long len = 0;
    if (s_len > 0)
    {
      for (len = 0; len < s_len; len++)
      {
        //if(((iord(UART1_TBBC)&0x1f) !=0x0))
        while (((iord(UART1_TBBC + uart_offset) & 0x1f) == 0x0))
          ;
        iowr(UART1_THR + uart_offset, *s++);
      }
      //while (((iord (UART1_TBBC)&0x1f)!=0x10))   ;
      //while((iord(UART1_CESR)&0x8)==0);
    }
    else
    {
      while (*s)
      {
        if (((iord(UART1_TBBC + uart_offset) & 0x1f) != 0x0))
        {
          //while (((iord (UART1_PISR)&0x02)==0));
          iowr(UART1_THR + uart_offset, *s++);
        }
      }
      while (((iord(UART1_TBBC + uart_offset) & 0x1f) == 0))
        ;
      //iowr(UART1_THR+uart_offset, '\0');
    }
  } //if(mybaud<9)
}

int gps1sgetsbin(unsigned char *s, int size, U32 mybaud)
{
  if (mybaud < 9)
  {
    unsigned long binlength = 0;
    do
    {
      U32 status = iord(UART1_RBBC + uart_offset);
      unsigned long slength = status & 0x1f;
      while (slength == 0)
      {
        status = iord(UART1_RBBC + uart_offset);
        slength = status & 0x1f;
      }

      unsigned long i, j;
      for (j = 0; j < slength; j++)
      {
        i = iord(UART1_RBR + uart_offset);
        *s = (unsigned char) (i & 0xff);
        ++binlength;
        if (binlength == size)
        {
          return binlength;
        }
        s++;
      }
    } while (1);
  }
}

VOID UART_init(U08 baudidx)
{
  U32 register_baud;
  U32 baudrate[9] = { 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };
  //U32 register_baud;

  U32 reg_val2;
  U08 uart_boost;
  reg_val2 = *(volatile unsigned long *) (0x2000f004);
  if ((baudidx == 6) || (baudidx == 7))
  {
    reg_val2 = (reg_val2 & 0x3fffffff) | 0x40000000;
    uart_boost = 2;
  }
  else if ((baudidx == 8))
  {

    reg_val2 = (reg_val2 & 0x3fffffff) | 0x80000000;
    uart_boost = 4;
  }
  else  //set back
  {
    reg_val2 = reg_val2 & 0x3fffffff;
    uart_boost = 1;
  }
  *(volatile unsigned long *) (0x2000f004) = reg_val2;

  U32 f08_val;
  *((volatile unsigned long *) (0x2000f018)) = 0x20000;
  f08_val = *((volatile unsigned long *) (0x2000f018));
  while ((f08_val & 0x20000) != 0)
  {
    f08_val = *((volatile unsigned long *) (0x2000f018));
  };  //the bit 17 case not zero should wait to go further
  if (((f08_val & 0x20000) == 0) && ((f08_val & 0x10000) == 0))
  {
    f08_val = 61378751 * uart_boost;
  }
  else if (((f08_val & 0x20000) == 0) && ((f08_val & 0x10000) != 0))
  {
    f08_val = f08_val & 0xffff;
    f08_val = f08_val / 8;
    f08_val = f08_val * 32768;
  }
  else
  {
    //should not come here
    f08_val = 61378751 * uart_boost;
  }

  //register_baud=((f08_val/115200)*reg_val)/16;
  register_baud = ((f08_val / baudrate[baudidx])) / 16;

  iowr(UART1_LCR + uart_offset, 0x83); // set DLAB=1, set transaction bits=8
  iowr(UART1_DLL + uart_offset, register_baud & 0xff); // now uart uses 50MHz clk, so, 50M/(px51 * 0x10) = 38400Hz (baud rate)
  iowr(UART1_DLM + uart_offset, (register_baud >> 8) & 0xff);
  iowr(UART1_LCR + uart_offset, 0x03); // set DLAB=0
  iowr(UART1_FCR + uart_offset, 0x86); // 1. reset TX/RX FIFO,  2. set RX FIFO trigger level to 8 bytes
  //iowr (UART1_IER, 0x03); // set interrupt enables, 0x03==> TX/RX fifo status

  while ((iord(UART1_CESR + uart_offset) & 0x6) != 0)
    ;

}

int new_fifouart_wr_buf(char* buf, int byte_cnt)
{
  int i;
  //for (i=0;i<byte_cnt;i++)
  {
    iowr(UART1_THR + uart_offset, buf[byte_cnt]);
  }
}

void InitSystem()
{
  Register tmp;
  volatile int *lreg = (volatile int *) REG_BASE_ADDR;

  tmp = lreg[MCFG1 / 4];
  lreg[MCFG1 / 4] = (tmp & 0xffff0000) | (FLASH_WIDTH / FLASH_CHIP_WIDTH) << 8 | 0x877;

  if (((*(volatile unsigned long *) (0x2000f010)) & 0x80000000) != 0)	//leon2
  {
    lreg[IMASK_LEON2 / 4] = 0x0; /* mask all interrupt */
    //cache keep to backward to ML605 leon2 board
    tmp = lreg[CACFG / 4]; /* disable and flush cache */
    lreg[CACFG / 4] = (tmp | 0x600000) & 0xfffffff0;
  }
  else
  {
    lreg[IMASK / 4] = 0x0; /* mask all interrupt */
  }

  tmp = lreg[MCFG1 / 4];
  lreg[MCFG1 / 4] = (tmp & 0xffff) | 0x10080000; // 15 wait state
  //cpu boost

  if (((*(volatile unsigned long *) (0x2000f000)) & 0x1) != 0)	//uart 0
  {
    uart_offset = 0;
  }
  else
  {
    uart_offset = 0x100;
  }

  //check boot from where
  tmp = (*(volatile unsigned long *) (0x2000f010));
  chipmode = (tmp >> 20) & 0x7;
} //InitSystem()

void InitFlashWriteMode()
{
  Register tmp;
  if (chipmode == 4) //external flash, make it as old way, write Flash
  {
    tmp = *(volatile unsigned long *) (0x20004c18);
    *(volatile unsigned long *) (0x20004c18) = tmp & 0xfeffffff;

    tmp = *(volatile unsigned long *) (0x20004c1c);
    *(volatile unsigned long *) (0x20004c1c) = tmp | 0x10000;
  }
}

void InitUart(int baud)
{
  if (baud < 9)
  {
    while ((iord(UART1_CESR + uart_offset) & 0x8) == 0)
      ;
    UART_init(baud);
  }
}

U32 GetInt(const int start, unsigned char* buf)
{
  U32 data = 0;
  static U08 i = 0;

  if (-1 == start)
  {
    i = 0;
    return 0;
  }

  i += start;
  for (; i < 100; i++)
  {
    if (buf[i] != ' ')
    {
      data = data * 10 + buf[i] - 0x30;
    }
    else
    {
      break;
    }
  }
  return data;
}

void DebugOutput0(const char *title, U32 data, U08 addDollor, U08 addNewLine)
{
  char buf[128];
  int i = 0;
  if (addDollor)
  {
    buf[0] = '$';
  }
  while (title[i])
  {
    if (addDollor)
    {
      buf[i + 1] = title[i];
    }
    else
    {
      buf[i] = title[i];
    }
    ++i;
  }
  if (addDollor)
  {
    i++;
  }
  unsigned long long m = 0x0F;
  int j = 0;
  U08 zeroPrefix = 1;
  for (; j < 8; ++j)
  {
    char c = (data >> (7 - j) * 4) & m;
    if (zeroPrefix && c)
    {
      zeroPrefix = 0;
    }
    if (zeroPrefix)
    {
      continue;
    }
    if (c > 9)
    {
      buf[i] = c - 10 + 'A';
    }
    else
    {
      buf[i] = c + '0';
    }
    ++i;
  }
  if (addNewLine)
  {
    buf[i] = 0x0D;
    buf[i + 1] = 0x0A;
    buf[i + 2] = 0;
  }
  else
  {
    buf[i] = 0x0;
  }
  gps1sputs(buf, 0, mybaud);
}

void DebugOutput(const char *title, U32 data)
{
  if (data == 0)
  {
    gps1sputs("$", 0, mybaud);
    gps1sputs(title, 0, mybaud);
    gps1sputs("0\r\n", 0, mybaud);
  }
  else
  {
    DebugOutput0(title, data, 1, 1);
  }
}

void DebugOutput2(const char *title, U32 addr, U32 result)
{
  DebugOutput0(title, addr, 1, 0);
  DebugOutput0(" ", result, 0, 1);
}

void DebugOutput3(const char *title, U32 way, U32 addr, U32 result)
{
  DebugOutput0("ER:", 0, 1, 0);
  DebugOutput0(title, way, 0, 0);
  DebugOutput0(" addr ", addr, 0, 0);
  DebugOutput0(" ", result, 0, 1);
}

void DisableCache()
{
  Register tmp;
  //disable cache
  asm volatile ("flush");
  asm volatile ("set 0x0, %g1");
  asm volatile ("sta %g1, [%g0] 2");

  //try parallel flash here
  tmp = *(volatile unsigned long *) (0x20004c18);
  *(volatile unsigned long *) (0x20004c18) = tmp & 0xfeffffff;
  tmp = *(volatile unsigned long *) (0x20004c1c);
  *(volatile unsigned long *) (0x20004c1c) = tmp | 0x10000;

}

//---------------------------------------------------------
#define GPIO_OUTPUT_ADDR       (0x20001008UL)
#define GPIO_DIR_ADDR          (0x2000100CUL)
void gpio_set_output(S32 gpio)
{
  volatile unsigned long int *lreg = (volatile unsigned long int *) GPIO_DIR_ADDR;
  unsigned long int reg = *lreg;
  reg &= ~(0x1UL << gpio);
  *lreg = reg;

}

//---------------------------------------------------------
void gpio_high(S32 gpio)
{
  volatile unsigned long int *lreg = (volatile unsigned long int *) GPIO_OUTPUT_ADDR;
  unsigned long int reg = *lreg;
  reg |= 0x1UL << gpio;
  *lreg = reg;
}

//---------------------------------------------------------
void gpio_low(S32 gpio)
{
  volatile unsigned long int *lreg = (volatile unsigned long int *) GPIO_OUTPUT_ADDR;
  unsigned long int reg = *lreg;
  reg &= ~(0x1UL << gpio);
  *lreg = reg;
}

void CalcCpuClock()
{
  U32 reg_val2;
  U32 f08_val;
  U32 val, pllclk_freq;

  *((volatile unsigned long *) (0x2000f018)) = 0x20000;
  
  f08_val = *((volatile unsigned long *) (0x2000f018));
//DebugOutput("f08_val,", f08_val);   
  while ((f08_val & 0x20000) != 0)
  {
    f08_val = *((volatile unsigned long *) (0x2000f018));
  };  //the bit 17 case not zero should wait to go further
  DebugOutput("f08_val,", f08_val);   
  if (((f08_val & 0x20000) == 0) && ((f08_val & 0x10000) == 0))
  {
    DebugOutput("Set CPU clock to pllclk/", 3);   
    *(volatile U32*)0x2000F00C = (((*(volatile U32*)0x2000F00C & 0xfffffff0UL)) | 0x19) ; // change CPU clock to pllclk/3 
    return;
  }
  else if (((f08_val & 0x20000) == 0) && ((f08_val & 0x10000) != 0))
  {
//DebugOutput("CalcCpuClock,", 2);   
    reg_val2 = *(volatile unsigned long *) (0x2000f004);
    val = (f08_val & 0xffff) >> 3;
DebugOutput("reg_val2 & 0xc0000000,", (reg_val2 & 0xc0000000));   
    if((reg_val2 & 0xc0000000) == 0)
    {
      pllclk_freq = (val * 32768) << 2;
    }
    else if((reg_val2 & 0xc0000000) == 0x40000000)
    {
      pllclk_freq = (val * 32768) << 1;
    }
    else
    {
      pllclk_freq = (val * 32768);
    }
    DebugOutput("pllclk_freq,", pllclk_freq);   
  }
  
  if(pllclk_freq < 275000000)
  {
    DebugOutput("Set CPU clock to pllclk/", 2);   
    *(volatile U32*)0x2000F00C = (((*(volatile U32*)0x2000F00C & 0xfffffff0UL)) | 0x13) ; // change CPU clock to pllclk/2
  } 
  else
  {
    DebugOutput("Set CPU clock to pllclk/", 3);   
    *(volatile U32*)0x2000F00C = (((*(volatile U32*)0x2000F00C & 0xfffffff0UL)) | 0x19) ; // change CPU clock to pllclk/3
  } 

}

void GetVersion(void)
{
  _LOADER_VER_[0] = '$';
  _LOADER_VER_[1] = 'L';
  _LOADER_VER_[2] = 'O';
  _LOADER_VER_[3] = 'A';
  _LOADER_VER_[4] = 'D';
  _LOADER_VER_[5] = 'E';
  _LOADER_VER_[6] = 'R';
  _LOADER_VER_[7] = ',';
  _LOADER_VER_[8] = REV_ID[0];
  _LOADER_VER_[9] = REV_ID[1];
  _LOADER_VER_[10] = REV_ID[2];
  _LOADER_VER_[11] = REV_ID[3];
  _LOADER_VER_[12] = REV_ID[4];
  _LOADER_VER_[13] = ',';
  _LOADER_VER_[14] = __DATE__[9];
  _LOADER_VER_[15] = __DATE__[10];
  _LOADER_VER_[16] = BUILD_MONTH[0];
  _LOADER_VER_[17] = BUILD_MONTH[1];
  _LOADER_VER_[18] = __DATE__[4];
  _LOADER_VER_[19] = __DATE__[5];
  _LOADER_VER_[20] = __TIME__[0];
  _LOADER_VER_[21] = __TIME__[1];
  _LOADER_VER_[22] = __TIME__[3];
  _LOADER_VER_[23] = __TIME__[4];
  _LOADER_VER_[24] = __TIME__[6];
  _LOADER_VER_[25] = __TIME__[7];
  _LOADER_VER_[26] = 0x0d;
  _LOADER_VER_[27] = 0x0a;
  _LOADER_VER_[28] = 0;
}