#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
#define NULL 0
#define TASKNUM 6
#define PROSTATUS 3
#define LEDs P0
sbit left=P1^0;
sbit right=P1^1;
sbit down=P1^2;
sbit up=P1^3;
sbit SEG=P1^4;
sbit Bit=P1^5;//数码显示段、位锁存使能（低电位有效）
sbit Beep=P1^6;
uchar (*runList[TASKNUM])(uchar c);	//以长度为TASKNUM的指针数组作为程序队列
uchar proStat[TASKNUM][PROSTATUS];		//程序状态表（程序状态有：运行->1、结束->0、挂起->step）
/****Start数据交换区*****/
uchar time=0,hour=0,minute=0,second=0;//时间容器
uchar mark=0,MODE=0;//设置位标志及模式标志位
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//定义数码管显示表
uchar ALERT[]={24,0,0};//闹钟时间
uchar adress=0;//歌曲播放初始地址
//闹钟及提醒歌曲码表
uint code songlevel[]={0x8E1,0x7E9,0x70C,0x6A6,0x5EC,0x548,0x4B4};
uchar song[]={1,2,3,1,1,2,3,1,3,4,5,3,4,5};
/****End数据交换区*****/
/***************************
*函数：数码管显示程序*******
*功能：数码管显示***********
****************************/
uchar display(uchar status){
	uchar hh,hl,mh,ml,sh,sl;
	proStat[0][1]=1;
	switch(MODE){
		case 0: ;
		case 1: hh=hour/10;
				hl=hour%10;
				mh=minute/10;
				ml=minute%10;
				sh=second/10;
				sl=second%10;
				break;
		case 2: hh=ALERT[0]/10;
				hl=ALERT[0]%10;
				mh=ALERT[1]/10;
				ml=ALERT[1]%10;
				sh=ALERT[2]/10;
				sl=ALERT[2]%10;
	}//确定显示内容
	switch(status){
		case 0 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xFE;
				Bit=1;Bit=0;
				LEDs=table[hh];
				SEG=1;SEG=0;
			return ++status;
		case 1 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xFD;
				Bit=1;Bit=0;
				LEDs=table[hl];
				SEG=1;SEG=0;
			return ++status;
		case 2 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xFB;
				Bit=1;Bit=0;
				LEDs=0x40;
				SEG=1;SEG=0;
			return ++status;
		case 3 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xF7;
				Bit=1;Bit=0;
				LEDs=table[mh];
				SEG=1;SEG=0;
			return ++status;
		case 4 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xEF;
				Bit=1;Bit=0;
				LEDs=table[ml];
				SEG=1;SEG=0;
			return ++status;
		case 5 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xDF;
				Bit=1;Bit=0;
				LEDs=0x40;
				SEG=1;SEG=0;
			return ++status;
		case 6 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0xBF;
				Bit=1;Bit=0;
				LEDs=table[sh];
				SEG=1;SEG=0;
			return ++status;
		case 7 :SEG=0;Bit=0;
				LEDs=0x0;
				SEG=1;SEG=0;
				LEDs=0x7F;
				Bit=1;Bit=0;
				LEDs=table[sl];
				SEG=1;SEG=0;
			return 0;
	}
}
/***************************
*函数：键盘扫描程序*********
*功能：扫描键盘*************
****************************/
uchar scanKey(uchar status){
	switch(status){
		case 0: if(up==0||down==0||left==0||right==0){
					if((MODE==1)||(MODE==2))return ++status;
				}
				return 0;
		case 1: if(up==0){
					while(!up);
					switch(MODE){
						case 1: if(mark==0)second++;
								if(mark==1)second+=10;
								if(mark==3)minute++;
								if(mark==4)minute+=10;
								if(mark==6)hour++;
								if(mark==7)hour+=10;
								break;
						case 2: if(mark==0)ALERT[2]++;
								if(mark==1)ALERT[2]+=10;
							    if(mark==3)ALERT[1]++;
								if(mark==4)ALERT[1]+=10;
								if(mark==6)ALERT[0]++;
								if(mark==7)ALERT[0]+=10; 
					}					
					return 0;
				}else{
					return ++status;
				}
		case 2: if(down==0){
					while(!down);
					switch(MODE){
						case 1: if(mark==0)second--;
								if(mark==1)second-=10;
								if(mark==3)minute--;
								if(mark==4)minute-=10;
								if(mark==6)hour--;
								if(mark==7)hour-=10;
								break;
						case 2: if(mark==0)ALERT[2]--;
								if(mark==1)ALERT[2]-=10;
							    if(mark==3)ALERT[1]--;
								if(mark==4)ALERT[1]-=10;
								if(mark==6)ALERT[0]--;
								if(mark==7)ALERT[0]-=10; 
					}					
					return 0;
				}else{
					return ++status;
				}
		case 3: if(left==0){
					while(!left);
					if(mark<7){
						mark++;
					}else{
						mark=0;
					}
					return 0;	
				}else{
					return ++status;
				}
		case 4: if(right==0){
					while(!right);
					if(mark>0){
						mark--;
					}else{
						mark=7;
					}
					return 0;	
				}else{
					return 0;
				}
	}
}
/***************************
*函数：闹钟程序*************
*功能：闹钟*****************
****************************/
uchar alert(uchar status){
	proStat[0][1]=10;
	switch(status){
		case 0: if((ALERT[0]==hour)&&(ALERT[1]==minute))
					return 1;
				return 0;
		case 1: if((second%2)==1){
					if(second<30)TR1=1;//设置闹铃时间
					adress++;
					if(adress==13)adress=0;
				}else{
					TR1=0;
				}
				if(ALERT[1]!=minute)
					return 2;
				break;		
		case 2: TR1=0;
				return 0;
	}
}
uchar halfHour(uchar status){
	proStat[0][1]=2;
	switch(status){
		case 0: if((minute==0)||(minute==30))//半点提醒
					return 1;
				return 0;
		case 1: if(second<10)TR1=1;
				if(second%2==1){
					adress++;
					if(adress==13)adress=0;
				}
				if(second>10)//设置提醒闹铃时间
					return 2;
				break;		
		case 2: TR1=0;
				return 0;
	}
}
/***************************
*函数：refreshTime()********
*功能：刷新系统时间*********
****************************/
uchar refreshTime(uchar status){
	proStat[0][1]=2;
	switch(status){
		case 0:	if(second>59){
			   		second=0;
					minute++;
				}
				return ++status;
		case 1: if(minute>59){
					minute=0;
					hour++;
				}
				return ++status;
		case 2:	if(hour>23){
					hour=0;
				}
				return 0;
	}
}				
/***************************
*函数：freshRunList()*******
*功能：更新运行队列*********
****************************/
void refreshRunList(void){
	uchar (*p)();
	uchar j=0,i=0;
	uchar a=proStat[0][0],b=proStat[0][1],c=proStat[0][2];
	p=runList[0];
	//以环形链表方式，切换任务	
	for(i=0;i<=TASKNUM-2;i++){
		runList[i]=runList[i+1];
		proStat[i][0]=proStat[i+1][0];
		proStat[i][1]=proStat[i+1][1];
		proStat[i][2]=proStat[i+1][2];
	}
	runList[TASKNUM-1]=p;
	proStat[TASKNUM-1][0]=a;
	proStat[TASKNUM-1][1]=b;
	proStat[TASKNUM-1][2]=c;
}
/***************************
*函数：taskArragement()*****
*功能：任务调度*************
****************************/
void taskArragement(void){
	if(runList[0]!=0){	//判断运行队列队首是否为空
		if(proStat[0][0]==1){//判断程序是否开启
			if(proStat[0][1]<=0){//如果计数值到0
				proStat[0][2]=(*runList[0])(proStat[0][2]);
			}else{
			--proStat[0][1];
			}
		}
	}
}
/***************************
*函数：初始化函数*********
*功能：初始化*************
****************************/
void init(void){
	uchar i;
	for(i=0;i<TASKNUM;i++)//初始化运行队列;
		runList[i]=NULL;
	runList[0]=display;
	proStat[0][0]=1;//程序是否开启
	proStat[0][1]=0;//程序计数值
	proStat[0][2]=0;//程序运行状态
	runList[1]=refreshTime;
	proStat[1][0]=1;//程序是否开启
	proStat[1][1]=0;//程序计数值
	proStat[1][2]=0;//程序运行状态
	runList[2]=scanKey;
	proStat[2][0]=1;//程序是否开启
	proStat[2][1]=0;//程序计数值
	proStat[2][2]=0;//程序运行状态
	runList[3]=alert;
	proStat[3][0]=1;//程序是否开启
	proStat[3][1]=0;//程序计数值
	proStat[3][2]=0;//程序运行状态
	runList[4]=halfHour;
	proStat[4][0]=1;//程序是否开启
	proStat[4][1]=0;//程序计数值
	proStat[4][2]=0;//程序运行状态
/*	//初始化CPU中断允许*/
//	IE=0x8F;//中断控制（EA=1开启CPU总中断允许，EX0=1开启外部中断0，ET0=1开启定时器/计时器中断0,ET1=1,EX1=1）
	EA=1;EX0=1;ET0=1;ET1=1;
	//初始化定时器0
	TMOD=0x11;//初始化定时器0工作方式
	TH1=0xF0;
	TL1=0xF0;//装入初值
//	PT0=1;//将定时器0作为高优先级
	IT0=1;//外部中断0的触发方式为跳变沿触发
	TR0=1;//启动定时器0
}
/***************************
*函数：主函数***************
*功能：主循环***************
****************************/
void main(void){
	init();						//初始化函数
	while(1){
		taskArragement();		//任务调度函数，将运行队列队首做为参数
		refreshRunList();
	}
}
/***************************
*函数：interrupt************
*功能：中断服务程序*********
****************************/
void ex0() interrupt 0
{
	MODE++;
	if(MODE==1){
		TR0=0;
	}
	if(MODE==3){
		TR0=1;
		MODE=0;
	}
}
void time0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	time++;
	if(time==20){
		time=0;
		second++;
	}
}
void time1() interrupt 3
{
	TH1=(65535-songlevel[song[adress]])/256;
	TL1=(65535-songlevel[song[adress]])%256;
	Beep=~Beep;
}
