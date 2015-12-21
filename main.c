/*
	Author: Brian Sonnie
	Date: 7/12/2008
*/
#define _PSP_FW_VERSION 150
#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspwlan.h>
#include <stdlib.h>
#include <pspchnnlsv.h>
#include <pspctrl.h>
#include <string.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include "main.h"

//Defines
PSP_MODULE_INFO("SonniE PRX", 0x3008, 1, 2); //0x3007
PSP_MAIN_THREAD_ATTR(0); //0 for kernel mode too

//Globals
SceUID thid;
unsigned char gameId[10];
unsigned int running=0;
//*****************SFLS********************
//Loads The Default Text
unsigned int *text1a=(unsigned int*) (0x000eebd4 +0x08800000); 
unsigned int *text2a=(unsigned int*) (0x000d2818 +0x08800000); 
unsigned int *text3a=(unsigned int*) (0x002efa50 +0x08800000); 
unsigned int *text1b=(unsigned int*) (0x000eebf8 +0x08800000); 
unsigned int *text2b=(unsigned int*) (0x000d283c +0x08800000); 
unsigned int *text3b=(unsigned int*) (0x002efa74 +0x08800000); 
//Keyboard Text
unsigned int *key1a=(unsigned int*) (0x000eebc4 +0x08800000); 
unsigned int *key2a=(unsigned int*) (0x000d280c +0x08800000); 
unsigned int *key3a=(unsigned int*) (0x002efa44 +0x08800000); 
unsigned int *key1b=(unsigned int*) (0x000eebd0 +0x08800000); 
unsigned int *key2b=(unsigned int*) (0x000d2814 +0x08800000); 
unsigned int *key3b=(unsigned int*) (0x002efa4c +0x08800000); 
//******************SFCO*********************
//Loads The Default Text
unsigned int *text1c=(unsigned int*) (0x002f04dc +0x08800000); 
unsigned int *text2c=(unsigned int*) (0x000e3c08 +0x08800000); 
unsigned int *text3c=(unsigned int*) (0x000c53f0 +0x08800000); 
unsigned int *text1d=(unsigned int*) (0x002F0500 +0x08800000); 
unsigned int *text2d=(unsigned int*) (0x000e3c2c +0x08800000); 
unsigned int *text3d=(unsigned int*) (0x000c5414 +0x08800000); 
//Keyboard Text
unsigned int *key1c=(unsigned int*) (0x002f04c8 +0x08800000); 
unsigned int *key2c=(unsigned int*) (0x000e3bf8 +0x08800000); 
unsigned int *key3c=(unsigned int*) (0x000c53e4 +0x08800000); 
unsigned int *key1d=(unsigned int*) (0x002f04d8 +0x08800000); 
unsigned int *key2d=(unsigned int*) (0x000e3c04 +0x08800000); 
unsigned int *key3d=(unsigned int*) (0x000c53ec +0x08800000); 
//******************SFDM*********************
//Loads The Default Text
unsigned int *text1e=(unsigned int*) (0x000e0584 +0x08800000); 
unsigned int *text2e=(unsigned int*) (0x000cd378 +0x08800000); 
unsigned int *text3e=(unsigned int*) (0x000c8aac +0x08800000); 
unsigned int *text1f=(unsigned int*) (0x000e05a8 +0x08800000); 
unsigned int *text2f=(unsigned int*) (0x000cd39c +0x08800000); 
unsigned int *text3f=(unsigned int*) (0x000c8ad0 +0x08800000); 
//Keyboard Text
unsigned int *key1e=(unsigned int*) (0x000e0580 +0x08800000); 
unsigned int *key2e=(unsigned int*) (0x000cd374 +0x08800000); 
unsigned int *key3e=(unsigned int*) (0x000c8aa8 +0x08800000); 
unsigned int *key1f=(unsigned int*) (0x000e059c +0x08800000); 
unsigned int *key2f=(unsigned int*) (0x000cd390 +0x08800000); 
unsigned int *key3f=(unsigned int*) (0x000c8ac8 +0x08800000); 
//Spam Text
unsigned int *spam0=(unsigned int*) (0x00000920 +0x08800000); 
//Keyboard Text
unsigned int *keyboard1=(unsigned int*) (0x00000900 +0x08800000); 
unsigned int *keyboard2=(unsigned int*) (0x00000904 +0x08800000); 
unsigned int *keyboard3=(unsigned int*) (0x00000908 +0x08800000); 
unsigned int *keyboard4=(unsigned int*) (0x0000090c +0x08800000); 
unsigned int *keyboard5=(unsigned int*) (0x00000910 +0x08800000); 


//Functions
int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));

int mainThread()
{
  sceKernelDelayThread(15000000);
  //Variables
  unsigned int nop = 0x00000000; 
  unsigned int luikey = 0x3c040880;
  unsigned int orikey = 0x24840900;
  unsigned int luitext = 0x3c060880;
  unsigned int oritext = 0x24c60920;
  //SFDM
  unsigned int luikeya = 0x3c050880;
  unsigned int orikeya = 0x24a50900;

  signed int fd;
  unsigned int counter=0;
  running=1;
  
  //Wait for the kernel to boot up (if we don't, the PRX will crash since some functions may be missing)
  sceKernelDelayThread(15000000);
  while(!sceKernelFindModuleByName("sceKernelLibrary"))
	sceKernelDelayThread(100000);
	sceKernelDelayThread(100000);
  
  //Find the GAME ID (by reading the UMD_DATA file on the UMD)
  do
  {
  	fd=sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777); 
    sceKernelDelayThread(10000);
  } while(fd<=0);
  sceIoRead(fd, gameId, 10);
  sceIoClose(fd);
  
  //Compare the gameID to see if the game is LS
    if(strncmp(gameId, "UCUS-98606", 10) == 0)
	{
		while(1)
{
	sceKernelDelayThread(150000);
if(*spam0 != nop)
{
	
	*keyboard1 = 0x6E6F5315;
	*keyboard2 = 0x2745696E;
	*keyboard3 = 0x654B2073;
	*keyboard4 = 0x616F6279;
	*keyboard5 = 0x00006472;
	*key1a = luikey;
	*key2a = luikey;
	*key3a = luikey;
	*key1b = orikey;
	*key2b = orikey;
	*key3b = orikey;
	*text1a = luitext;
	*text2a = luitext;
	*text3a = luitext;
	*text1b = oritext;
	*text2b = oritext;
	*text3b = oritext;
	sceKernelDelayThread(20000000);
}
		}
}
  //Compare the gameID to see if the game is CO
    else if(strncmp(gameId, "NPUG-80114", 10) == 0)
	{
	while(1)
	{
		sceKernelDelayThread(1500);
		if(*spam0 != nop)
		{
		//*key1c = luikey;
		*key2c = luikey;
		*key3c = luikey;
		//*key1d = orikey;
		*key2d = orikey;
		*key3d = orikey;
		//*text1c = luitext;
		*text2c = luitext;
		*text3c = luitext;
		//*text1d = oritext;
		*text2d = oritext;
		*text3d = oritext;
		*keyboard1 = 0x6E6F5315;
		*keyboard2 = 0x2745696E;
		*keyboard3 = 0x654B2073;
		*keyboard4 = 0x616F6279;
		*keyboard5 = 0x00006472;
		sceKernelDelayThread(20000000);
		}
	else
	{
	*keyboard1 = 0x6E6F5315;
	*keyboard2 = 0x2745696E;
	*keyboard3 = 0x654B2073;
	*keyboard4 = 0x616F6279;
	*keyboard5 = 0x00006472;
	sceKernelDelayThread(1000000);
	}
   }
	}
	 else if(strncmp(gameId, "UCUS-98641", 10) == 0)
	{
	while(1)
	{
		sceKernelDelayThread(15000);
		if(*spam0 != nop)
		{
			if(*key1e == 0x3c0508dc)
			{
		*key1e = luikeya;
		*key2e = luikeya;
		*key3e = luikeya;
		*key1f = orikeya;
		*key2f = orikeya;
		*key3f = orikeya;
		*text1e = luitext;
		*text2e = luitext;
		*text3e = luitext;
		*text1f = oritext;
		*text2f = oritext;
		*text3f = oritext;
		*keyboard1 = 0x6E6F5315;
		*keyboard2 = 0x2745696E;
		*keyboard3 = 0x654B2073;
		*keyboard4 = 0x616F6279;
		*keyboard5 = 0x00006472;
		sceKernelDelayThread(20000000);
			}
		}
		}
	}
	
else
   {
    running=0;
    return 0;
  }
  


  return 0;
}

int _start(SceSize args, void *argp)
{
	//Create thread
	thid=sceKernelCreateThread("MainThread", &mainThread, 0x18, 0x1000, 0, NULL);
      
  //Start thread
  if(thid >= 0) sceKernelStartThread(thid, 0, NULL);
  
	return 0;
}

int _stop(SceSize args, void *argp)
{
	running=0;
 	sceKernelTerminateThread(thid);
  return 0;
}