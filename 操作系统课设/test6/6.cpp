#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

int n,m;


int Resource[101];
int Available[101];
int Claim[101][101];
int Allocation[101][101];
int Need[101][101];
int Request[101];
int Security[101];
int processnum;

void my_init()
{
	//�������Allocation�����Available���� 
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			Allocation[i][j]=rand()%10;
	for(int j=1;j<=m;j++)
		Available[j]=rand()%10;
	for(int j=1;j<=m;j++)
	{
		int total=0;
		for(int i=1;i<=n;i++)
		{
			total+=Allocation[i][j];
		}
		Resource[j]=Available[j]+total;
	}
	//�������������Claim����Allocation[i][j]��Resource[j]֮������� 
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			Claim[i][j]=rand()%(Resource[j]-Allocation[i][j]+1)+Allocation[i][j];
	//�����������Need
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			Need[i][j]=Claim[i][j]-Allocation[i][j];
}

//������� 
void test()
{
	cout<<"Claim:"<<endl;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			cout<<Claim[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"Allocation:"<<endl;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			cout<<Allocation[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"Need:"<<endl;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			cout<<Need[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"Resource"<<endl;
	for(int j=1;j<=m;j++)
	{
		cout<<Resource[j]<<" ";
	}
	cout<<endl;
	cout<<"Available"<<endl;
	for(int j=1;j<=m;j++)
	{
		cout<<Available[j]<<" ";
	}
	cout<<endl<<endl;
}

//�ж����Ƿ�ȫ 
int is_safe(int mode)
{
	int counting=0;
	int ans=0;
	int work[101];//mά;
	int finish[101];//nά;
	//��ʼ��Finnish��work 
	for(int i=1;i<=n;i++)
		finish[i]=0;
	for(int j=1;j<=m;j++)
		work[j]=Available[j];
	for(int i=1;i<=n;i++)
	{
		counting=0;
		for(int j=1;j<=m;j++)
		{
			if(finish[i]==0&&Need[i][j]<=work[j])
			{
				counting++;
				if(counting==m)
				{
					for(int k=1;k<=m;k++)
						work[k]+=Allocation[i][k];
					finish[i]=1;
					Security[ans++]=i;
					i=0;
				}
			}
		}
	}
	
	for(int i=1;i<=n;i++)
	{
		if(finish[i]==0)
		{
			if(mode!=1)
			cout<<"ϵͳ����ȫ"<<endl;
			return 0;			
		}
	}
	cout<<"ϵͳ��ȫ������������һ����ȫ���У�"<<endl;
	for(int k=0;k<n;k++)
	{
		cout<<Security[k]<<" ";
		if(k!=n-1)cout<<"->";
	}
	cout<<endl;
	return 1;
}

int entering(int num)
{
	for(int j=1;j<=m;j++)
	{
		Available[j]-=Request[j];
		Allocation[num][j]+=Request[j];
		Need[num][j]-=Request[j];
	}
	return 1;
}

int reseting(int num)
{
	for(int j=1;j<=m;j++)
	{
		Available[j]+=Request[j];
		Allocation[num][j]-=Request[j];
		Need[num][j]+=Request[j];		
	}
	return 1;
}

int banker()
{
	int flag=1;
	int num;
	cout<<"���������������Դ�Ľ��̺ţ�1-"<<n<<"��"<<endl;
	cin>>num;
	
	cout<<"������"<<num<<"�Ž��̵���Դ����"<<endl;
	for(int j=1;j<=m;j++)
	{
		cout<<"��"<<j<<"����Դ������"<<endl;
		cin>>Request[j];
		if(Request[j]>Need[num][j])
		{
			cout<<"��Դ���䲻����"<<endl;
			flag=0;
			break;
		}
		else
		{
			if(Request[j]>Available[j])
			{
				cout<<"��Դ���������ɷ���"<<endl;
				flag=0;
				break;
			}
		}
	}
	
	if(flag==1)
	{
		cout<<"��Դ�������"<<endl;
		entering(num);
		cout<<"��Դ��������壺"<<endl; 
		test();
		Sleep(1000);
		if(is_safe(0)==0)
		{
			reseting(num);
			Sleep(1000);
			test();
		}
	}
}

signed main()
{
	srand((unsigned)time(NULL));
	cout<<"���������������Դ������"<<endl;
	cin>>n>>m;
//	n=4,m=3;
	int not_safe=1;
	while(not_safe)
	{
		//��ʼ�����������Դ 
		my_init();
		not_safe=!is_safe(1);
	}
	test();
	banker();
}
