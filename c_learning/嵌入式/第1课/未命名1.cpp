#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
int menu(){
	printf("*************************************\n");
	printf("*               计算器              *\n");
	printf("*              1.加法               *\n");
	printf("*              2.减法               *\n");
	printf("*              3.乘法               *\n");
	printf("*              4.除法               *\n");
	printf("*************************************\n");
	printf("请输入你的选项\n");
	int choice;
	scanf("%d",&choice);
	return choice;
}
void pause(){
	system("pause");
	system("cls");
}
int main(){
	int n = 0;
	n = menu();
	while(n!=5){
		switch(n){
			case 1:{
				float num1,num2;
				printf("加法\n");
				printf("请输入两个数，并用逗号分隔\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 + num2;
				printf("%f + %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 2:{
				float num1,num2;
				printf("减法\n");
				printf("请输入两个数，并用逗号分隔\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 - num2;
				printf("%f - %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 3:{
				float num1,num2;
				printf("乘法\n");
				printf("请输入两个数，并用逗号分隔\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 * num2;
				printf("%f * %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 4:{
				float num1,num2;
				printf("除法\n");
				printf("请输入两个数，并用逗号分隔\n");
				scanf("%f,%f",&num1,&num2);
				while(num2 == 0){
					printf("除数不能为0\n请重新输入\n");
					scanf("%f,%f",&num1,&num2);
				}
				float res = num1 / num2;
				printf("%f / %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 5:{
				break;
			}
		}
	}
	
	return 0;
}
