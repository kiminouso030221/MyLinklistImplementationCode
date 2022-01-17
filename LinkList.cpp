#include<iostream>
#include<string>
#include<sstream>
#include<windows.h>
#include<map>
#include<conio.h>
#include<utility>
#include<vector>

using namespace std;

typedef map<string, string> Data,*Pdata;	//数据域


typedef struct Lnode
{
	unsigned int index;
	Data data;				//数据域
	Lnode* next;	
}Lnode, *Lnklst;


typedef map<string, Lnklst> Listmap, * PListmap;	//链表与string映射

typedef vector<Lnklst> Lheads,Lnodes,*PLheads,*PLnodes;	//链表头与标号映射

bool GetLnodePtrArr(const Lnklst head, PLnodes nodes);

Lnklst GetLnodePtr(const Lnklst head, const int pos);

Lnklst CreateList(const int n);

bool InsertNode(const int n, Lnklst head, const int pos);

bool ListAppend(const Lnklst& dest, const Lnklst& src);

bool DeleteListNode(const int pos, Lnklst& head, const int n);

unsigned int GetListLength(const Lnklst head);

void UpdateData(const Lnklst head, const int pos);

void PrintNodeData(const Lnklst head, const int pos, string elemname, int index);

void menu_Listmgr(PListmap cast);

Lnklst menu_CreatlstIntro(int* n);

void menu_Listmgr(PListmap cast);

bool GetLnodePtrArr(const Lnklst head,PLnodes nodes)	//获得与链表一一对应，从起始点到结束点的指针,数组下标与链表实际顺序相同
{
	Lnklst pnode = head;
	unsigned int cnt = 0;
	while (pnode)
	{
		cnt++;	
		nodes->push_back(pnode);
		pnode = pnode->next;
	}
	return true;
}

Lnklst GetLnodePtr(const Lnklst head, const int pos)	//获得链表pos位置节点
{
	Lnklst pnode = head;
	unsigned int cnt = 0;
	if (pos < 0) return NULL;
	while (cnt!=pos && pnode)
	{
		cnt++;
		pnode = pnode->next;
	}
	return pnode;
}


Lnklst CreateList(const int n) /*新建n个链表*/
{
	if (n <= 0) return NULL;
	Lnklst head;
	unsigned int cnt = 1;
	try
	{
		head = new Lnode;
	}
	catch (const exception& e)		//异常判断
	{
		printf("%s", e.what());
		return NULL;
	}
	Lnklst pre = head;
	pre->index = 0;		//头，标号索引0
	Lnklst pnode;
	while (cnt != n)
	{
		try
		{
			pnode = new Lnode;
		}
		catch(const exception& e)			//异常捕获
		{
			printf("%s", e.what());
			return NULL;
		}
		pnode->index = cnt;
		pre->next = pnode;
		pre = pnode;
		cnt++;
	}
	pre->next = NULL;
	return head;
}

bool InsertNode(const int n, Lnklst head, const int pos)	/*在pos位置链表后面插入n个节点*/
{
	if (n <= 0 || pos < 0 || !head) return 0;
	unsigned int cnt = 0;	
	Lnklst pnode = head;
	while (pnode && cnt!= pos)		//指针移到对应pos位置,head为0
	{
		pnode = pnode->next;
		cnt++;
	}
	if (pnode == NULL) return 0;
	Lnklst next = pnode->next;
	Lnklst pre=pnode;
	while (cnt - pos != n)
	{
		cnt++;
		try
		{
			pnode = new Lnode;
		}
		catch (const exception& e)		//异常捕获
		{
			printf("%s", e.what());
			return false;
		}
		pre->next = pnode;
		pre = pnode;
		pnode->index = cnt;			//标号
	}
	pnode->next = next;
	while (next)			//插入点后标号后移
	{
		next->index += n;
		next = next->next;
	}
	return 1;
}


bool ListAppend(const Lnklst& dest,const Lnklst& src) //把以src为头的链表个附在以dest为头的链表后面
{
	if (dest == NULL) return false;
	Lnklst pnode_dest=dest;
	Lnklst pnode_src = src;
	unsigned int cnt_dest=1;
	while (1)		//获得dest链表个数，以及dest尾部指针
	{
		if (pnode_dest->next)	//如果后面还有
		{
			pnode_dest = pnode_dest->next;
			cnt_dest++;
		}
		else
		{
			pnode_dest->next = src;	//最后一个了，让最后一个的next为src头
			break;
		}
	}
	while (pnode_src)
	{
		pnode_src->index += cnt_dest;	//更新src的标号
		pnode_src = pnode_src->next;
	}

	return true;
}

bool DeleteListNode(const int pos,Lnklst& head,const int n)	//删除从pos开始n个链表,可以删除头,删除头后会改变头指针
{
	if (n <= 0 || pos < 0 || !head) return 0;
	unsigned int cnt = 0;								
	Lnklst pnode = head;
	Lnklst pre = NULL;
	bool flag_deletehead=0;
	if (cnt == pos) flag_deletehead = 1; //删除头判断
	
	while (!flag_deletehead)		//指针移到对应pos位置,head为0
	{
		pnode = pnode->next;
		cnt++;
		if (cnt - pos == -1)	//移动到pos前，保存前面节点的指针以待连接
		{
			pre = pnode;
			cnt++;
			pnode = pnode->next;
			break;
		}
	}

	Lnklst next;		//next获取pnode后一个节点指针
	while (cnt - pos != n )
	{
		cnt++;
		next = pnode->next;
		try
		{
			delete pnode;		//删除pnode，此时pnode的next已经被存储,如果pnode为空，抛出异常;
		}
		catch (const exception& e)		//异常捕获
		{
			printf("%s", e.what());
			return false;
		}
		pnode = next;	//待删除节点pnode设为next,跳出时这个就是要与pre连接的节点
		if (next) next = next->next; //next后移
		else break;
	}
	if (flag_deletehead)	head = pnode; //如果头被删除，新头为pnode
	else pre->next = pnode;
	while (pnode)			//删除后标号前移
	{
		pnode->index -= n;
		pnode = pnode->next;
	}
	return 1;
}

unsigned int  GetListLength(const Lnklst head) //获得链表长度
{
	unsigned int cnt = 0;
	Lnklst pnode = head;
	while (pnode)
	{
		pnode = pnode->next;
		cnt++;
	}
	return cnt;
}

int SearchList(const Lnklst head,string info,const int pos,int* nindex,bool mode)	//从pos节点第*nindex对以head为头的节点查找info信息 nindex可返回条目信息	 mode查找模式，1时不查找*nindex条目（强制specialstart）
{
	unsigned int cnt = 0;
	Lnklst pnode = GetLnodePtr(head,pos);
	bool specialstart=0;
	if (*nindex != 0 || mode) specialstart = 1;		//节点开始不是从第0条开始查，特殊处理标志
	while (pnode)
	{
		unsigned int cnt2 = 0;
		Data::iterator iter = pnode->data.begin();
		if (specialstart)
		{
			while (cnt2 != *nindex+1 && iter!=pnode->data.end())
			{
				iter++;
				cnt2++;
			}
		}
		specialstart = 0;

		while (iter != pnode->data.end())
		{
			if (iter->second == info)
			{
				*nindex = cnt2;
				return cnt;	//找到 返回标号
			}
			cnt++;								//条目号从0开始
			iter++;
		}
		pnode = pnode->next;
		cnt++;		//节点号从0开始
	}	
	return -1;	//未找到 返回-1
}


void PrintNodeData(const Lnklst head, const int pos)	
{
	Lnklst dest = GetLnodePtr(head, pos);

	Data::iterator iter=dest->data.begin();
	int cnt = 0;
	while (iter != dest->data.end())
	{
		printf("%d %10s : %10s\n",cnt, iter->first.c_str(), iter->second.c_str());	//开头为0
		cnt++;
		iter++;
	}
}

bool UpdateNodeData(const Lnklst dest, const int index)	//index起始为0
{
	Data::iterator iter = dest->data.begin();
	int cnt = 0;
	char str1[20], str2[20];
	while (iter != dest->data.end() && cnt!=index)
	{	
		iter++;
		cnt++;
	}
	if (iter == dest->data.end() && cnt == index)
	{
		printf("新内容:\n");
		rewind(stdin);
		scanf_s("%s %s", str1, 20, str2, 20);
		dest->data[str1] = str2;
		return true;
	}
	else if (iter == dest->data.end() && cnt != index) return false;
	printf("原内容:%s %s\n", iter->first.c_str(), iter->second.c_str());
	printf("新内容:\n");
	rewind(stdin);
	scanf_s("%s %s", str1, 20, str2, 20);
	dest->data[str1] = str2;
	return true;
}

void PrintList(const Lnklst head)
{
	Lnklst curr = head;
	while (curr)
	{
		printf("序号:%d\n", curr->index);
		Data::iterator iter = curr->data.begin();
		int cnt = 0;
		while (iter != curr->data.end())
		{
			printf("%d %10s : %10s\n",cnt, iter->first.c_str(), iter->second.c_str());
			cnt++;
			iter++;
		}
		curr = curr->next;
	}
}

bool strtoint(string str,int* n)		//字符串是否为纯数字判断
{
	stringstream sin(str);
	if (!(sin >> *n))return false;
	else return true;
}


Lnklst menu_CreatlstIntro(int *n) 
{
	string str;
	char s[5];
	system("cls");
	printf("输入链表结点数 (输入非数字,数字小于1或生成失败会返回菜单)\n");
	scanf_s("%s", s, 5);
	s[4] = 0;
	fflush(stdin);
	str = s;
	if (strtoint(str, n))
	{
		if (*n <= 0) return NULL;
		return CreateList(*n);
	}
	return NULL;
}

void menu_Listmgr(PListmap cast)
{
	system("cls");
	while (1)
	{
		Listmap::iterator iter = cast->begin();
		int n, cnt = 0;
		printf("现有链表信息如下:\n");
		printf("%20s %5s\n", "名称", "长度");
		string cmd;
		char cmdstr[3];
		bool selectednode = 0;
		bool selectedlist = 0;
		int size = 0;
		while (iter != cast->end())
		{
			cnt++;
			n = GetListLength(iter->second);
			printf("%d %20s %5d\n", cnt, iter->first.c_str(), n);
			iter++;
		}

		printf("总链表数:%d ", cnt);
		int cmdindex = -1, cnt2;
		Lnklst curr = NULL;
		printf("输入想要操作的链表序号,输入非数字则返回菜单\n");
		while (!selectedlist)
		{
			cnt2 = 1;
			cmd.clear();
			scanf_s("%s", cmdstr, 3);
			cmd = cmdstr;
			if (strtoint(cmd, &cmdindex))
			{
				if (cmdindex <= 0 || cmdindex > cnt) printf("未找到该序号,请重新输入\n");
				iter = cast->begin();
				while (cnt2 != cmdindex && iter != cast->end())
				{
					iter++;
					cnt2++;
				}

				if (iter == cast->end()) printf("未找到该序号,请重新输入\n");
				else
				{
					curr = iter->second;
					selectedlist = 1;
					break;
				}
			}
			else return;
		}

		while (selectedlist)
		{
			Lnklst currpnode;
			char cmd2[5];
			int cmd = 0;
			string cmdstr;
			int nindex, n;
			printf("当前选定的链表序号:%d\n", cmdindex);	//iter==curr 
			Lnodes nodes_curr;
			GetLnodePtrArr(curr, &nodes_curr);
			printf("1.打印此链表所有内容\n");
			printf("2.删除此链表的节点\n");
			printf("3.删除此链表\n");
			printf("4.修改此链表的内容\n");
			printf("5.为此链表添加节点\n");
		//	printf("9.将其他链表的内容附加到此链表\n"); 不想写了 累
			printf("6.查询此链表的内容\n");
			printf("7.重命名此链表\n");
			printf("输入要操作的序号，输入其他数字返回上一步\n");
			rewind(stdin);
			scanf_s("%d", &cmd, 1);
			rewind(stdin);
			string nstr;
			switch (cmd)
			{
			case 1:
				PrintList(curr); break;
			case 2:
				while (1)
				{
					printf("输入要删除节点的起始位置数字,输入其他内容则返回上一步\n");
					scanf_s("%s", cmd2, 5);
					cmdstr = cmd2;
					if (strtoint(cmdstr, &nindex))
					{
						printf("已选定节点序号:%d,输入从%d开始删除的节点个数,输入其他内容则返回上一步\n", nindex, nindex);
						fflush(stdin);
						scanf_s("%s", cmd2, 5);
						cmdstr = cmd2;
						if (strtoint(cmdstr, &n))
						{
							DeleteListNode(nindex, curr, n);
							cast[0][iter->first] = curr;		//更新头，头部可能被删除因而头指针变化
							printf("删除成功,按任意键返回\n");
							_getch();
						}
					}
					else break;

				}
				break;
			case 3:
				printf("确定执行链表整体删除操作?输入y继续\n");
				if (getchar() != 'y') break;
				DeleteListNode(0, curr, GetListLength(curr));
				cast->erase(iter);
				printf("删除此链表成功，即将返回链表管理页面\n");
				Sleep(1000);
				selectedlist = 0;
				break;
			case 4:
				while (1)
				{
					printf("输入要修改的节点的数字,输入其他内容则返回上一步\n");
					rewind(stdin);
					scanf_s("%s", cmd2, 5);
					cmdstr = cmd2;
					if (strtoint(cmdstr, &nindex))
					{
						Lnklst pnode = GetLnodePtr(curr, nindex);
						printf("已选定节点%d,输入要修改的节点中的条目序号,输入现有最大序号加1则新建内容,输入其他内容则返回上一步\n", nindex);
						rewind(stdin);
						scanf_s("%s", cmd2, 5);
						cmdstr = cmd2;

						if (strtoint(cmdstr, &n))
						{
							if (UpdateNodeData(pnode, n))
								printf("修改成功,按任意键返回上一步\n");
							else printf("修改失败,条目不存在,按任意键返回上一步\n");
							_getch();
						}
					}
					else break;
				}
				break;
			case 5:
				while (1)
				{
					printf("输入要添加节点的位置,如输入\"5\"将在节点5后面添加链表,输入其他内容则返回上一步");
					int nindex, n;
					rewind(stdin);
					scanf_s("%s", cmd2, 5);
					cmdstr = cmd2;
					if (!strtoint(cmdstr, &nindex)) break;
					printf("当前选定节点序号:%d,输入要添加的节点个数,输入其他内容则返回上一步\n", nindex);
					rewind(stdin);
					scanf_s("%s", cmd2, 5);
					cmdstr = cmd2;
					if (!strtoint(cmdstr, &n));
					else
					{
						if (InsertNode(n, curr, nindex))printf("成功在%d节点后插入%d个节点,按任意键返回上一步\n", nindex, n);
						else printf("插入失败,按任意键返回上一步\n");
					}
					rewind(stdin);
					_getch();
				}
				break;
			case 6:
				while (1)
				{
					char fstr[20];
					printf("输入要查询的内容:\n");
					scanf_s("%s", fstr, 20);
					string info = fstr;
					int index = 0;
					int nindex = 0;
					int cnt = 0;
					while (1)
					{
						index = SearchList(curr, info, index, &nindex, cnt);
						if (index == -1)
						{
							printf("未找到此内容\n");
							break;
						}
						else
						{
							printf("找到其中一个结果,在序号为%d的节点中的第%d条目\n", index, nindex);
							printf("继续查找此内容吗?输入y继续\n");
							rewind(stdin);
						}
						if (getchar() == 'y') cnt++;
						else break;
					}
					break;
				}
				break;

			case 7:
				char name[20];
				printf("输入新名字:\n");
				rewind(stdin);
				scanf_s("%s", name, 20);
				printf("修改为:%s ? 若有重名链表则覆盖!?<y/n>", name);
				rewind(stdin);
				nstr = name;
				if (getchar() == 'y')
				{
					cast[0][nstr] = iter->second;
					cast->erase(iter);
					iter = cast->find(nstr);
					printf("修改成功\n");
				}
				else
				{
					printf("退出修改\n");
				}
				break;

			default: selectedlist = 0;
			}
		
		}
	}
}





int main()
{
	Listmap cast;
	Lheads heads;
	Lheads::iterator iter_h = heads.begin();
	char cmd;
	Listmap::iterator iter = cast.begin();
	string name;
	char namestr[20];
	int cnt = 0;
	int n;
	Lnklst head;
	while (1)
	{
		fflush(stdin);
		system("cls");
		printf("1.创建链表\n");
		printf("2.管理链表\n");
		scanf_s("%c", &cmd,1);
		fflush(stdin);
		switch (cmd)
		{
		case '1':head = menu_CreatlstIntro(&n);
			if (head)
			{
				printf("创建含有%d个节点的链表成功\n", n);
				printf("命名链表:");
				while (1)
				{
					scanf_s("%s", namestr, 20);
					fflush(stdin);
					name = namestr;
					if (cast.find(name) != cast.end())
						printf("名字与现有链表有重名, 请重新输入\n");
					else
					{
						cast[name] = head;
						printf("命名成功,按任意键返回菜单");
						_getch();
						break;
					}
				}
				break;
			}
			else break;

		case '2':menu_Listmgr(&cast);
		}

	}


	return 0;

}