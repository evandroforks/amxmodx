#ifndef _INCLUDE_LIBRARIES_H
#define _INCLUDE_LIBRARIES_H

#include <string.h>
#include "amxmodx.h"
#include "CString.h"

enum LibSource
{
	LibSource_Plugin,
	LibSource_Module
};

enum LibType
{
	LibType_Library,
	LibType_Class
};

struct Library
{
	String name;
	LibSource src;
	LibType type;
	void *parent;
};

enum LibCmd
{
	LibCmd_ReqLib,
	LibCmd_ReqClass,
	LibCmd_ForceLib,
	LibCmd_ExpectLib,
	LibCmd_ExpectClass,
	LibCmd_DefaultLib,
};

enum LibError
{
	LibErr_None = 0,
	LibErr_NoLibrary,
	LibErr_NoClass,
};

class LibDecoder
{
public:
	LibDecoder() : buffer(NULL)
	{
	}
	~LibDecoder()
	{
		free(buffer);
		buffer = NULL;
		param1 = NULL;
		param2 = NULL;
	}
	char *buffer;
	char *param1;
	char *param2;
	LibCmd cmd;
};

bool AddLibrary(const char *name, LibType type, LibSource src, void *parent=NULL);
bool DecodeLibCmdString(const char *str, LibDecoder *cmd);
size_t AddLibrariesFromString(const char *name, LibType type, LibSource src, void *parent=NULL);
size_t ClearLibraries(LibSource src);
LibError RunLibCommand(const LibDecoder *enc);
size_t RemoveLibraries(void *parent);
bool FindLibrary(const char *name, LibType type);


#endif //_INCLUDE_LIBRARIES_H
