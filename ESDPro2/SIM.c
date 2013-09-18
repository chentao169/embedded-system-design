/*
On my honor, I have neither given nor received unauthorized aid on this 
assignment

gcc SIM.c -o SIM

*/



#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef unsigned int U32;
typedef int S32;
typedef unsigned char U8;
typedef char S8;

FILE *pinput = NULL;
FILE *poutput = NULL;
U8 	  globflg = 0;  // 1--compression; 2--decompression
U8    cmpbuff[100*34];
U32	  pbuff=0;
U8	  cntline;

typedef struct instru
{
	U8 cell[33];
	U8 cnt;
}Instr;
Instr list[100];
U8 	  plist=0;
Instr dir[4];

int CountInstr()
{
	U8  temp1[33];
	int i=0;
	char line[40];
	char *pline=line;

	do{
		memset(temp1,0,33);
		memset(line,0,40);
		pline = fgets(line, 40, pinput);
		
		/*if(feof(pinput))
			return -1;	*/
		if(pline==NULL)
			return -1;
		memcpy(temp1,line,32);
		temp1[32]='\0';

		/*for(i=0;i<33;i++)
		{
			printf("%c",temp1[i]);
		}		
		printf("\n");
		*/
		
		if(plist==0 && list[0].cnt==0)
		{
			memcpy(list[0].cell, temp1, 33);
			list[0].cnt=1;
			continue;
		}
		
		for(i=0; i<=plist; i++)
		{	
			if(strcmp((const char *)temp1,(const char *)list[i].cell)==0)
			{
				list[i].cnt+=1;
				break;
			}
		}
		if(i>plist)
		{	
			memcpy(list[++plist].cell, temp1, 33);
			list[plist].cnt=1;
		}
	}while(pline!=NULL);

	/*for(i=0;i<=plist;i++)
	{
		fprintf(poutput,"%d : cnt= %d \n",i,list[i].cnt);
		for(j=0;j<32;j++)
		{
			fprintf(poutput,"%c",list[i].cell[j]);
		}
		fprintf(poutput,"\n");
	}
	*/
	return 0;
	
}

void CreateDic()
{
	int i=0,j=0,q=0;
	Instr temp;

	memset(dir,0,sizeof(dir));

	for(i=0;i<4;i++)
	{
		memcpy(&dir[i],&list[i],sizeof(Instr));
	}
	
	for(i=0;i<4;i++)
	{
		for(j=3;j>i;j--)
		{
			if(dir[j].cnt>dir[j-1].cnt)
			{
				memcpy(&temp,&dir[j],sizeof(Instr));
				memcpy(&dir[j],&dir[j-1],sizeof(Instr));
				memcpy(&dir[j-1],&temp,sizeof(Instr));
			}			
		}
	}
		
	for(i=4;i<=plist;i++)
	{
		for(j=0;j<4;j++)
		{
			if(list[i].cnt > dir[j].cnt)
			{
				for( q=3;q>j;q--)
				{
					memcpy(&dir[q],&dir[q-1],sizeof(Instr));
				}
				memcpy(&dir[j],&list[i],sizeof(Instr));
				break;
			}
		}
	}

}

void Integer2String(U8 *string, U32 num, U8 power,U8 width)
{
	int j=num;
	int temp=1;
 	int i=0;
	U8  usedwid=0;

	for(temp=1; j>0; j/=power)
	{
		temp*=power;
		usedwid++;
	}
	//printf("used width=%d \n",usedwid);

	for(; i<width-usedwid; i++)
		*string++='0';
	
	for(temp/=power;temp>0; temp/=power)
	{
		*string++='0'+num/temp;
		num%=temp;
	}
	 *string='\0';
}


int FindMatch(U8 *instr)
{
	int i=0;
	int j=0;
	U8 	cmpress[34];
	U8  temp[10]={0};
	U8  cnt=0; 
	U8  locate=0;
	U32  diff=0;

	memset(cmpress,0,sizeof(cmpress));
	for(i=0;i<4; i++)
	{
		if(strcmp((const char *)instr, (const char *)dir[i].cell)==0)
		{
			Integer2String(cmpress,1,2,2);
			Integer2String(temp,i,2,2);
			strcat((char *)cmpress,(char *)temp);
			strcat((char *)cmpbuff,(char *)cmpress);
			//printf("%s\n",cmpress);
			return 0;
		}	
	}

	
	for(i=0;i<4;i++)
	{	
		cnt=locate=diff=0;
		for(j=0;j<32;j++)
		{
			if((instr[j]!=dir[i].cell[j])&&(instr[j+1]!=dir[i].cell[j+1])&&\
				(instr[j+2]!=dir[i].cell[j+2])&&(instr[j+3]!=dir[i].cell[j+3]))
			{	
				cnt=3;
				locate=j;
				diff++;
				j+=3;
				continue;
			}
			if(instr[j]!=dir[i].cell[j]&&instr[j+1]!=dir[i].cell[j+1]&&\
				instr[j+2]!=dir[i].cell[j+2])
			{
				cnt=2;
				locate=j;
				diff++;
				j+=2;
				continue;
			}
			if(instr[j]!=dir[i].cell[j]&&instr[j+1]!=dir[i].cell[j+1])
			{	
				cnt=1;
				locate=j;
				diff++;
				j+=1;
				continue;
			}
			if(instr[j]!=dir[i].cell[j])
			{	
				cnt=0;
				locate=j;	
				diff++;
				continue;
			}
			
		}
		
		if(diff==1)
		{
			Integer2String(cmpress,2,2,2);
			Integer2String(temp,cnt,2,2);
			strcat((char *)cmpress,(char *)temp);
			memset(temp,0,sizeof(temp));
			Integer2String(temp,locate,2,5);
			strcat((char *)cmpress,(char *)temp);
			memset(temp,0,sizeof(temp));
			Integer2String(temp,i,2,2);
			strcat((char *)cmpress,(char *)temp);
			strcat((char *)cmpbuff,(char *)cmpress);

			//printf("%s\n",cmpress);
			return 0;
		}		
	}
	
	Integer2String(cmpress,0,2,2);
	strcat((char *)cmpress,(char *)instr);
	strcat((char *)cmpbuff,(char *)cmpress);

	//printf("%s\n",cmpress);
	return 0;
	
}

int Compress()
{
	U8  temp1[33];
	char line[40];
	char *pline=line;

	do{
		memset(temp1,0,33);
		memset(line,9,35);
		pline = fgets(line, 40, pinput);
		if(pline==NULL)
			return -1;
		memcpy(temp1,line,32);
		temp1[32]='\0';
		cntline++;
		FindMatch(temp1);
		
	}while(pline!=NULL);
	return 0;

}

void ShowCmpre()
{
	int i=0;
	int j=0;
	U8  cnt=0;
	float ratio=0;
	U8  buff[10]={0};
	U32 temp=0;
	
	while(cmpbuff[i]!='\0')
	{
		fprintf(poutput,"%c",cmpbuff[i]);
		i++;
		if(i%32==0)
		{
			cnt++;
			fprintf(poutput,"\n");
		}
	}
	if(i%32)
	{	
		cnt++;
		for(i=i%32;i<32;i++)
			fprintf(poutput,"0");
	}
	fprintf(poutput,"\n222\n");
	
	for(i=0;i<4;i++)
	{	
		for(j=0;j<32;j++)
			fprintf(poutput,"%c",dir[i].cell[j]);
		fprintf(poutput,"\n");
		cnt++;
	}
	fprintf(poutput,"222\n");
	ratio=(float)cnt/(float)cntline;

	temp=(int)(ratio*100000);
	Integer2String(buff, temp, 10, 5);
	
	if(buff[4]>='5'&& buff[4]<='9')
		buff[3]+=1;
	for(i=3;i>0;i--)
	{	
		if(buff[i]>'9')
		{
			buff[i]='0';
			buff[i-1]+=1;
		}
	}
	buff[4]=buff[3];
	buff[3]=buff[2];
	buff[2]='.';

	fprintf(poutput,"%s\n",buff);
}

void ReadDecomp()
{
	U8	 buff[33]={0};
	U8   i=4;
	char line[40];
	char *pline=line;
	
	do{
		memset(buff,0,33);
		memset(line,0,40);
		pline=fgets(line, 40, pinput);
				
		if(line[0]=='2')
		{
			for(i=0;i<4;i++)
			{
				memset(buff,0,33);
				memset(line,0,40);
				fgets(line,40,pinput);
				memcpy(buff,line,32);
				buff[32]='\0';
				memcpy(dir[i].cell, buff, 33);
			}
			return ;
		}
		else
		{		
			memcpy(buff,line,32);
			buff[32]='\0';
			strcat((char *)cmpbuff,(char *)buff);
		}

	}while(pline!=NULL);
	
}

int FetchDecmp(U8 * string, U8 length)
{
	U8 i=0;
	
	for(i=0;i<length;i++)
	{
		if(cmpbuff[pbuff]!='\0')
			*string++ = cmpbuff[pbuff++];
		else
			return -1;
	}
	*string='\0';
	return 0;
}
U8 Str2Bin(U8 *string)
{
	U8 i=0;
	while(*string!='\0')
	{	
		i+=*string-'0';
		string++;
		i=i<<1;
	}
	i=i>>1;
	return i;
}
void Decompress()
{
	U8   temp[6]={0};
	U8   code[33];
	U32  i=0;
	int	 j=0;
	int  rv=0;
	int  rv2=0;
	U8   locate;
	U8	 rang;
	
	ReadDecomp();
	pbuff=0;
	plist=0;
	while(rv!=-1)
	{	
		i=0;
		memset(temp,0,6);
		rv=FetchDecmp(temp,2);
		i=Str2Bin(temp);
		switch(i)
		{
			case 0: 
				memset(code,0,33);
				rv2=FetchDecmp(code,32);
				if(rv2!=-1)
				{
					//printf("%s\n",code);
					memcpy(list[plist++].cell,code,33);
					break;
				}
				else 
					return;
		
			case 1: 
				memset(temp,0,6);
				FetchDecmp(temp,2);
				j=Str2Bin(temp);
				memcpy(list[plist++].cell,dir[j].cell,33);
				//printf("dir %d %s\n",j,dir[j].cell);
				break;

			case 2: 
				memset(temp,0,6);
				FetchDecmp(temp,2);
				rang=Str2Bin(temp);
				memset(temp,0,6);
				FetchDecmp(temp,5);
				locate=Str2Bin(temp);
				memset(temp,0,6);
				FetchDecmp(temp,2);
				j=Str2Bin(temp);
				memcpy(list[plist++].cell,dir[j].cell,33);
				for(j=locate;j<rang+locate+1;j++)
				{
					if(list[plist-1].cell[j]=='1')
						list[plist-1].cell[j]='0';
					else
						list[plist-1].cell[j]='1';
				}
				//printf("%s\n",list[plist-1].cell);
				break;
		}
	}

}

int main(int argc, char** argv)
{
	int i=0;
	if(argc < 4)  //estimate the num of arguments
	{
		perror("there are not enough arguments!\n");
		return -1;
	}
	
	pinput = fopen(argv[2], "r"); //open  file
	if (pinput == NULL)
    {
        printf("Could not open %s\n" , argv[1]);
        return -__LINE__;
    }
	
	poutput = fopen(argv[3], "w"); //open output file
	if (poutput == NULL)
    {
        printf("Could not open %s\n" , argv[2]);
        return -__LINE__;
    }

	if(argv[1][1]=='c')	
		globflg = 1;

	if(argv[1][1]=='d')
		globflg = 0;

	if(globflg)
	{
		CountInstr();
		CreateDic();

		memset(list, 0, sizeof(list));
		fseek(pinput,0L,SEEK_SET);	
		Compress();
		ShowCmpre();
		fclose(pinput);
		fclose(poutput);
	}
	else
	{
		Decompress();
		for(i=0;i<plist;i++)
			fprintf(poutput,"%s\n",list[i].cell);
	}
	return 0;
	
}



