extern "C" {
#include <Windows.h>

BYTE inportb(UINT portid);
void outportb(UINT portid, BYTE value);
}