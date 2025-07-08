#include <reg51.h>

sbit DO13 = P1^0;
sbit VANG13 = P1^1;
sbit XANH13 = P1^2;

sbit DO24 = P1^3;
sbit VANG24 = P1^4;
sbit XANH24 = P1^5;

sbit LED10 = P3^0;
sbit LED11 = P3^1;
sbit LED20 = P3^6;
sbit LED21 = P3^7;

unsigned char time;
unsigned char r0, r1;
unsigned char r3 = 0;

unsigned char dtime = 15;
unsigned char vtime = 3;
unsigned char xtime = 12;

unsigned char MALED[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 127; j++);
}

void Display7Seg() {
    unsigned char chuc, dv;

    chuc = r1 / 10;
    dv = r1 % 10;
    P0 = MALED[chuc]; LED10 = 1; delay_ms(2); LED10 = 0;
    P0 = MALED[dv];   LED11 = 1; delay_ms(2); LED11 = 0;

    chuc = r0 / 10;
    dv = r0 % 10;
    P2 = MALED[chuc]; LED20 = 1; delay_ms(2); LED20 = 0;
    P2 = MALED[dv];   LED21 = 1; delay_ms(2); LED21 = 0;
}

void Timer0_ISR(void) interrupt 1 {
    TH0 = 0x50; TL0 = 0x38;
    r3++;
    if(r3 >= 20) {
        r3 = 0;
        if(time > 0) time--;
        if(r0 > 0) r0--;
        if(r1 > 0) r1--;
    }
}

void B1() {
    DO24 = 0; VANG13 = 0;
    DO13 = 1; XANH24 = 1;
}

void B2() {
    XANH24 = 0; VANG24 = 1;
}

void B3() {
    DO13 = 0; VANG24 = 0;
    DO24 = 1; XANH13 = 1;
}

void B4() {
    XANH13 = 0; VANG13 = 1;
}


void main() {
    TMOD = 0x21;
    TH0 = 0x50; TL0 = 0x38;
    IE = 0x82;
    TR0 = 1;
    
    P1 = 0x00;
    
    while(1) {
       
        time = xtime; r0 = dtime; r1 = xtime;
        while(time > 0) { Display7Seg(); B1(); }

        time = vtime; r0 = vtime; r1 = vtime;
        while(time > 0) { Display7Seg(); B2(); }

        time = xtime; r0 = xtime; r1 = dtime;
        while(time > 0) { Display7Seg(); B3(); }

        time = vtime; r0 = vtime; r1 = vtime;
        while(time > 0) { Display7Seg(); B4(); }
    }
}
