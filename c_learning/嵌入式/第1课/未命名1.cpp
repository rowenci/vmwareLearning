#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
int menu(){
	printf("*************************************\n");
	printf("*               ������              *\n");
	printf("*              1.�ӷ�               *\n");
	printf("*              2.����               *\n");
	printf("*              3.�˷�               *\n");
	printf("*              4.����               *\n");
	printf("*************************************\n");
	printf("���������ѡ��\n");
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
				printf("�ӷ�\n");
				printf("�����������������ö��ŷָ�\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 + num2;
				printf("%f + %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 2:{
				float num1,num2;
				printf("����\n");
				printf("�����������������ö��ŷָ�\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 - num2;
				printf("%f - %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 3:{
				float num1,num2;
				printf("�˷�\n");
				printf("�����������������ö��ŷָ�\n");
				scanf("%f,%f",&num1,&num2);
				float res = num1 * num2;
				printf("%f * %f = %f\n",num1,num2,res);
				pause();
				n = menu();
				break;
			}
			case 4:{
				float num1,num2;
				printf("����\n");
				printf("�����������������ö��ŷָ�\n");
				scanf("%f,%f",&num1,&num2);
				while(num2 == 0){
					printf("��������Ϊ0\n����������\n");
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
