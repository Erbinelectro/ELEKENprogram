#include"mbed.h"

BusOut x(p20, p19, p18, p17, p16);//ser
DigitalOut a(p22);//rclk
DigitalOut b(p23);//srclk
DigitalOut c(p24);//srclr

void srclk(){
    b=1; wait_us(1); b=0;
}

void output(char outputdata,long time){
    char t0,t1,t2,t3,t4,t5,t6,t7;
    
    t0=outputdata>>7;//上位ビット
    t1=outputdata<<1; t1/=128;
    t2=outputdata<<2; t2/=128;
    t3=outputdata<<3; t3/=128;
    t4=outputdata<<4; t4/=128;
    t5=outputdata<<5; t5/=128;
    t6=outputdata<<6; t6/=128;
    t7=outputdata<<7; t7/=128;
    
    for(long i=0; i<time/20; i++){
        srclk(); x=1; c=1;
        srclk(); x=0; c=1;
        
        a=(int)t0; wait_us(1); a=0; srclk();
        a=(int)t1; wait_us(1); a=0; srclk();
        a=(int)t2; wait_us(1); a=0; srclk();
        a=(int)t3; wait_us(1); a=0; srclk();
        a=(int)t4; wait_us(1); a=0; srclk();
        a=(int)t5; wait_us(1); a=0; srclk();
        a=(int)t6; wait_us(1); a=0; srclk();
        a=(int)t7; wait_us(1); a=0;
        
        c=0; wait_us(1); c=1;
        srclk();
        x=1; c=1; srclk(); 
        x=0; c=1; //20usかかる
    }
}

int main(){
    x=0; a=0; b=0; c=0;
    while(1){
        for(int num = 1; num <= 5; num++{
            for(int i = 0; i < 8; i++){
                x = num;
                output(1 << i, 100);
            }
        }
    }
}
