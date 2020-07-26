/*
Name:Sachin Iyer
OS Name:Phase 1+2
GR Number:1710575
*/

#include<stdlib.h>
#include<iostream>
#include<fstream>
#include <time.h>

using namespace std;
ifstream input_file("P1");
ofstream output_file("OUTPUT");
int PTBR=0;
int si=0;
int pi=0;
int ti=0;
int ttl;
int ttc=0;
int available[30]={0};



//class for cpu
class cpu
{
	public:
	  char ir[4];
	  char r[4];
	  int ic;
	  int t;

	  void display()
	  {
	     cout<<"Contents of ir:";
		   cout<<ir<<endl;

	     cout<<"Contents of R:";
		   cout<<r<<endl;


	     cout<<"IC="<<ic<<endl;
	     cout<<"t="<<t<<endl;
	  }

};

//class for memory(ram)
class mem
{
public:
  char A[300][4];
void init()
  {
     for(int i=0;i<300;i++)
     {
       for(int j=0;j<4;j++)
	 {
	  A[i][j]=0;
	 }
     }
   }
  void display()
  {
     cout<<"Contents of m:"<<endl;

     for(int i=0;i<300;i++)
     {
       cout<<"\t"<<i<<" ";
       for(int j=0;j<4;j++)
	 {
	    cout<<A[i][j];
	 }

     }
  }

};

mem m;
cpu c;
char buffer[41];

int getrandom()
    {
    int i;
    do
    {
    i=rand()%30;
    }
    while(available[i]!=0);
    available[i]=1;
    return i;
    }

void makepageentry(int P,int F)
{
m.A[(PTBR*10)+P][1]=' ';
m.A[(PTBR*10)+P][2]=(F/10)+48;
m.A[(PTBR*10)+P][3]=(F%10)+48;
m.A[(PTBR*10)+P][0]='$';
}

void loadprogram()
{
char *ptr=m.A[0];
input_file.getline(buffer,41);
if(buffer[0]=='$' && buffer[1]=='A')
    {
    ttl=(buffer[10]-48)*10+(buffer[11]-48);
    }
if(buffer[0]!='$')
    return;
input_file.getline(buffer,41);
int PN=0,FN;
while(buffer[0]!='$')
    {
    FN = getrandom();
    makepageentry(PN,FN);
    ptr = &m.A[FN*10][0];
    for(int i=0;i<40 & buffer[i]!=0;i++)
        *ptr++=buffer[i];
    input_file.getline(buffer,41);
    PN++;
    }
}

//loading instruction registter
void loadir()
{
for(int i=0;i<4;i++)
	{
	c.ir[i]=m.A[c.ic][i];
	}
c.ic++;
}

//functions for instructions using string matching

void executePD()
{
int i[2],a,j;
if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
{
i[0]=c.ir[2]-48;
i[1]=c.ir[3]-48;
a=i[0]*10+i[1];

char *ptr=m.A[a];
for(int j=0;j<40;j++)
buffer[j]=0;
j=0;
while(j<41 && *ptr!=0)
{
buffer[j]=*ptr++;
j++;
}
output_file.write(buffer,40);
}
else
pi=2;
}

//Get Data

void executeGD()
{
  int i[2],j;
  int PN;
  input_file.getline(buffer,41);
  if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
  {
  i[0]=c.ir[2]-48;
  i[1]=c.ir[3]-48;
  PN=i[0];
    if(m.A[(PTBR*10)+PN][0]=='$')
        {
        cout<<"\nPage Fault";
        }
    else
                makepageentry(PN,getrandom());
  int FN = (m.A[(PTBR*10)+PN][2]-48)*10 + (m.A[(PTBR*10)+PN][3]-48);
    char *ptr=m.A[FN*10];
  j=0;
  while(buffer[j]!=0)
  {

    *ptr++=buffer[j];
    j++;
  }
 }
else
  pi=2;

}

//Load register from ram

void executeLR()
{
if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
{
int word = (c.ir[2]-48)*10 + (c.ir[3]-48);
c.r[0] = m.A[word][0];
c.r[1] = m.A[word][1];
c.r[2] = m.A[word][2];
c.r[3] = m.A[word][3];
}
else
pi=2;
}

//Store register into ram

void executeSR()
{
if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
{int word = (c.ir[2]-48)*10 + (c.ir[3]-48);
m.A[word][0] = c.r[0];
m.A[word][1] = c.r[1];
m.A[word][2] = c.r[2];
m.A[word][3] = c.r[3];
}
else
pi=2;
}

//execute BT

void executeBT()
{
if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
{
int word = (c.ir[2]-48)*10 + (c.ir[3]-48);
if(c.t==1)
	c.ic=word;
}
else
pi=2;
}

//execute CR

void executeCR()
{
if((c.ir[2]>='0' && c.ir[2]<='9') && (c.ir[3]>='0' && c.ir[3]<='9'))
{int word = (c.ir[2]-48)*10 + (c.ir[3]-48);
if(c.r[0]==m.A[word][0] && c.r[1]==m.A[word][1] && c.r[2]==m.A[word][2] && c.r[3]==m.A[word][3])
	c.t=1;
else
	c.t=0;
}
else
pi=2;
}

void error_msg(int n1, int n2=-1)
{
    if(n1==0)
        cout<<"Exe success!";
     if(n1==1)
        cout<<"Out of Data";
     if(n1==2)
        cout<<"Line limit exceeded";
     if(n1==3)
        cout<<"Time Limit exceeded";
     if(n1==4)
        cout<<"Opcode error";
     if(n1==5)
        cout<<"Oprand error";
     if(n1==6)
        cout<<"Invalid page";
    if(n1==3 && n2==4)
      cout<<"time limit exceeded and opcode error";
    if(n1==3 && n2==5)
      cout<<"time limit exceeded and operand error";
}
//code to stimulate the instructions in ram
void mos()
{
if(ti==0 && si==1)
    executeGD();
if(ti==0 && si==2)
    executePD();
if(ti==0 && si==3)
{
    error_msg(0);
    c.display();
    m.display();
    exit(0);
}
if(ti==2 && si==1)
{
    error_msg(3);
    c.display();
    m.display();
    exit(0);
}
if(ti==2 && si==2)
{
    executePD();
    error_msg(3);
    c.display();
    m.display();
    exit(0);
}
if(ti==2 && si==3)
{
    error_msg(0);
    c.display();
    m.display();
    exit(0);
}
if(ti==0 && pi==1)
    error_msg(4);
if(ti==0 && pi==2)
    error_msg(5);
if(ti==2 && pi==1)
{
    error_msg(3,4);
    c.display();
    m.display();
    exit(0);
}
if(ti==2 && pi==2)
{
    error_msg(3,5);
    c.display();
    m.display();
    exit(0);
}
pi=0;
si=0;
ti=0;
}


void simulate()
{
while(1)
	{
	    ttc++;
        if(ttc>ttl)
        ti=2;
	loadir();
	if(c.ir[0]=='G'&&c.ir[1]=='D')
		si=1;
	else if(c.ir[0]=='P'&c.ir[1]=='D')
		si=2;
	else if(c.ir[0]=='H')
		si=3;
	else if(c.ir[0]=='L' && c.ir[1]=='R')
		executeLR();
	else if(c.ir[0]=='S' && c.ir[1]=='R')
		executeSR();
	else if(c.ir[0]=='B' && c.ir[1]=='T')
		executeBT();
	else if(c.ir[0]=='C' && c.ir[1]=='R')
		executeCR();
    else
        pi=1;
    if(c.ir[3]<48 || c.ir[3] >57 || c.ir[4]<48 || c.ir[4] >57 )
		pi=2;
    mos();
	}
}


int main()
{
    PTBR = getrandom(); //get a random number 0-30
 //   PTBR
    cout<<PTBR<<"\n";
    for(int j=0;j<10;j++)
    {
    int base = PTBR*10;
    m.A[base+j][0]='*';
    }
    int i[2],j;
    for(int i=0;i<30;i++)
    cout<<available[i]<<"\n";

	m.init();
	loadprogram();

  //int base=PTBR*10;

   int operation = (((m.A[PTBR*10][2]-48)*10 + m.A[PTBR*10][3]-48)*10);
   cout<<(m.A[(PTBR*10)][2])<<"\n";
	c.ic=operation;
	cout<<c.ic<<"\n";

	simulate();

	m.display();
	c.display();

    input_file.close();
    output_file.close();
	return 0;
}
