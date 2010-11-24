// Free open-source Grom library

#ifndef __Grom_System_Utils_h__
#define __Grom_System_Utils_h__

#include "Config.h"

namespace Sys
{

void RunTimeAssert(bool statement);
void ValidateIndex(int index, int count);
void ValidateRange(int index, int range, int count);
int NormalizeRange(int index, int range, int count);
void Sleep(int milliseconds);

} // namespace Sys

#endif //__Grom_System_Utils_h__
