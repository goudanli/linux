/*************************************************************************
	> File Name: environ.c
	> Author: xiaowei
	> Mail: 871343234@qq.com 
	> Created Time: 2015年04月11日 星期六 21时46分02秒
 ************************************************************************/

#include<stdio.h>
extern char **environ;
int main()
{
	char **env = environ;
	while(*env)
	{
		printf("%s\n",*env++);
	}

}
