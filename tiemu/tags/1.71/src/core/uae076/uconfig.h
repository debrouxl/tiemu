 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * User configuration options
  *
  * Copyright 1995, 1996 Bernd Schmidt
  */

/*
 * Please note: Many things are configurable with command line parameters,
 * and you can put anything you can pass on the command line into a 
 * configuration file ~/.uaerc. Please read the documentation for more
 * information.
 */

/*
 * Define this when you are compiling UAE for the first time. If it works, you
 * can try to undefine it to get (much) better performance. It does not seem
 * to work on all machines, though.
 */
#define DONT_WANT_SHM

/*
 * If you are running UAE over the network on a remote X server, this can
 * boost performance quite a bit. It can even boost performance on a 
 * non-networked system.
 */
#define LOW_BANDWIDTH

/*
 * The blitter emulator contains some optimization methods that are, strictly
 * speaking, invalid, but very unlikely to break anything. 
 * You can set the optimization level from 0 (normal emulation) to 3 (max.
 * optimization). 
 * Methods 2 and 3 are really a lot faster with games and demos.
 * Careful: method 3 will break most RISC chips. You'll see bus errors.
 */
#define FAST_BLITTER 2

/*
 * Define this if you have the AF System and want sound in UAE.
 * You also have to set the right paths in the Makefile.
#define AF_SOUND
 */

/*
 * When these two are enabled, a subset of the ECS features is emulated.
 * Actually, it's only the chip identification and big blits. This may be
 * enough to persuade some ECS programs to run.
 * DON'T enable SuperHires or Productivity modes. They are not emulated,
 * and will look very messy. NTSC doesn't work either.
 */
#define ECS_AGNUS
#define ECS_DENISE

#ifdef __linux /* This line protects you if you don't use Linux */
/***************************************************************************
 * Linux specific options. Ignore these if you are using another OS.
 */

/*
 * Define if you have installed the Linux sound driver and if you have read
 * the section about sound in the README.
 * Turn off sound at run-time with the "-S" option.
#define LINUX_SOUND
 */

/*
 * Try defining this if you don't get steady sound output. 
#define LINUX_SOUND_SLOW_MACHINE
 */

#endif /* __linux */

/***************************************************************************
 * Support for broken software. These options are set to default values
 * that are reasonable for most uses. You should not need to change these.
 */

/*
 * Some STUPID programs access a longword at an odd address and expect to
 * end up at the routine given in the vector for exception 3.
 * (For example, Katakis does this). And yes, I know it's legal, but it's dumb
 * anyway.
 * If you leave this commented in, memory accesses will be faster,
 * but some programs may fail for an obscure reason.
 */
#define NO_EXCEPTION_3

/*
 * If you want to see the "Hardwired" demo, you need to define this.
 * Otherwise, it will say "This demo don't like Axel" - apparently, Axel
 * has a 68040.
#define WANT_SLOW_MULTIPLY
 */

