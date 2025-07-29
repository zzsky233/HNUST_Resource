#include<stdio.h>
#include<stdlib.h>
 
#define False 0
#define True 1
 
/********��Ҫ���ݽṹ********/
char NAME[100]={0};//��Դ������
int Max[100][100]={0};//����������
int Allocation[100][100]={0};//ϵͳ�ѷ������
int Need[100][100]={0};//����Ҫ��Դ����
int Available[100]={0};//������Դ����
int Request[100]={0};//������Դ����
int Work[100]={0};//���ϵͳ���ṩ��Դ��
int Finish[100]={0}; //���ϵͳ�Ƿ����㹻����Դ�������������
int Security[100]={0};//��Ű�ȫ����
 
int M=100;//���̵������
int N=100;//��Դ�������
 
/********��ʼ�����ݣ����������������Դ���ࡢ������Դ������������
�����̶���Դ����������������̵���Դ�ѷ��������ȡ�********/
void init()
{
	/* mΪ���̸�����������������nΪ��Դ���࣬������������*/
    int i,j,m,n;
	int number,flag;
	char name;//������Դ����
	int temp[100]={0};//ͳ���Ѿ��������Դ
	//����ϵͳ��Դ��Ŀ������Դ���Ը���
	printf("ϵͳ������Դ����Ϊ:");
	scanf("%d",&n);
	N=n;
	for(i=0;i<n;i++)
	{
		printf("��Դ%d������:",i);
		fflush(stdin);  //������������������ַ���ע���������#include<stdlib.h>ͷ�ļ�
		scanf("%c",&name);
		NAME[i]=name;
		printf("��Դ%c�ĳ�ʼ����Ϊ:",name);
		scanf("%d",&number);
		Available[i]=number;
	}
 
	//����������������̵�����������
	printf("\n��������̵�����:");
	scanf("%d",&m);
	M=m;
	printf("����������̵������������ֵ[Max]:\n");
	do{
		flag = False;
		for(i=0;i<M;i++)
			for(j=0;j<N;j++)
			{
				scanf("%d",&Max[i][j]);
				if(Max[i][j]>Available[j])
					flag = True;
			}
		if(flag)
			printf("��Դ�������������ϵͳ����Դ�����������������!\n");
	} while(flag);
 
 
	//����������Ѿ��������Դ��������û���Ҫ����Դ��
	do{
		flag=False;
		printf("������������Ѿ��������Դ��[Allocation]:\n");
		for(i=0;i<M;i++)
		{
			for(j=0;j<N;j++)
		  	{
				scanf("%d",&Allocation[i][j]);
				if(Allocation[i][j]>Max[i][j])
					flag=True;
				Need[i][j]=Max[i][j]-Allocation[i][j];
				temp[j]+=Allocation[i][j];//ͳ���Ѿ���������̵���Դ��Ŀ
		  	}
		}
		if(flag)
			printf("�������Դ���������������������!\n");
	}while(flag);
 
	//���ϵͳ�п����õ���Դ��
	for(j=0;j<N;j++)
		Available[j]=Available[j]-temp[j];
}
 
/********��ʾ��Դ�������********/
void showdata()
{
	int i,j;
	printf("*************************************************************\n");
	printf("ϵͳĿǰ���õ���Դ[Available]:\n");
	for(i=0;i<N;i++)
        printf("%c  ",NAME[i]);
	printf("\n");
	for(j=0;j<N;j++)
        printf("%d  ",Available[j]);
	printf("\n");
	printf("ϵͳ��ǰ����Դ����������£�\n");
	printf("            Max   	 Allocation    Need\n");
	printf("������      ");
	//����������ͬ�е���Դ����Max��Allocation��Need�·ֱ��Ӧ
	for(j=0;j<3;j++){
		for(i=0;i<N;i++)
			printf("%c  ",NAME[i]);
		printf("     ");
	}
	printf("\n");
	//���ÿ�����̵�Max��Allocation��Need
	for(i=0;i<M;i++){
		printf(" P%d        ",i);
	    for(j=0;j<N;j++)
			printf("%d  ",Max[i][j]);
		printf("     ");
		for(j=0;j<N;j++)
			printf("%d  ",Allocation[i][j]);
		printf("     ");
		for(j=0;j<N;j++)
			printf("%d  ",Need[i][j]);
		printf("\n");
	}
}
 
/********���Է�����Դ********/
int test(int i) //��̽�ԵĽ���Դ�������i������
{
	for(int j=0;j<N;j++)
	{
		Available[j]=Available[j]-Request[j];
		Allocation[i][j]=Allocation[i][j]+Request[j];
		Need[i][j]=Need[i][j]-Request[j];
	}
	return True;
}
 
/********��̽�Է�����Դ����********/
int Retest(int i) //��test�����෴
{
	for(int j=0; j<N; j++)
	{
		Available[j] = Available[j] + Request[j];
		Allocation[i][j] = Allocation[i][j] - Request[j];
		Need[i][j] = Need[i][j] + Request[j];
	}
	return True;
}
 
/********��ȫ���㷨********/
int safe()
{
	int i,j,k=0,m,apply;
	//��ʼ��work
	for(j=0;j<N;j++)
        Work[j] = Available[j];
    //��ʼ��Finish
    for(i=0;i<M;i++)
    	Finish[i] = False;
	//��ȫ����
	for(i=0;i<M;i++){
		apply=0;
		for(j=0;j<N;j++){
			if(Finish[i]==False && Need[i][j]<=Work[j])
			{
				apply++;
				//ֱ��ÿ����Դ��������С��ϵͳ��������Դ���ſɷ���
				if(apply==N)
				{
					for(m=0;m<N;m++)
				        Work[m]=Work[m]+Allocation[i][m];//���ĵ�ǰ�ɷ�����Դ
					Finish[i]=True;
					Security[k++]=i;
					i=-1; //��֤ÿ�β�ѯ���ӵ�һ�����̿�ʼ
				}
			}
		}
	}
 
	for(i=0;i<M;i++){
		if(Finish[i]==False){
			printf("ϵͳ����ȫ\n");//���ɹ�ϵͳ����ȫ
			return False;
		}
	}
    printf("ϵͳ�ǰ�ȫ��!\n");//�����ȫ������ɹ�
    printf("����һ����ȫ����:");
	for(i=0;i<M;i++){//������н�������
		printf("P%d",Security[i]);
		if(i<M-1)
			printf("->");
	}
	printf("\n");
	return True;
}
 
/********�������м��㷨��������Դ�����Է�********/
void bank()
{
	int flag = True;//��־�������ж��ܷ�������м��㷨����һ��
	int i,j;
 
	printf("���������������Դ�Ľ��̺�(0-%d):",M-1);
    scanf("%d",&i);//������������Դ�Ľ��̺�
 
	printf("���������P%dҪ�������Դ����:\n",i);
	for(j=0;j<N;j++)
	{
		printf("%c:",NAME[j]);
		scanf("%d",&Request[j]);//������Ҫ�������Դ
	}
 
	//�ж����м��㷨��ǰ�������Ƿ����
    for (j=0;j<N;j++)
	{
		if(Request[j]>Need[i][j])//�ж������Ƿ�������������������
		{
			printf("����P%d�������Դ��������Ҫ����Դ",i);
			printf("���䲻����������䣡\n");
			flag = False;
			break;
		}
		else
		{
            if(Request[j]>Available[j])//�ж������Ƿ���ڵ�ǰ�ɷ�����Դ�������������
			{
				printf("����%d�������Դ����ϵͳ���ڿ����õ���Դ",i);
				printf("\n");
				printf("ϵͳ�����㹻��Դ���������!\n");
				flag = False;
				break;
			}
		}
    }
    //ǰ���������������Է�����Դ��Ѱ�Ұ�ȫ����
    if(flag) {
		test(i); //���ݽ���������,�Է�����Դ
		showdata(); //���ݽ�������������ʾ�Է�������Դ��
		if(!safe()) //Ѱ�Ұ�ȫ����
		{
			Retest(i);
			showdata();
		}
    }
}
 
 
int main()//������
{
	char choice;
	init();//��ʼ������
    showdata();//��ʾ������Դ
    //�����м��㷨�ж�ϵͳ��ǰʱ���Ƿ�ȫ������ȫ�Ͳ��ټ���������Դ
	if(!safe()) exit(0);
 
	do{
		printf("*************************************************************\n");
		printf("\n");
		printf("\n");
		printf("\t-------------------���м��㷨��ʾ------------------\n");
		printf("                     R(r):�������   \n");
		printf("                     E(e):�˳�       \n");
		printf("\t---------------------------------------------------\n");
		printf("��ѡ��");
		fflush(stdin);  //������������������ַ���ע���������#include<stdlib.h>ͷ�ļ�
		scanf("%c",&choice);
		switch(choice)
		{
			case 'r':
			case 'R':
				bank();break;
			case 'e':
			case 'E':
				exit(0);
			default: printf("����ȷѡ��!\n");break;
		}
	} while(choice);
}
 
