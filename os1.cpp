#include<iostream>
#include<fstream>
#include<stdlib.h>
using namespace std;
ifstream input_file("P1");
ofstream output_file("OUTPUT");
class cpu
{
public:
  char IR[4];
  char R[4];
  int IC;
  int T;
	cpu()
	{
	IC=0;
	T=0;
	IR[0]=0;
	R[0]=0;
	}
  void display()
  {
     cout<<endl<<"IR:"<<IR<<endl;
     cout<<"R :"<<R<<endl;
     cout<<"IC:"<<IC<<endl;
     cout<<"T :"<<T<<endl;
  }

};


class ram
{
public:
  char A[100][4];
  ram()
  {
     for(int i=0;i<100;i++)
     {
       for(int j=0;j<4;j++)
	 {
	  A[i][j]=0;
	 }
     }
   }
void init()
{
     for(int i=0;i<100;i++)
     {
       for(int j=0;j<4;j++)
	 {
	  A[i][j]=0;
	 }
     }
}
void display()
  {
     cout<<"\nContents of RAM:"<<endl;

     for(int i=0;i<100;i++)
     {
       cout<<i<<":";
       for(int j=0;j<4;j++)
	 {
	    cout<<A[i][j];
	 }
	 cout<<"\t";
     }
     cout<<endl;
  }
};
//---------------------------------------------
cpu CPU;
ram RAM;
char buffer[41];
//---------------------------------------------
void load_program()
{
char *ptr=RAM.A[0];
input_file.getline(buffer,41);
if(buffer[0]!='$')
	return;
for(int j=0;j<40;j++)
	buffer[j] = 0;
input_file.getline(buffer,41);
while(buffer[0]!='$')
	{
	for(int i=0;i<40 & buffer[i]!=0;i++)
		*ptr++=buffer[i];
	for(int j=0;j<40;j++)
	buffer[j] = 0;
	input_file.getline(buffer,41);
	}
}
//---------------------------------------------
void load_IR()
{
CPU.IR[0]=RAM.A[CPU.IC][0];
CPU.IR[1]=RAM.A[CPU.IC][1];
CPU.IR[2]=RAM.A[CPU.IC][2];
CPU.IR[3]=RAM.A[CPU.IC][3];
CPU.IC++;
}
//---------------------------------------------
void execute_GD()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
char *ptr = &RAM.A[word][0];
for(int j=0;j<40;j++)
	buffer[j] = 0;
input_file.getline(buffer,41);
for(int i=0;i<40;i++)
	{
	*ptr = buffer[i];
	ptr++;
	}
}
//--------------------------------------------
void execute_PD()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
char *ptr = &RAM.A[word][0];
for(int j=0;j<40;j++)
	buffer[j] = 0;
int i;
for(i=0; i<40 && *ptr!=0 ;i++)
	{
	buffer[i] = *ptr;
	ptr++;
	}
buffer[i]='\n';
output_file.write(buffer,40);
}
//--------------------------------------------
void execute_LR()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
CPU.R[0] = RAM.A[word][0];
CPU.R[1] = RAM.A[word][1];
CPU.R[2] = RAM.A[word][2];
CPU.R[3] = RAM.A[word][3];
}
//--------------------------------------------
void execute_SR()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
RAM.A[word][0] = CPU.R[0];
RAM.A[word][1] = CPU.R[1];
RAM.A[word][2] = CPU.R[2];
RAM.A[word][3] = CPU.R[3];
}
//--------------------------------------------
void execute_BT()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
if(CPU.T==1)
	CPU.IC=word;
}
//--------------------------------------------
void execute_CR()
{
int word = (CPU.IR[2]-48)*10 + (CPU.IR[3]-48);
if(CPU.R[0]==RAM.A[word][0] && CPU.R[1]==RAM.A[word][1] && CPU.R[2]==RAM.A[word][2] && CPU.R[3]==RAM.A[word][3])
	CPU.T=1;
else
	CPU.T=0;
}
//--------------------------------------------
void START_SIMULATION()
{
while(1)
	{
	load_IR();
	if(CPU.IR[0]=='G'&&CPU.IR[1]=='D')
		execute_GD();
	else if(CPU.IR[0]=='P'&&CPU.IR[1]=='D')
		execute_PD();
	else if(CPU.IR[0]=='H')
		return; 
	else if(CPU.IR[0]=='L' && CPU.IR[1]=='R')
		execute_LR(); 
	else if(CPU.IR[0]=='S' && CPU.IR[1]=='R')
		execute_SR(); 
	else if(CPU.IR[0]=='B' && CPU.IR[1]=='T')
		execute_BT(); 
	else if(CPU.IR[0]=='C' && CPU.IR[1]=='R')
		execute_CR(); 
	}
}
int main()
{
//CPU.display();
//RAM.display();
//input_file.getline(buffer,40);
//cout<<buffer;
RAM.init();
load_program();
CPU.IC=0;
START_SIMULATION();


CPU.display();
RAM.display();
input_file.close();
output_file.close();
return 0;
}












