#define USE_TI89
#define USE_TI92P
#define USE_V200
#define SAVE_SCREEN
#include <tigcclib.h>
void _main(void) {
 unsigned long *p=(unsigned long *)4;
 unsigned long firstlong=*(unsigned long *)0;
 clrscr();
 try_again:
 while (*(p++)!=firstlong) {
  if ((unsigned long)p>=0x600000) return;
 }
 if (memcmp(p-1,0,1000)) goto try_again;
 printf("%lp\n",p-1);
 ngetchx();
 goto try_again;
}
