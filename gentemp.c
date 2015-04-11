#include <strings.h>
#include <unistd.h>

static int num=0;
static char namebuf[20];
static char prefix[]="tmp";

void itoa(int i,char *string)
{
	 int power,j;
	 j=i;
	 for(power = 1;j>=10;j/=10)
		 power*=10;
	 for(;power>0;power/=10)
	 {
		 *string++ = '0'+i/power;
		 i%=power;
	 }
	 *string='\0';
}

//更加进程标识符产生文件名字符串
char* gentemp()
{
	int length,pid;
/* 获得进程标识符 */
	pid=getpid();
	strcpy(namebuf,prefix);
	length=strlen(namebuf);
/* 在文件名中增加 pid 部分 */

	itoa(pid,&namebuf[length]);
	strcat(namebuf,".");
	length=strlen(namebuf);
	do{
/* 增加后缀 number */
		itoa(num++,&namebuf[length]);
	} while (access(namebuf,0)!=-1);
	return namebuf;
}


int main()
{
	int i=0;
	for(i=0; i<5; i++)
	  printf("%s\n",gentemp());
	return 0;
}
