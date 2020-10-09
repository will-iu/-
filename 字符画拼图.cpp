#include <stdio.h>
#include <conio.h>//读取方向键 
#include <windows.h>
#include <time.h>  
#include <pthread.h>  

int n,p=3,q=3; 
float Time,fastTime=0; 
int Box[4][4]={0};
int b[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
clock_t start,end;  
HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//创建事件，控制子进程 

void gotoxy();
int color();
void drawTheme();
void drawTheGameBox();
void Gameplay();
void Gamewin();
int ifwin();
void random();
int keyboardControl(int key);
void regulation();
void preview();

void gotoxy(int x, int y) 
{
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

int color(int c) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
	return 0;
}

DWORD WINAPI ThreadProc1(LPVOID lpParam)  //子线程
{
	while(TRUE) 
	{
		WaitForSingleObject(hEvent , INFINITE );
		end = clock();
		Time= (double)(end-start)/CLK_TCK;
		gotoxy(60,5);
		printf("已用时：%.2f s",Time);
		Sleep(10);
	} 
    return 0;
}

void drawTheme()
{
	int n;
	p=3,q=3;//重新开始时给白块归位 
	color(7);
	gotoxy(18,5);
	printf("▋╭⌒╮");
	gotoxy(16,6);
	printf("╱◥██◣");
	gotoxy(15,7);
	printf("｜田｜田田│");
	color(6);
	gotoxy(15,8);
	printf("╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬");
	color(3);
	gotoxy(38,5);
	printf("数字拼图      …8＜……");
	color(14);
	for(int i=9;i<=20;i++)
	{
		for(int j=15;j<=59;j+=2)
		{
			if(i==9||i==20)
			{
				gotoxy(j,i);
				printf("■"); 
			}
			else if(j==15||j==59)
			{
				gotoxy(j,i);
				printf("■");
			}
		}
	}
	
	color(12);
	gotoxy(25,12);
	printf("1.开始游戏");
	gotoxy(40,12);
	printf("2.游戏规则");
	gotoxy(25,16);
	printf("3.字符画预览");
	gotoxy(40,16);
	printf("4.退出游戏");
	color(5);
	gotoxy(25,19);
	printf("请选择[1 2 3 4]:[ ]\b\b");
	scanf("%d",&n);
	switch(n)
	{
		case 1:
			Gameplay();
			break;
		case 2:
			regulation(); 
			break;
		case 3:
			preview();
			break;
		case 4:
			exit(0);
			break;
	}
}

void drawTheGameBox()
{
	start = clock();
	color(14);
	for(int j=2;j<=22;j+=5)
	{
		gotoxy(15,j);
		printf("-----------------------------------------");
	}
	for(int j=3;j<22;j++)
	{
		if(j!=7&&j!=12&&j!=17)
		{
			gotoxy(15,j);
			printf("▏        |         |         |        ▕");	
		}
	}
	
	color(5);
	gotoxy(60,3);
	FILE *fp;
	fp=fopen("save.txt","r"); 
	fscanf(fp,"%f",&fastTime);
	fclose(fp);
	printf("最短用时：%.2f s",fastTime);
}

void Gameplay()
{
	system("cls");
	drawTheGameBox();
	random();
	SetEvent(hEvent);
	while(1)
	{
		while(kbhit())
		{
			if(!keyboardControl(getch())) 
			{
				SetEvent(hEvent);
				continue;
			}
			SetEvent(hEvent);
			if(ifwin()!=0)
			{
				ResetEvent(hEvent);
				Gamewin();
	 			gotoxy(15,20);
				color(15);
				printf("重新开始拼图之旅-----1");
				gotoxy(40,20);
				printf("不玩了，退出游戏-----2\n");
				gotoxy(15,21);
				printf("请选择[1 2]:[ ]\b\b");
				scanf("%d",&n);
				switch(n)
				{
					case 1:
						system("cls");
						drawTheme();
					case 2:
						exit(0);
						break;
				}
			}
		}
	}
}

void Gamewin()
{
	int n;
	system("cls");
	color(14);
	gotoxy(16,3);
	printf("."".      ."",");
	gotoxy(15,4);
	printf("|  |   /  /");
	gotoxy(15,5);
	printf("|  |  /  /");
	gotoxy(15,6);
	printf("|  | /  /");
	gotoxy(15,7);
	printf("|  |/  ;-._");
	gotoxy(15,8);
	printf("}  ` _/  / ;");
	gotoxy(15,9);
	printf("|  /` ) /  /");
	gotoxy(15,10);
	printf("| /  /_/\\_/\\");
	gotoxy(15,11);
	printf("|/  /      |");
	gotoxy(15,12);
	printf("(  ' \\ '-  |");
	gotoxy(15,13);
	printf("\\    `.  /");
	gotoxy(15,14);
	printf(" |      |");
	gotoxy(15,15);
	printf(" |      |");
	color(10);
	gotoxy(42,8);
	printf("Congratulations!");
	gotoxy(42,10);
	printf("拼 图 成 功！");

	
	FILE *fp;
	if(Time<fastTime)
	{
		fastTime=Time; 
		fp=fopen("save.txt","w");//w+是读写，w是写 
		fprintf(fp,"%.2f",Time);//注意数据类型 
		fclose(fp);
		color(10);
		gotoxy(45,16);
		printf("打破记录☆"); 
	}
	else
	{
		color(12);
		gotoxy(45,16);
		printf("继续加油哦!");
	}	
	color(14);
	gotoxy(42,12);
	printf("您的用时：%.2f s",Time);
	color(12);
	gotoxy(42,14);
	printf("最佳用时：%.2f s",fastTime);
} 

int ifwin()
{
	int i,j,t=0;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(Box[i][j]==i*4+j+1)
				t++;
		}
	if(t==15)
		return 1;
	return 0;
}

void random()
{
	int i,j,t;
	srand(time(NULL));
	for(i=14; i>0; i--) 
	{
        j=rand()%i;
        t=b[i];
        b[i]=b[j];
        b[j]=t;
    }    
	t=0;
    for(i=0;i<14;i++)
    {
    	for(j=i+1;j<15;j++)
    	{
    		if(b[i]>b[j])
    			t++;
		}
	}
	if(t%2!=0)
	{
		t=b[13];
        b[13]=b[14];
        b[14]=t;
	}
	t=0;
    color(10);
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Box[i][j]=b[t++];
			gotoxy(15+j*10+5,2+i*5+3);
			printf("%d",Box[i][j]);
		}
	}
	gotoxy(50,20);
	printf(" ");	
}

int keyboardControl(int key)
{
	int i,j;
	ResetEvent(hEvent);
	Sleep(1);
	switch(key)
	{
		case 77://向上 
		{
			if(q-1<0)
				return 0;
			else
			{
				gotoxy(15+q*10+5,2+p*5+3);
				Box[p][q]=Box[p][--q];
				printf("%d",Box[p][q]);
				gotoxy(15+q*10+5,2+p*5+3);
				printf("  ");
				return 1;
			}
		}
			
		case 75://向下 
		{
			if(q+1>3)
				return 0;
			else
			{
				gotoxy(15+q*10+5,2+p*5+3);
				Box[p][q]=Box[p][++q];
				printf("%d",Box[p][q]);
				gotoxy(15+q*10+5,2+p*5+3);
				printf("  ");
				return 1;
			}
		}

		case 80://向左 
		{
			if(p-1<0)
				return 0;
			else
			{
				gotoxy(15+q*10+5,2+p*5+3);
				Box[p][q]=Box[--p][q];
				printf("%d",Box[p][q]);
				gotoxy(15+q*10+5,2+p*5+3);
				printf("  ");
				return 1;
			}	
		}

		case 72://向右
		{
			if(p+1>3)
				return 0;
			else
			{
				gotoxy(15+q*10+5,2+p*5+3);
				Box[p][q]=Box[++p][q];
				printf("%d",Box[p][q]);
				gotoxy(15+q*10+5,2+p*5+3);
				printf("  ");
				return 1;
			}
		} 

		case 27:
			{
				gotoxy(20,30);
				color(12);
				printf("是否返回主界面？(y/n)");
				char c=getch();
				if(c=='y'||c=='Y')
				{
					ResetEvent(hEvent);
					system("cls");
					drawTheme();
				}
				if(c=='n'||c=='N')
				{
					color(10);
					gotoxy(20,30);
					printf("                     ");
				}
				color(10);
				break;
			}
		default:
			return 0;
	}
}

void regulation()
{
	int i,j = 1;
    system("cls");
    color(13);
    gotoxy(34,3);
    printf("游戏规则");
    color(2);
    for (i = 6; i <= 18; i++)  
	{
		for (j = 12; j <= 68; j++)  
		{
			gotoxy(j, i);
			if (i == 6 || i == 18) printf("=");
			else if (j == 12 || j == 67) printf("||");
		}
	}
    color(3);
    gotoxy(18,7);
    printf("tip1: 玩家通过↑ ↓ ← →方向键来移动空白方块");
    color(10);
    gotoxy(18,9);
    printf("tip2: 例如↓方向键是使上面的方块下来");
    color(14);
    gotoxy(18,11);
    printf("tip3: 按ESC退出游戏");
    color(11);
    gotoxy(18,13);
    printf("tip4: 当1~9按顺序排列在矩阵中，拼图成功");
    color(4);
    gotoxy(18,15);
    printf("tip5: 玩家的最佳纪录最终会被记录在文件中");
    color(5);
    gotoxy(18,17);
    printf("tip6: 难度模块正在开发中……");
    getch();                //按任意键返回主界面
    system("cls");
    drawTheme();
}

void preview()
{
	system("cls");
	color(5);
    gotoxy(35,10);
    printf("敬请期待"); 
    getch();          
    system("cls");
    drawTheme();
}


int main()
{
	CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);//创建进程 
	drawTheme();
	return 0;
}
