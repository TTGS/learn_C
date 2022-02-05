#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*控制产生链表的块数*/
#define chainNum 10

/*链表结构*/
typedef struct LinkChain{
        int id; /*块编号,从0开始*/
        int isUsed; /*是否使用,0未使用,大于0,使用,每次插入数之后加1*/
        int values; /*保存数值*/
        struct LinkChain *nextChain; /*下一个链指针*/
}linkChain;

/*比较块情况的结构体*/
typedef struct CompareChain{
        int oldestIsUsed; /*寻找最早使用的块临时数,linkChain->isUsed*/
        linkChain *equalValuesChain; /*存放值相等指针*/
        linkChain *oldestUsedChain; /*存放最早使用块指针*/
        linkChain *zeroIsUsedChain; /*存放空块指针*/
}compareChain;

/*debug函数,不过没有用上.*/
void write_debug(char *p){
#ifdef debug
	printf("%s \n",p);
#endif
}

/*显示每个块情况.*/
void display(linkChain *h){
linkChain *tmp=h;
        do{
        	printf("current:%p,id:%d,isUsed:%d,values:%d,nextChain:%p \n"
        	          ,tmp,tmp->id,tmp->isUsed,tmp->values,tmp->nextChain);
        	tmp=tmp->nextChain;
        }while(tmp!=h);
}

/*清空块函数,传入头指针.*/
void clearChain(linkChain *h){
linkChain *tmp=h;
        do{
        	tmp->isUsed=0; /*只要isUsed是0,那么values就会被覆盖.*/
                tmp=tmp->nextChain;
        }while(tmp!=h);

}

/*初始化块函数,传入chainNum参数*/
linkChain *initchain(int nums){
linkChain *headChain; /*头块指针*/
linkChain *curr;      /*当前指针*/

int i ;
for(i=0;i<nums;i++){
	linkChain *nChain=(linkChain *)malloc(sizeof(linkChain));
	if(i==0){/*如果是第一个块,那么头块指针和当前块指针就指过去.*/
                headChain=nChain;
		curr=headChain;
	}/*初始化块内容.*/
	curr->nextChain=nChain;
	curr=nChain;
	curr->id=i;
	curr->isUsed=0;
}/*最后一个块的下一个指针指向头块,完成一个环*/
curr->nextChain=headChain;

/*返回头指针*/
return headChain;
}

/*操作函数,传入头指针和用户提供的数据*/
void funChain(linkChain *h,int i){
/*用tmp进行操作,不用h*/
linkChain *tmp=h;
/*初始化比较函数,指针都是null,最早指针和最早使用赋予h的内容.*/
compareChain *cc=(compareChain *)malloc(sizeof(compareChain));
cc->oldestIsUsed=h->isUsed;
cc->oldestUsedChain=h;
cc->equalValuesChain=NULL;
cc->zeroIsUsedChain=NULL;

do{
	if(tmp->values==i){/*如果相等那么赋值,不会有2个相同值的块.*/
                cc->equalValuesChain=tmp;	
	}/*未使用的块可能有多个,我只要第一个.*/
	if(tmp->isUsed==0 && cc->zeroIsUsedChain==NULL){
		cc->zeroIsUsedChain=tmp;
	}/*扫描全部块,只要最早一个使用的块,及它的指针.*/
	if(tmp->isUsed>cc->oldestIsUsed){
		cc->oldestIsUsed=tmp->isUsed;
		cc->oldestUsedChain=tmp;
	}/*只要是使用块,那么就加1,这个是必须的动作.*/
	if(tmp->isUsed!=0){
		tmp->isUsed++;
	}
	tmp=tmp->nextChain;
}while(tmp!=h);/*无论找到的结果是什么,都必须扫描全部块.*/

/*只用优先级:
1,相等
2,空块
3,最早使用块*/
if(cc->equalValuesChain!=NULL){/*使用相等值块*/
	cc->equalValuesChain->isUsed=1;
}else{/*没有等值块*/
        if(cc->zeroIsUsedChain!=NULL){/*使用有空块*/
	        cc->zeroIsUsedChain->isUsed=1;
	        cc->zeroIsUsedChain->values=i;
        }else{/*没有空块,并且没有等值块*/
	        cc->oldestUsedChain->isUsed=1;
	        cc->oldestUsedChain->values=i;
        }
}


}

/*主程序*/
int main(void){
        /*获得头块指针,并且初始化*/
	linkChain *headChain=initchain(chainNum);
        char cmm[100];/*或者输入命令*/

	do{
	        /*提示用户输入命令*/
		printf("enter a number > ");
		scanf("%s",cmm);
		
		/*解析命令*/
		if(strcmp(cmm,"display")==0){/*显示全部块内容*/
				display(headChain);
		}else if(strcmp(cmm,"quit")==0 || strcmp(cmm,"exit")==0){
		                break;/*如果输入exit或者quit,直接推出.*/
		}else if (strcmp(cmm,"clear")==0){/*清空块内容.*/
		               clearChain(headChain);
		}else{/*传递给函数,直接等结果就好了.*/
		                funChain(headChain,atoi(cmm)); 
		}
	}while(1);

/*退出.*/
exit(0);
}
