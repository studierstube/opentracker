#include <Windows.h>

BYTE inportb(UINT portid)
{
    unsigned char value;
    __asm mov edx,portid
    __asm in al,dx
    __asm mov value,al
    return value;
}

void outportb(UINT portid, BYTE value)
{
    __asm mov edx,portid
    __asm mov al,value
    __asm out dx,al
}
