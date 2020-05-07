#include <stdio.h>
#include <DOS.h>

struct VIDEO
{
    unsigned char symb;
    unsigned char attr;
};

void get_reg();
void print(int);

void interrupt (*int8) (...);
void interrupt (*int9) (...);
void interrupt (*int10) (...);
void interrupt (*int11) (...);
void interrupt (*int12) (...);
void interrupt (*int13) (...);
void interrupt (*int14) (...);
void interrupt (*int15) (...);

void interrupt int68 (...) { get_reg(); int8(); }
void interrupt int69 (...) { get_reg(); int9(); }
void interrupt int70 (...) { get_reg(); int10(); }
void interrupt int71 (...) { get_reg(); int11(); }
void interrupt int72 (...) { get_reg(); int12(); }
void interrupt int73 (...) { get_reg(); int13(); }
void interrupt int74 (...) { get_reg(); int14(); }
void interrupt int75 (...) { get_reg(); int15(); }

void print(int value)
{
    char temp;
    int i;
    VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);

    for(i=7;i>=0;i--)
    {
	temp = value%2;
	value/=2;
	screen->symb=temp+'0';
	screen->attr=0x5E;
	screen++;
    }
}

void get_reg()
{
    outp(0x20, 0x0B);
    print(inp(0x20));
}

void init()
{
    int8 = getvect(0x08);
    int9 = getvect(0x09);
    int10 = getvect(0x0A);
    int11 = getvect(0x0B);
    int12 = getvect(0x0C);
    int13 = getvect(0x0D);
    int14 = getvect(0x0E);
    int15 = getvect(0x0F);

    setvect(0x70, int68);
    setvect(0x71, int69);
    setvect(0x72, int70);
    setvect(0x73, int71);
    setvect(0x74, int72);
    setvect(0x75, int73);
    setvect(0x76, int74);
    setvect(0x77, int75);

    _disable();

    outp(0x20, 0x11);
    outp(0x21, 0x70);
    outp(0x21, 0x04);
    outp(0x21, 0x01);

    _enable();
}

int main()
{
    unsigned far *fp;
    init();

    FP_SEG (fp) = _psp;
    FP_OFF (fp) = 0x2c;

    _dos_freemem (*fp);

    _dos_keep(0, (_DS - _CS) + (_SP/16) + 1);

    return 0;
}