#ifndef _AMXXSC_INCLUDE_H
#define _AMXXSC_INCLUDE_H

#define VERSION_STRING		"1.50-300"
#define VERSION				03000
#define MAGIC_HEADER		0x414D5842
#define MAGIC_HEADER2		0x414D5858
#define	MAGIC_VERSION		0x0300

#ifdef __linux__
# include <dlfcn.h>
#else
# include <windows.h>
#endif

#include <string.h>

#ifdef __linux__
# define dlmount(x)		dlopen(x, RTLD_NOW)
  typedef void*			HINSTANCE;
#else
# define dlsym(x, s)	GetProcAddress(x, s)
# define dlmount(x)		LoadLibrary(x)
# define dlclose(x)		FreeLibrary(x)
#endif

#include "zlib.h"
#include "Binary.h"

typedef int	(*COMPILER)(int argc, char **argv);
typedef int (*PRINTF)(const char *message, ...);

char *FindFileName(int argc, char **argv);
char *swiext(const char *file, const char *ext);
void show_help();


struct ablhdr
{
	int magic;
	char size;
};

struct abl
{
	long stp;
	char cellsize;
	int size;
	long cmpsize;
	char *data;
	char *cmp;
};

struct BinHeader
{
	int32_t magic;
	int16_t version;
	int8_t plugins;
};

struct BinPlugin
{
	int8_t cellsize;	//cell size
	int32_t imagesize;	//uncompressed image size
	int32_t disksize;	//compressed image size
	int32_t memsize;	//memory image size
	int32_t offs;		//file offset
};

#endif //_AMXXSC_INCLUDE_H