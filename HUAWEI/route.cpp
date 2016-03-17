
#include "route.h"
#include"string.h"
#include"memory.h"
#include "lib_record.h"
#include <algorithm>
#include <stdio.h>
#define INF 32768
#define MaxVertex 600
#define MaxDemand 50
//你要完成的功能总入口 hi
void print_array(int row, int col, int array[][600])
{
	int i, j;
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++)
		{
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
}
void search_route(char *topo[5000], int edge_num, char *demand)
{
	int i,j,k,m,temp,vertex_num=0,demand_num=0,best_gene_num; //暫存变量i,j,k,m,temp,顶点数vertex_num，总需求列表长度demand_num，种群大小best_gene_num
	//拓扑矩阵topo_array，权值矩阵cost，路径矩阵path，边ID矩阵edge_array，总需求列表demand_col，父代及其子代构成的种群best_gene
	int topo_array[5000][4]={0},cost[MaxVertex][MaxVertex],path[MaxVertex][MaxVertex]={0},edge_array[MaxVertex][MaxVertex]={0},demand_col[MaxDemand+2]={0},best_gene[MaxDemand*2][600];
	bool flag;  //标志位
	for(i=0;i<MaxVertex;i++)			//cost矩阵初始化
{
		std::fill_n(cost[i], MaxVertex, INF);
		cost[i][i]=0;
}
	for(i=0;i<edge_num;i++)			//读取topo.csv表至topo_array矩阵
	{
		j=0;k=0;
		flag=true;
		while((*(topo+i))[j]!='\0')
		{
			if(flag)
			{
				temp=int((*(topo+i))[j] - '0');
				j++;
				flag=false;
			}
			else
			{
				temp=temp*10+int((*(topo+i))[j] - '0');
				j++;
			}
			if((*(topo+i))[j]==',')
			{
				topo_array[i][k]=temp;
				flag=true;
				j++;k++;
			}
			if((*(topo+i))[j]==10||(*(topo+i))[j]=='\0')
			{
				topo_array[i][k]=temp;
				edge_array[i][k]=i;             //路径与边ID的对应表
				break;
			}
		}
		if(topo_array[i][1]>vertex_num)vertex_num=topo_array[i][1];     //求顶点数
		if(topo_array[i][2]>vertex_num)vertex_num=topo_array[i][2];		//求顶点数
		cost[topo_array[i][1]][topo_array[i][2]]=topo_array[i][3];				//初次权值矩阵初始化
		path[topo_array[i][1]][topo_array[i][2]]=topo_array[i][2];				//初次路径矩阵初始化
	}
	vertex_num+=1;			//求顶点数
	for(k=0;k<vertex_num;k++)      	//Floyd算法
	{
		for(i=0;i<vertex_num;i++)
		{
			for(j=0;j<vertex_num;j++)
			{
				if(cost[i][j]>(cost[i][k]+cost[k][j]))
				{
					cost[i][j]=cost[i][k]+cost[k][j];
					path[i][j]=k;
				}
			}
		}
	}
j=0;k=0;
flag=true;
while(demand[j]!='\0')   //读取demand.csv表至demand_col
{
	if(flag)
	{
			temp=int(demand[j] - '0');
			j++;
			flag=false;
	}
	else
	{
			temp=temp*10+int(demand[j] - '0');
			j++;
	}
	if(demand[j]==','||demand[j]=='|')
	{
			demand_col[k]=temp;
			flag=true;
			j++;k++;demand_num++;
	}
	if(demand[j]==10||demand[j]=='\0')
	{
			demand_col[k]=temp;demand_num++;
			break;
	}
}
if(demand_num>4)      //需求总长度大于4，即有3个或以上的经由点的初始种群产生
	{
	best_gene_num=(demand_num-2)*2;
	for(i=1;i<demand_num-1;i++)		//产生父代1
	{
		best_gene[0][i]=demand_col[i+1];
	}
	best_gene[0][0]=demand_col[0];best_gene[0][demand_num-1]=demand_col[1];
	for(i=1;i<demand_num-1;i++)		//产生父代2
		{
			best_gene[1][i]=demand_col[demand_num-i];
		}
	best_gene[1][0]=demand_col[0];best_gene[1][demand_num-1]=demand_col[1];
	m=1;
	for(i=2;i<demand_num-1;i++)
	{
		for(j=0;j<demand_num;j++)
		{
			best_gene[i][j]=best_gene[0][j];
		}
		temp=best_gene[i][m];best_gene[i][m]=best_gene[i][m+1];best_gene[i][m+1]=temp;
		m++;
	}
	m=1;
	for(;i<best_gene_num-1;i++)
	{
		for(j=0;j<demand_num;j++)
		{
			best_gene[i][j]=best_gene[1][j];
		}
		temp=best_gene[i][m];best_gene[i][m]=best_gene[i][m+1];best_gene[i][m+1]=temp;
		m++;
	}
	}
else
{
	best_gene_num=demand_num-2;
	for(i=1;i<demand_num-1;i++)		//产生父代1
		{
			best_gene[0][i]=demand_col[i+1];
		}
		best_gene[0][0]=demand_col[0];best_gene[0][demand_num-1]=demand_col[1];
 if(best_gene_num==2)      //若只有一个经由点则只有一个父代，不需要子代
 {
	 for(i=1;i<demand_num-1;i++)		//产生父代2
	 			{
	 				best_gene[1][i]=demand_col[demand_num-i];
	 			}
	 best_gene[1][0]=demand_col[0];best_gene[1][demand_num-1]=demand_col[1];
 }
}
	print_array(vertex_num,vertex_num,cost);
	print_array(vertex_num,vertex_num,path);
	/*
    for(i=0;i<vertex_num;i++)
    	{
    		for(j=0;j<vertex_num;j++)
    				{
    					printf ("%d ",cost[i][j]);
    				}
    		printf("\n");
    	}
    for(i=0;i<vertex_num;i++)
    	{
    		for(j=0;j<vertex_num;j++)
    				{
    					printf ("%d ",path[i][j]);
    				}
    		printf("\n");
    	}
    	*/
	unsigned short result[] = {2, 6, 3};//示例中的一个解
    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}


