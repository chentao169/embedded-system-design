/*
g++ PNsim.c++ -o PNsim

On my honor, I have neither given nor received unauthorized aid on this assignment

Name: Tao Chen
UFID: 18816215

*/

#include<fstream>
#include<iostream>
#include <string.h>

typedef  unsigned char u8;
typedef  unsigned int u32;

using namespace std;

typedef struct place
{
	u8 penable;
	u8 tnum;
}PLACE;
PLACE Parray[15];

typedef struct trans
{
	PLACE inplace[15];
	PLACE outplace[15];
	u8 enable;
}TRANS;
TRANS tarray[15];

ifstream fin;
ofstream fout;
u8 tarstep=0;
u8 stopflag=1; //0: continue 1:stop

void SkipChar(int skip)
{
	u8 temp;
	for(;skip>0;skip--)
		temp=fin.get();
}

int StoreStep()
{
	u8 temp1=0;
	u8 temp2=0;
	
	while((temp1=fin.get())!='\n')
	{
		if(temp1>='0'&& temp1<='9')
		{
			tarstep = temp1-48;
			if((temp2=fin.get())>='0' && temp2<='9')
				tarstep= tarstep*10 + temp2-48;
		}
	}
	
	return 0;
}

int StorePlace()
{
	u8 temp1=0;
	u8 temp2=0;
	u8 pname=0;
	
	while((temp1=fin.get())!='\n')
	{
		if(temp1>'0'&& temp1<='9')
		{
			pname=temp1-48;
			if((temp1=fin.get())>='0'&&temp1<='9')
				pname=pname*10+temp1-48;
			Parray[pname].penable=1;
			if((temp2=fin.get())>='0' && temp2<='9')
				Parray[pname].tnum=temp2-48;
		}	
	}
	
/*	for(temp1=0;temp1<15;temp1++)
		if(Parray[temp1].penable)
			cout<<"place name: "<< (u32)temp1<< "  token num: "<<(u32)Parray[temp1].tnum<<endl;
*/		
	return 0;
}

void StoreTrans()
{
	u8 temp1=0;
	u8 temp2=0;
	u8 traname=0;
	u8 pname=0;
	
	while((temp1=fin.get()) !='\n')
	{
		while((temp1=fin.get()) !='(');
		temp1=fin.get();
		temp1=fin.get();
		if(temp1>'0'&& temp1<='9')
		{	
			traname = temp1-48;
			if((temp1=fin.get())>='0' && temp1<='9')
				traname = traname*10+temp1-48;
			//cout<<"StoreTrans: transition name: "<<(u32)traname<<endl;
			
			while((temp1=fin.get())!= '<');
			
			while((temp1=fin.get())!='>')
			{
				if(temp1=='P')
				{
					if((temp1=fin.get())>='0' && temp1<='9')
						pname=temp1-48;
					if((temp1=fin.get())>='0'&& temp1<='9')
						pname=pname*10+temp2-48;
					//cout<<"StoreTrans: inplace name:"<<(u32)pname<<endl;
					tarray[traname].inplace[pname].penable=1;
				}
			}	
			
			while((temp1=fin.get())!= '<');

			while((temp1=fin.get())!=')')
			{
				if(temp1=='P')
				{
					if((temp1=fin.get())>='0' && temp1<='9')
						pname=temp1-48;
					if((temp1=fin.get())>='0'&& temp1<='9')
						pname=pname*10+temp2-48;
					//cout<<"StoreTrans: outplace name:"<<(u32)pname<<endl;
					tarray[traname].outplace[pname].penable=1;
				}
			}		
		}
	}
}

int FindArc(u8 value)
{
	u8 i=0;
	u8 j=0;
	
	for(i=1;i<15;i++)
	{
		for(j=1;j<15;j++)
		{
			if(tarray[i].inplace[j].penable)
			{	
				if(!tarray[i].inplace[j].tnum)
				{	
					tarray[i].inplace[j].tnum=value;	
					return 1;					
				}
			}
		}
		for(j=1;j<15;j++)
		{
			if(tarray[i].outplace[j].penable)
				if(!tarray[i].outplace[j].tnum)
					{	
						tarray[i].outplace[j].tnum=value;	
						return 1;					
					}
		}
	}	
	return 0;	
}

void StoreArcs()
{
	u8 temp1=0;
	u8 temp2=0;
	u8 i=0;
	u8 j=0;
	u8 arcv=0;
	int rv=0;
	
	while((temp1=fin.get()) !='\n')
	{
		while((temp2=fin.get())!=',');
		if((temp1=fin.get())>='0' && temp1<='9')
		{
			arcv=temp1-48;
			if((temp1=fin.get())>='0'&&temp1<='9')
				arcv=arcv*10+temp1-48;
			//cout<<"arc value: "<<(u32)arcv<<endl;
			rv=FindArc(arcv);
			//if(!rv)
			//	cout<<"something wrong!"<<endl;		
			
		}
	}
}

void ProceedIn()
{
	u8 temp1=0;
	u8 temp=1;
	u8 i=0;
	u8 j=0;
	u8 cnt=0;

	for(i=1;i<15;i++)
	{	
		temp=1;
		cnt=0;
		for(j=1;j<15;j++)
		{
			if(Parray[j].penable & tarray[i].inplace[j].penable )
			{	
				temp1 = Parray[j].tnum >= tarray[i].inplace[j].tnum? 1:0;
				temp = temp&temp1;
			}
			else cnt++;
		}
		
		if(temp==1 && cnt !=14)
		{
			for(j=1;j<15;j++)
			{
				if(Parray[j].penable & tarray[i].inplace[j].penable)
				{	Parray[j].tnum =Parray[j].tnum-tarray[i].inplace[j].tnum;
					//cout<<"proceed in trans num: "<<(u32)i<<" token num : "<<(u32)tarray[i].inplace[j].tnum;
					//cout<<" place num: "<<(u32)j<<" token num : "<<(u32)Parray[j].tnum<<endl;
				}
			}
			tarray[i].enable=1;
			stopflag =0;
		}
	}
}

void ProceedOut()
{
	u8 i=0;
	u8 j=0;
	
	for(i=1;i<15;i++)
	{
		if(tarray[i].enable)
		{
			for(j=1;j<15;j++)
			{
				if(Parray[j].penable & tarray[i].outplace[j].penable)
				{	
					Parray[j].tnum += tarray[i].outplace[j].tnum;
					//cout<<"proceed out trans num: "<<(u32)i<<" token num : "<<(u32)tarray[i].outplace[j].tnum;
					//cout<<" place num: "<<(u32)j<<" token num : "<<(u32)Parray[j].tnum<<endl;
				}
			}
			tarray[i].enable=0;
		}
	}

}


int main(int argc, char *argv[])
{
	if(argc<3)
	{
		cout<<"there are not enough parameters!"<<endl;
		return -1;
	}
	
	fin.open(argv[1]);
	fout.open(argv[2]);
	if(!fin.is_open() | !fout.is_open())
	{
		cout<<"cann't open files!\n";
		return -1;
	}
	//cout<<"open file suceed!\n";
	
	memset(Parray,0,sizeof(Parray));
	memset(tarray,0,sizeof(tarray));
	
	StoreStep();
	//cout<<"target step: "<<(unsigned int)tarstep<<endl;
	StorePlace();
	StoreTrans();
	StoreArcs();
	
	u8 i=0;
	u8 step=0;
	while(step<=tarstep)
	{
		fout<<"Step "<<(u32)step<<":";
		for(i=0;i<15;i++)
		{	
			if(Parray[i].penable)
			{
				fout<<"(P"<<(u32)i<<","<<(u32)Parray[i].tnum<<")";
				break;
			}
		}
		for(++i;i<15;i++)
		{
			if(Parray[i].penable)
				fout<<",(P"<<(u32)i<<","<<(u32)Parray[i].tnum<<")";
		}
		fout<<'\n';
	
		ProceedIn();
		if(!stopflag)
		{
			ProceedOut();
			stopflag=1;
			step++;
		}
		else return -1;	
	}
	return 0;

}
