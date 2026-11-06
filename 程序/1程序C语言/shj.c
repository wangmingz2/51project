#include<reg51.h>

/*******************define*************************/
#define WAIT_SALE	0                 //定义1602显示状态
#define NO_WATER 	1                 
#define INPUT_MONEY	2 
#define SALING		3
#define CHANGE		4
#define CANCLE		5
#define NO_DATA		0
#define water_change(x, y) ((x) & (~(0x01 << y))) //宏 控制水种类的切换 
#define uint unsigned int
#define uchar unsigned char

unsigned char water_total[] = {10, 10, 10, 10, 10, 10, 10, 10};   
unsigned char water_price[] = {1, 2, 3, 4, 5, 6, 7, 8};
uchar code wait_sale[]="for sale";                  //定义所显示的内容
uchar code no_water[] = "no water!";
uchar code price[] = "price:";
uchar code number[] = "num:";
uchar code sum[] = "SUM:";
uchar code input[] = "INPUT:";
uchar code change[] = "change:";
uchar code put_water[] = "put water....";
uchar code cancle[] = "cancle...";

unsigned char e=0x00;                    

sbit dj=P2^4;
sbit led=P2^3;
sbit lcd_en=P2^5;             
sbit rs=P2^7;
sbit rw = P2^6;
sbit P20 = P2^0;
sbit P21 = P2^1;
/****************************************************/

/***********************function**********************************/
/************lcd 1602*************************/
void lcd_1602_delay(uint z)          //延时
{
uint x,y;
for(x=z;x>0;x--)
for(y=110;y>0;y--);
}

void lcd_1602_write_com(uchar com)   //写命令
{
rs=0;
rw=0;
P0=com;
lcd_1602_delay(5);
lcd_en=1;
lcd_1602_delay(5);
lcd_en=0;
}

void lcd_1602_write_data(uchar date) //写数据
{
rs=1;
rw=0;
P0=date;
lcd_1602_delay(5);
lcd_en=1;
lcd_1602_delay(5);
lcd_en=0;
}

void lcd_1602_init()               //LCD1602初始化函数
{
lcd_en=0;
lcd_1602_write_com(0x38);
lcd_1602_write_com(0x0e);
lcd_1602_write_com(0x06);
lcd_1602_write_com(0x01);
}

void lcd_1602_wait_sure()         //显示'OK'&'NO'界面
{
lcd_1602_write_com(0x80+0x40);
lcd_1602_write_data('O');
lcd_1602_write_data('K');
		
lcd_1602_write_com(0x80+0x4e);
lcd_1602_write_data('N');
lcd_1602_write_data('O');
}

void lcd_1602_show_num(uint num)       // 1602显示数字的函数
{
if(num <= 9) 
 {
   lcd_1602_write_data('0'+num);
   lcd_1602_write_data(' ');
  }else if(num >= 10 ) 
 {
   lcd_1602_write_data('0'+num/10);
   lcd_1602_write_data('0'+num%10);
  }
}

void lcd1602_info_display(uint choose, uint num_type, uint num_count)  //1602显示函数
{
int num;
lcd_1602_write_com(0x01);
switch(choose) 
  {
case WAIT_SALE:
lcd_1602_write_com(0x80+0x03);       //
for(num = 0; num < 8; num++) 
{
lcd_1602_write_data(wait_sale[num]);	
}
break;
case NO_WATER:
lcd_1602_write_com(0x80+0x03);
for(num = 0; num < 9; num++)
  {
   lcd_1602_write_data(no_water[num]);	
   }
break;
case SALING:
//price
lcd_1602_write_com(0x80+0x00);
for(num = 0; num < 6; num++) 
  {
    lcd_1602_write_data(price[num]);
   }
lcd_1602_write_data('0'+(num_type+1));
lcd_1602_write_data(' ');
//number
lcd_1602_write_data(' ');
for(num = 0; num < 4; num++) 
          {
           lcd_1602_write_data(number[num]);
           }
lcd_1602_show_num(num_count);
		
		
break;
case INPUT_MONEY:
//need money	
lcd_1602_write_com(0x80+0x00);
for(num = 0; num < 4; num++) 
    {
      lcd_1602_write_data(sum[num]);
    }
lcd_1602_show_num(num_type);
//input
lcd_1602_write_data(' ');
lcd_1602_write_data(' ');
for(num = 0; num < 6; num++) 
	{
       lcd_1602_write_data(input[num]);
	}
lcd_1602_show_num(num_count);
break;
case CHANGE:
if(!num_count) 
		{
		//put water
       lcd_1602_write_com(0x80+0x02);
		for(num = 0; num < 13; num++)
            {
lcd_1602_write_data(put_water[num]);
            }
       }
else 
{
//cancle water
lcd_1602_write_com(0x80+0x02);              //取消选择时写命令退币
for(num = 0; num < 9; num++) 
	{
     	lcd_1602_write_data(cancle[num]);
	}
}
//change
lcd_1602_write_com(0x80+0x43);
for(num = 0; num < 7; num++)
	{
    	lcd_1602_write_data(change[num]);
	}
lcd_1602_show_num(num_type);
break;
default:
break;
}
lcd_1602_write_com(0x0c);
}
void lcd_1602_saling(uint type, uint num)
{
	lcd1602_info_display(SALING, type, num);
	lcd_1602_wait_sure();
}

void lcd_1602_inputing(uint need, uint input)
{
  lcd1602_info_display(INPUT_MONEY, need, input);
  lcd_1602_wait_sure();
}
/*************************************************/
void delay1(unsigned int t)           // 延时函数
{
unsigned int i,j;
for(i=0;i<t;i++)
for(j=0;j<10;j++);
}

/*
 void out_water()             //推出水函数
{
int i, j = 100; 
while(j--) 
  {
	for(i = 0; i < 8; i++) 
	{
	  P0 = water_change(0xff, i);
     delay1(50);
    }
  }
P0 = 0xff;
}
*/

unsigned char key(void)       //按键响应函数
{
 unsigned char key,key1,key2;
 P1=0XF0;
if(((P1&0XF0)!=0XF0)||(P20==0 ))  //ked down
    {
                   if(P20==0)
								{
									e=0x04;
									while(P20==0);
									return 1;
								}
       delay1(10);			 //延时去抖动
       if((P1&0XF0)!=0XF0)		  //仍然有键按下
      {
        key1=P1&0XF0;			  //读入列线值
        P1=0X0F;
        key2=P1&0X0F;
        key=key1|key2;
 switch(key)                  //按键映射
 {
 case 0xee: e=0x00;break;
 case 0xde: e=0x01;break;
 case 0xbe: e=0x02;break;
 case 0x7e: e=0x03; break;
 case 0xed: e=0x04;break;
 case 0xdd: e=0x05;break;
 case 0xbd: e=0x06;break;
 case 0x7d: e=0x07; break;
 case 0xeb: e=0x08;break;
 case 0xdb: e=0x09;break;
 case 0xbb: e=0x0a;break;
 case 0x7b: e=0x0b; break;
 case 0xe7: e=0x0c;break;
 case 0xd7: e=0x0d;break;
 case 0xb7: e=0x0e;break;
 case 0x77: e=0x0f; break;
 }
 while((P1&0X0F)!=0X0F);
 delay1(10);
 return 1;
}
  }
 return 0;
}

/*********************************************************************/
/*****************main********************************/
void main()
{
 unsigned char ok = 0, water_type = -1,  water_is_choose = 0;
 char water_count = 0;
unsigned int flag = 0, money = 0, need_money;

lcd_1602_init();
again:
ok = 0; water_type = -1; water_count = 0; water_is_choose = 0;
flag = 0; money = 0; need_money = 0;	
lcd1602_info_display(WAIT_SALE, 0, 0);
while(!ok)	//choose the water and count
{
  flag=key();
  if(flag)
  {
    switch(e)         //不同的按键进行不同的操作
     {
        case(0x00):
        water_type++;
        water_is_choose = 1;
        if(water_type == 8) 
       	{
             water_type = 0;
	        } 
P3 = water_change(0xff, water_type); 
break;		
case(0x01):
water_type--;
water_is_choose = 1;
if(water_type == -1 || water_type == -2)
{
  water_type = 7;
}
P3 = water_change(0xff, water_type);
break;
case(0x02):
if(water_is_choose == 1) 
  {
     water_count++;
     if(water_count > water_total[water_type]) 
     {
       water_count = water_total[water_type];
    	}
   }
break;
case(0x03):
if(water_is_choose == 1)
{
  water_count--;
  if(water_count < 0) 
	{
      water_count = 0;
	 }
}				
break;
case(0x0e):	//  返回
goto again;
break;    
case(0x0f): 	//   确认
if(water_is_choose == 1) 
	 {	
      ok=1;
	  }				
	break; 
   default:
   break;				
}
lcd_1602_saling(water_type, water_count);			
	}
}
	
//choose ok: calculate the money        //计算钱的总数
need_money = water_count * water_price[water_type];
lcd_1602_inputing(need_money, money);
ok = 0;	
while(!ok)		//get money
	{
		flag=key();
		if(flag)
		{
			switch(e)
			{
				case(0x04):
					money=money+1;
					break;
				case(0x05):
					money=money+5; 
					break;
				case(0x06):
					money=money+10;
					break;
				case(0x07):
					money=money+20;
					break;
				case(0x08):
					money=money+50;
					break;
				case(0x09):
					money=money+100;
					break;
				case(0x0e):	//back					
	lcd1602_info_display(CHANGE, money, CANCLE);
	P3 = 0xff;
	delay1(10000);
					
   goto again;
   break;
   default:
   break;
			}
			 		
//show money
lcd_1602_inputing(need_money, money);    //投币系统

if(money>0)

{if(money >= need_money) 
			{
				P21 =0;
				delay1(1000);
				P21 =1;
				dj=0;
				led=0;
				ok=0;			//break and show back money
				//show back money
				lcd1602_info_display(CHANGE, money-need_money, NO_DATA);
			//	out_water();
				delay1(20000);
				P3 = 0xff;
				dj=1;
				led=1;
				goto again;
		
			}
		}
	}
}
}
