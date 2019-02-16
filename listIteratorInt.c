/*
  listIteratorInt.c : list Iterator ADT implementation
  Written by Alan
  Date: 2018-2019
*/

// 库 
#include <stdlib.h>		// malloc
#include <stdio.h>
#include <assert.h>		// assert(我觉得可写可不写)
#include "listIteratorInt.h"

/*
  难点其实就是add和delete这两个函数，因为分析的情况比较多
  它们两个的写法不唯一，我的写法比较清晰，也就是比较傻瓜式 :D 
*/


// ----------------------------------------------------------------------------------
// 节点（常规固定写法） 
typedef struct Node {
	// 前 
	struct Node *prev;
	// 后 
	struct Node *next;
	// 数据 
	int data;
} Node;

// 迭代器/封装器（可自行添加节点，指针或数据）
// 反正就是怎么方便怎么对下面程序有利就怎么来
typedef struct IteratorIntRep {
	// 头（必须的，链表必须有起始节点和中止节点(中止一般就是NULL)） 
	Node *head;
	// 定义两个光标
	// 前光标 
	Node *cursor_prev;
	// 后光标 
	Node *cursor_next;
	/*
	  两个光标的意思：前光标指向 ^ 前面那个，后光标指向 ^ 后面那个
	    例1： 1 2 ^ 3 4
	          cursor_prev 指向 2
			  cursor_next 指向 3
			 
	    例2： ^ 5 6 7
	          cursor_prev 指向 NULL
			  cursor_next 指向 5
			 
	    例3： 8 9 ^
	          cursor_prev 指向 9
			  cursor_next 指向 NULL
	*/
	
	// 然后delete部分要根据上一个函数来删除元素，所以再定义一个指针，指向要删除的元素
	// 并且根据题意，del这个指针只有在next或previous或findNext或findPrevious中才有指向
	// 在其他方程中应该设置为NULL 
	// 换句话说，这个要删除的节点del，不为NULL时说明成功，为NULL代表失败 
	Node *del;
	
} IteratorIntRep;
// ----------------------------------------------------------------------------------



/*
  hint: 
    Majority of functions are required to consider a situation below: 
      if "IteratorInt it"(doubly linked list) is not exists
      
	  the code is like:
        if(!it){
    		return; / retunr NULL; / return 0;
		}
*/



// ==================================================================================
// 新建双向链表 
IteratorInt IteratorIntNew(){
	// 给双向链表doubly_linked_list分配空间
	struct IteratorIntRep *doubly_linked_list = malloc(sizeof(struct IteratorIntRep));
	// doubly_linked_list不为NULL才继续 
	assert(doubly_linked_list != NULL);
	// 初始化迭代器中的指针 
	// 将双向链表doubly_linked_list的头令为NULL 
	doubly_linked_list -> head = NULL;
	// 将光标初始化 
	doubly_linked_list -> cursor_prev = NULL;
	doubly_linked_list -> cursor_next = NULL;
	// 将del初始化 
	doubly_linked_list->del = NULL;
	// 建好，返回此双向链表 
	return doubly_linked_list;
}


// 重置光标到双向链表的开头 
/*
  也就是要弄成这种： ^  head  （前光标指向NULL，后光标指向head） 
*/ 
void reset(IteratorInt it){
	// 链表必须存在才能进行如下操作
	// 判读条件主要是防止链表不存在或者为NULL这种超特殊情况 
	if(it){
		// 前光标
		it->cursor_prev = NULL;
		// 后光标 
		it->cursor_next = it->head;
		
		// 不是那4个方程就让del为NULL 
		it->del = NULL;
	}
}


// （自行添加函数）新建节点函数 
Node *newNode(int v){
	// 给新节点分配空间 
	Node *new_node = malloc(sizeof(struct Node));
	// assert 一下 
	assert(new_node != NULL);
	// 节点里面的数据 
	new_node->data = v;
	// 节点的前和后 
	new_node->prev = NULL;
	new_node->next = NULL;
	// 建立好后，返回这个节点
	return new_node;
}


// 向双向链表中添加元素 
// 若链表不为空，那么添加元素总是在 cursor_prev 位置 
// 也就是说，new_node添加到在cursor_prev位置，cursor_prev后移一位 
/*
  添加元素共3种不同情况：(第二种情况其实可以归纳为1种情况)
    1.    NULL
    2.1   head  ^
    2.2   head  node_1  ^
    2.3   head  ^  node_1
    2.4   node_1  ^  node_2
    3.    ^  head
  
  其实也就是这3种情况：
  (因为添加元素总是在cursor_prev位置，所以不需要考虑cursor_next的位置)
    1. head==NULL && cursor_prev==NULL
    2. head!=NULL && cursor_prev!=NULL
    3. head!=NULL && cursor_prev==NULL
*/
int add(IteratorInt it, int v){
	
	printf("  add --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		// 因为链表不存在，那么it->XX这种写法是非法的 
		return 0;
	}
	
	// 新建节点 
	Node *new_node = newNode(v);
	
	// 到这里，没有return 0的话，那肯定是成功了（return 1） 
	 
	// 3种情况的一种写法
	// 直接写成"if，else if，else if"更好懂，更清晰明了一点 
	// 可能情况1：也就是链表为空的情况 
	// 其实 it->cursor_prev==NULL 没有必要写，因为头节点为NULL代表里面没有节点，
	// 那么 cursor_prev 肯定为 NULL 
	if(it->head==NULL && it->cursor_prev==NULL){
		// 相当于初始化头节点 
		it->head = new_node;
	}
	// 可能情况2：链表不为空，cursor_prev不为NULL 
	else if(it->head!=NULL && it->cursor_prev!=NULL){
		// 2.4(和2.3)是一般情况，所以可以先看2.4，然后看其他较为特殊的情况
		// 最后精简代码就行，也就是合在一起写
		// 将2.1，2.2，2.3，2.4合在一起写的写法如下： 
		// 首先，改变 new_node 的前和后 
		new_node->prev = it->cursor_prev;
		new_node->next = it->cursor_prev->next;
		// 然后，判断 
		if(new_node->prev != NULL){
			// 不为空才能继续，因为NULL是没有next或者prev 
			// 写成 new_node->prev->next 稳一点，不容易出错，也好理解一点 
			// （也就是说，写成 it->cursor_prev->next 也行） 
			new_node->prev->next = new_node;
		}
		if(new_node->next != NULL){
			new_node->next->prev = new_node;
		}
	}
	// 可能情况3：链表不为空，cursor_prev为NULL 
	// 也就是光标在head之前，所以插入节点的话，就要插到head之前 
	// 这种情况就需要重置head 
	else if(it->head!=NULL && it->cursor_prev==NULL){
		// 将两个节点连上 
		it->head->prev = new_node;
		new_node->next = it->head;
		// 重置head
		it->head = new_node;
	}
	
	// 插完之后，cursor_prev需要被重置，也就是指向新插入的那个节点 
	// 而cursor_next并不需要重置 
	// 下面这句代码是简化后的写法
	// 因为每个判断条件中都要写，所以可以直接提出来写外面 
	it->cursor_prev = new_node;
	
	// 不是那4个方程就让del为NULL 
	it->del = NULL;
	
	// 成功，所以返回1 
	return 1;
}


// 看双向链表中的光标后面有没有下一个 
/*
  一共2种情况：（第一种非常特殊，第二种比较特殊，第三种是普遍情况） 
    1.  链表不存在 
    2.1 链表中没有节点（也就是没有头节点） 
    2.2 光标在链表的末尾 
*/ 
int hasNext(IteratorInt it){
	
	printf("  hasNext --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		// 因为链表不存在，那么it->XX这种写法是非法的 
		return 0;
	}
	
	// 然后现在链表是存在的 
	// 此时的头节点可能为NULL，前光标可能为NULL，后光标可能为NULL
	// 下一种情况：
	// "链表中没有节点（也就是没有头节点）"和"光标 ^ 在链表末尾"可以合在一起写 
	// 不过这里只需要看后光标就行 
	if(it->cursor_next == NULL){
		// 但是在返回0之前需要令del为NULL，因为这里并没有成功调用hasNext函数 
		it->del = NULL;
		return 0;
	}
	 
	// 上面的条件语句都没有return 0，那么就说明双向链表的光标后面肯定有上一个
	// 所以应该返回1，但是在返回1之前需要将del重置 
	// 不是那4个方程就让del为NULL 
	it->del = NULL;
	return 1;
}


// 看双向链表中的光标前面有没有上一个
/*
  这里的话，共有2种情况：
    1.  链表不存在
    2.1 链表中没有节点（也就是没有头节点）
    2.2 光标在链表的开头
*/ 
int hasPrevious(IteratorInt it){
	
	printf("  hasPrevious --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return 0;
	}
	
	// 然后链表存在
	// 此时的头节点可能为NULL，前光标可能为NULL，后光标可能为NULL
	// 下一种情况：
	// "链表中没有节点（也就是没有头节点）"和"光标 ^ 在链表开头"可以合在一起写 
	// 不过这里只需要看前光标就行 
	if(it->cursor_prev == NULL){
		// 但是在返回0之前需要令del为NULL，因为这里并没有成功调用hasPrevious函数 
		it->del = NULL;
		return 0;
	}
	
	// 上面的条件语句都没有return 0，那么就说明双向链表的光标前面肯定有上一个
	// 所以应该返回1，但是在返回1之前需要将del重置 
	// 不是那4个方程就让del为NULL 
	it->del = NULL;
	return 1;
}


// 指针函数 
// 使光标向下一个方向移动
int *next(IteratorInt it){
	
	printf("  next --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return NULL;
	}
	
	// 然后，看有没有下一个，直接用hasNext函数判断就行 
	if(hasNext(it)){
		// 有的话，先改变del指针指向 
		it->del = it->cursor_next;
		// 然后移动光标（前/后光标）
		it->cursor_prev = it->cursor_next;
		it->cursor_next = it->cursor_next->next;
		// 最后返回要删除的值
		// 不过注意这里的写法，因为是指针函数，所以不能直接返回(it->del->data)
		// 因为是指针函数，需要是指针类型的返回值，
		// （如果要接收这个函数的返回值的话，需要用同样类型的指针接收） 
		// 所以这里用指针指向数据的地址来暂存data 
		int *p = &(it->del->data);
		return (p);
	}
	
	// 如果上面的if条件判断为false，也就是没有进入if条件中执行程序，
	// 那么就说明失败了，所以由题意，返回NULL 
	// 当然写成： else{return NULL;} 也行 
	// 但是在返回NULL之前需要令del为NULL，因为这里并没有成功调用next函数 
	it->del = NULL;
	return NULL;
}


// 指针函数 
// 使光标向上一个方向移动
int *previous(IteratorInt it){
	
	printf("  previous --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return NULL;
	}
	
	// 然后，看有没上一个，直接用hasPrevious函数判断就行 
	if(hasPrevious(it)){
		// 有的话，先改变del指针指向
		it->del = it->cursor_prev;
		// 然后移动光标（前/后光标）
		it->cursor_next = it->cursor_prev;
		it->cursor_prev = it->cursor_prev->prev;
		// 最后返回要删除的值
		// 因为是指针函数，需要是指针类型的返回值，
		// （如果要接收这个函数的返回值的话，需要用同样类型的指针接收） 
		// 所以这里用指针指向数据的地址来暂存data
		int *p = &(it->del->data);
		return (p);
	}
	
	// 如果上面的if条件判断为false，也就是没有进入if条件中执行程序，
	// 那么就说明失败了，所以由题意，返回NULL 
	// 当然写成： else{return NULL;} 也行
	// 但是在返回NULL之前需要令del为NULL，因为这里并没有成功调用previous函数
	it->del = NULL;
	return NULL;
}


// 删除光标处的元素（其实是删除del指向的节点）
/*
  分2种情况讨论：
    
    1. 头节点需要重置 
    1.1  ^  head(del)  node
    1.2  ^  head(del)
    1.3  head(del)  ^  node
    1.4  head(del)  ^
    
    2. 头节点不需要重置 
    2.1  head  node_1(del)  ^  node_2
    2.2  head  node_1(del)  ^
    2.3  head/node  ^  node_1(del)  node_2
    2.4  head/node  ^  node_1(del)
*/ 
int deleteElm(IteratorInt it){
	
	printf("  deleteElm --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return 0;
	}
	
	// 必须成功调用4个函数之一，也就是del不为NULL 
	// 四个函数中令 it->del = NULL 的操作就是为了如下这个判断条件 
	// 当del这个指针有所指向的时候才进行删除操作 
	// 这里其实自动排除了head为NULL的情况，因为del为NULL，那么head肯定为NULL 
	if(it->del != NULL){
		// 首先改变前后光标的指向（反正后面又不用光标）
		// 因为it->del存在，所以对it->del可以用prev和next
		it->cursor_prev = it->del->prev;
		it->cursor_next = it->del->next;
		
		// 需要重置head的情况，也就是需要删除head节点 
		if(it->del == it->head){
			// 移动head节点，向head节点的下一个的方向移动 
			it->head = it->head->next;
			// 判断新的head节点是否为NULL 
			if(it->head != NULL){
				// 若不为NULL，则改变head节点的prev为NULL 
				it->head->prev = NULL;
			}
			// 释放del指向的节点
			free(it->del);
			// 然后令del指向NULL，因为 deleteElm 并不是3个函数之一
			// 更深层的意思就是，让del不再指向原来的地址，而是令为NULL
			it->del = NULL;
		} else {
			// 不需要重置head的情况，也就是不需要删除head节点
			// （注意，全用it->del的话能精简代码）
			// 改变要del的前节点的next指向
			it->del->prev->next = it->del->next;
			// 判断del的next是否存在 
			if(it->del->next != NULL){
				// 存在的话，改变del的后节点的prev指向
				it->del->next->prev = it->del->prev;
			}
			// 释放del指向的节点 
			free(it->del);
			// 然后令del指向NULL，因为 deleteElm 并不是3个函数之一
			// 更深层的意思就是，让del不再指向原来的地址，而是令为NULL 
			it->del = NULL;
		}
		
		// 在if-else之后返回1，代表成功了，因为进入if(it->del != NULL)中就代表成功了
		return 1;
	}
	
	// 如果上面if条件里面的条件不满足，也就是 it->del == NULL，并且没有return 1 
	// 那么就说明失败了，直接返回0即可（此时不需要重置del，因为del本身就是NULL） 
	return 0;
}


// 替换元素（我觉得用replace代表这个函数名更直观一些，搞不懂老师为什么要叫它set） 
int set(IteratorInt it, int v){
	
	printf("  set --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return 0;
	}
	
	// 必须成功调用4个函数之一，也就是del不为NULL 
	// 四个函数中令 it->del = NULL 的操作就是为了如下这个判断条件 
	// 当del这个指针有所指向的时候才进行替换操作 
	if(it->del != NULL){
		// 直接改值 
		it->del->data = v;
		// 然后将del置为NULL，因为这个方程不是4个函数之一
		it->del = NULL;
		// 最后返回1，代表成功了 
		return 1;
	}
	
	// 如果上面if条件里面的条件不满足，也就是 it->del == NULL，并且没有return 1 
	// 那么就说明失败了，直接返回0即可（此时不需要重置del，因为del本身就是NULL） 
	return 0;
}


// 从光标位置开始在链表中找有没有节点的值为v的
// 找到了的话就移动光标，并返回v；否则返回NULL 
int *findNext(IteratorInt it, int v){
	
	printf("  findNext --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return NULL;
	}
	
	// 没有下一个 
	if(!hasNext(it)){
		// 因为失败了，del需要置为NULL
		it->del = NULL;
		return NULL;
	}
	
	// 循环找出节点
	Node *the_node = it->cursor_next;
	while(the_node != NULL){
		// 从 it->cursor_next 开始查找
		// 找到了的话 
		if(the_node->data == v){
			// 重置前后光标 
			it->cursor_prev = the_node;
			it->cursor_next = the_node->next;
			// 并且重置del指针，也就是令del指向the_node的那个节点
			it->del = the_node;
			// 并且return找到的这个值
			// 因为是指针函数，需要是指针类型的返回值，
			// （如果要接收这个函数的返回值的话，需要用同样类型的指针接收） 
			// 所以这里用指针指向数据的地址来暂存data 
			int *p = &(the_node->data);
			return (p);
		}
		// the_node 向下一个的方向移动 
		the_node = the_node->next;
	}
	
	// 没有找到对应的值，那么就返回NULL
	// 但是在返回之前，因为失败了，del需要置为NULL
	it->del = NULL;
	return NULL;
}


// 从光标位置开始在链表中找有没有节点的值为v的
// 找到了的话就移动光标，并返回v；否则返回NULL 
// 因为这是双向链表，所以 findNext 和 findPrevious 的写法是差不多的 
int *findPrevious(IteratorInt it, int v){
	
	printf("  findPrevious --- ");
	
	// 首先，防止不存在链表这个超特殊情况
	if(!it){
		return NULL;
	}
	
	// 没有上一个 
	if(!hasPrevious(it)){
		// 因为失败了，del需要置为NULL
		it->del = NULL;
		return NULL;
	}
	
	// （这里跟上面的不同） 
	Node *the_node = it->cursor_prev;
	while(the_node != NULL){
		// 从 it->cursor_prev 开始查找
		// 找到了的话 
		if(the_node->data == v){
			// 重置前后光标（这里跟上面的不同） 
			it->cursor_next = the_node;
			it->cursor_prev = the_node->prev;
			// 并且重置del指针，也就是令del指向the_node的那个节点
			it->del = the_node;
			// 并且return找到的这个值
			// 因为是指针函数，需要是指针类型的返回值，
			// （如果要接收这个函数的返回值的话，需要用同样类型的指针接收） 
			// 所以这里用指针指向数据的地址来暂存data 
			int *p = &(the_node->data);
			return (p);
		}
		// the_node 向上一个的方向移动（这里跟上面的不同） 
		the_node = the_node->prev;
	}
	
	// 没有找到对应的值，那么就返回NULL
	// 但是在返回之前，因为失败了，del需要置为NULL
	it->del = NULL;
	return NULL;
}


// 看哪个东西用了malloc这类向计算机要内存的函数就free哪个 
// 看上面的，链表用了malloc，链表中的节点也用了malloc，所以需要释放这两个
// 首先就要释放链表中的节点占用的内存，然后释放链表占用的内存 
void freeIt(IteratorInt it){
	// 首先呢，得考虑链表不存在这种超特殊情况 
	if(!it){
		// 直接结束 
		return;
	}
	
	// 随便定义两个指针节点，一个指向当前节点，一个指向当前节点的上一个节点
	// 需要释放的节点是 temp 指向的节点 
	Node *current, *temp;
	current = it->head;
	// 用while循环好理解一些
	while(current != NULL){
		// temp临时存放，每次自动重置为current 
		temp = current;
		// 将current后移 
		current = current->next;
		// 释放current前面那个节点 
		free(temp);
	}
	
	// 释放完链表中的节点后，再释放链表占用的内存
	free(it);
	
	// 这里return仅仅代表结束 
	return;
}
// ==================================================================================



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// 最终答案需要删除下面的包括主函数的所有函数
// 并且还要删除上面每个函数中的printf部分 

/***********************************************************************************/
// 检查正误函数 
void cheak_result(int correct_answer, int the_output){
	if(correct_answer == the_output){
		printf("√\n");
	} else {
		printf("×××\n");
		printf(" --- should return: %d\n", correct_answer);
		printf(" --- but output is: %d\n", the_output);
		// 出错了，就使程序正常退出，不继续了 
		printf(" ============ program termination ============ \n");
		exit(0);
	}
}


// 检查指针是否为空 
void cheak_pointer(int *pointer){
	if(pointer == NULL){
		printf("√\n");
	} else {
		printf("×××\n");
		printf("pointer is not NULL!\n");
	}
}


// 打印链表函数 
void showDLL(IteratorInt it){
	printf("\n --- showDDL ---\n");
	Node *curr;
	for(curr=it->head; curr!=NULL; curr=curr->next){
		printf("%d", curr->data);
		if(curr->next != NULL){
			printf(" -> ");
		}
		if(curr == it->cursor_prev){
			printf("^ -> ");
		}
	}
	printf("\n");
}
/***********************************************************************************/ 



/***********************************************************************************/ 
// test case(sample_test_log1 && test_0-9):

void sample_test_log1(){
	
	printf("\n ------------ sample_test_log1 ------------ \n");
	
	// set up a new doubly linked list which named it 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	
	integer = add(it, 20);
	cheak_result(1, integer);
	
	integer = add(it, 12);
	cheak_result(1, integer);
	
	integer = add(it, 33);
	cheak_result(1, integer);
	
	integer = add(it, 25);
	cheak_result(1, integer);
	
	pointer = previous(it);
	// 因为 previous 函数中引用了 hasPrevious 这个函数，
	// 所以打印出来会是 " previous ---   hasPrevious --- " 这样子
	// 但是是对的，因为程序没有退出，后面也打印了一个 √
	cheak_result(25, *pointer);
	
	pointer = previous(it);
	cheak_result(33, *pointer);
	
	pointer = next(it);
	// 因为 next 函数中引用了 hasNext 这个函数，
	// 所以打印出来会是 " next ---   hasNext --- " 这样子
	// 但是是对的，因为程序没有退出，后面也打印了一个 √
	cheak_result(33, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	pointer = previous(it);
	cheak_result(12, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	pointer = previous(it);
	cheak_result(20, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	pointer = next(it);
	cheak_result(25, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	integer = add(it, 55);
	cheak_result(1, integer);
	
	integer = add(it, 29);
	cheak_result(1, integer);
	
	// reset
	reset(it);
	
	pointer = next(it);
	cheak_result(55, *pointer);
	
	integer = add(it,62);
	cheak_result(1, integer);
	
	integer = add(it,44);
	cheak_result(1, integer);
	
	pointer = previous(it);
	cheak_result(44, *pointer);
	
	pointer = next(it);
	cheak_result(44, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	pointer = previous(it);
	cheak_result(62, *pointer);
	
	pointer = next(it);
	cheak_result(62, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	integer = deleteElm(it);
	cheak_result(0, integer);
	
	pointer = next(it);
	cheak_result(29, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	pointer = previous(it);
	cheak_result(55, *pointer);
	
	integer = deleteElm(it);
	cheak_result(1, integer);
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// free memory allocated by malloc
	freeIt(it);
	
	/*
	  其实只要通过老师给的这个 sample_test_log1 就可以拿较高的分数了 
	  因为老师最终测试的案例也不是很变态，只是有较为特殊的情况
	   
	  下面补充的测试主要是测 sample_test_log1 里没有测过的函数
	  例如 findPrevious，hasNext 这类函数 
	  然后还测了一些比较特殊，特殊和极其特殊的情况 
	  如果下面的测试全部通过，那么8分肯定稳了
	  （满分10分，其中2分是格式备注风格分，8分是答案全部正确的得分）
	   
	  有兴趣的话还可以测试一下内存是否存在泄漏
	  老师并没有讲过内存泄漏（只讲过free函数），也没有测内存是否泄漏
	*/
	
}


void test_0(){
	
	printf("\n\n ------------ test_0 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[9] = {20, 12, 15, 5, 14, 10, 5, 9, 3};
	
	int i;
	for(i=0; i<9; i++){
		integer = add(it, test_array[i]);
		cheak_result(1, integer);
		
		// 判断hasNext的操作
		integer = hasNext(it);
		cheak_result(0, integer);
	
		// 判断hasPrevious的操作
		integer = hasPrevious(it);
		cheak_result(1, integer);
	
		pointer = next(it);
		// 这里就是单纯地判断是否为NULL
		//////////////////////////////// 
		cheak_pointer(pointer);
		////////////////////////////////
		
		// 判断 previous
		pointer = previous(it);
		cheak_result(test_array[i], *pointer);
		
		// 判断 next
		pointer = next(it);
		cheak_result(test_array[i], *pointer);
	    
		// （向前移动又向后移动，等于没动）
	}
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存
	freeIt(it);
}


// test_1是看极其特殊的情况 
void test_1(){
	
	printf("\n\n ------------ test_1 ------------ \n");
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// 这里不新建链表，直接令链表为NULL
	// 空链表意味着所有的函数的返回的都是"失败"
	IteratorInt it = NULL; 
	
	int integer;
	int *pointer;
	
	// 判断添加
    integer = add(it, 1);
    cheak_result(0, integer);
	
    // 判断没有next的操作
    integer = hasNext(it);
    cheak_result(0, integer);
	
    // 判断是否存在Previous的数据
    integer = hasPrevious(it);
    cheak_result(0, integer);
	
    pointer = next(it);
    cheak_pointer(pointer);
	
    pointer = previous(it);
    cheak_pointer(pointer);
	
    pointer = next(it);
    cheak_pointer(pointer);
	
    pointer = findPrevious(it, 1);
    cheak_pointer(pointer);
	
    pointer = findNext(it, 1);
    cheak_pointer(pointer);
	
    integer = deleteElm(it);
    cheak_result(0, integer);
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


//// test_2就是 sample_test_log1
//void test_2(){
//	
//	printf("\n\n ------------ test_2 ------------ \n");
//	
//	// 新建链表 
//	IteratorInt it = IteratorIntNew();
//	
//	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	int integer;
//	int *pointer;
//	 
//	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	
//	// 释放内存 
//	freeIt(it);
//}


void test_3(){
	
	printf("\n\n ------------ test_3 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	
    /*
    add 20, returns 1
    ............................
    20   ^
    ............................
    */
    integer = add(it, 20);
    cheak_result(1, integer);
    
	/*
    add 12, returns 1
    ............................
    20   12   ^
    ............................
    */
    integer = add(it, 12);
    cheak_result(1, integer);

    integer = hasNext(it);
    cheak_result(0, integer);
	
	/*
    previous, returns 12
    ............................
    20   ^   12
    ............................
    */
    pointer = previous(it);
    cheak_result(12, *pointer);
	
	/*
    hasNext, returns 1
    ............................
    20   ^   12
    ............................
    */
    integer = hasNext(it);
    cheak_result(1, integer);
    
	/*
    hasPrevious, returns 1
    ............................
    20   ^   12
    ............................
    */
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
	/*
    previous, returns 20
    ............................
    ^   20   12
    ............................
    */
    pointer = previous(it);
    cheak_result(20, *pointer);
	
	/*
    hasNext, returns 1
    ............................
    ^   20   12
    ............................
    */
    integer = hasNext(it);
    cheak_result(1, integer);
    
    /*
    hasPrevious, returns 0
    ............................
    ^   20   12
    ............................
    */
    integer = hasPrevious(it);
    cheak_result(0, integer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


void test_4(){
	
	printf("\n\n ------------ test_4 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[9] = {20, 12, 15, 5, 14, 10, 5, 9, 3};
    
	int i;
    for (i=0; i<9; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
    // 重置光标
    reset(it);
	
    integer = hasNext(it);
    cheak_result(1, integer);
	
    integer = add(it, 20);
    cheak_result(1, integer);
	
    pointer = next(it);
    cheak_result(20, *pointer);
	
    pointer = findNext(it, 5);
    cheak_result(5, *pointer);
	
    pointer = next(it);
    cheak_result(14, *pointer);
	
    pointer = previous(it);
    cheak_result(14, *pointer);
    
    pointer = previous(it);
    cheak_result(5, *pointer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


void test_5(){
	
	printf("\n\n ------------ test_5 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[9] = {20, 12, 15, 5, 14, 10, 5, 9, 3};
	
	int i;
    for(i=0; i<9; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
	/*
    hasNext, returns 0
    ............................
    20   12   15   5   14   10   5   9   3   ^
    ............................
    */
    integer = hasNext(it);
    cheak_result(0, integer);
	
	/*
    next, returns NULL
    ............................
    20   12   15   5   14   10   5   9   3   ^
    ............................
    */
    pointer = next(it);
    cheak_pointer(pointer);
	
	/*
    hasPrevious, returns 1
    ............................
    20   12   15   5   14   10   5   9   3   ^
    ............................
    */
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
	/*
    findPrevious 5, returns 5
    ............................
    20   12   15   5   14   10   ^   5   9   3
    ............................
    */
    pointer = findPrevious(it, 5);
    cheak_result(5, *pointer);
	
	/*
    previous, returns 10
    ............................
    20   12   15   5   14   ^   10   5   9   3
    ............................
    */
    pointer = previous(it);
    cheak_result(10, *pointer);
    
	/*
    findPrevious 5, returns 5
    ............................
    20   12   15   ^   5   14   10   5   9   3
    ............................
    */
    pointer = findPrevious(it, 5);
    cheak_result(5, *pointer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


void test_6(){
	
	printf("\n\n ------------ test_6 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	int i;
    for(i=0; i<10; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }

    // 1 2 3 4 5 6 7 8 9 10 ^
    integer = hasNext(it);
    cheak_result(0, integer);
	
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
    pointer = next(it);
    cheak_pointer(pointer);

    for(i=9; i>=0; i--){
        integer = hasPrevious(it);
        cheak_result(1, integer);

        pointer = previous(it);
        cheak_result(test_array[i], *pointer);
    }
    // ^ 1 2 3 4 5 6 7 8 9 10

    integer = hasPrevious(it);
    cheak_result(0, integer);

    integer = hasNext(it);
    cheak_result(1, integer);
	
    for(i=0; i<10; i++){
		integer = hasNext(it);
        cheak_result(1, integer);

        pointer = next(it);
        cheak_result(test_array[i], *pointer);
	}
	
    // 1 2 3 4 5 6 7 8 9 10 ^
    // 之前的next的内容需要保留，因此还是原来的next查询的结果：10
    pointer = findNext(it, 2);
    cheak_pointer(pointer);

    pointer = findPrevious(it, 2);
    // 1 ^ 2 3 4 5 6 7 8 9 10
    cheak_result(2, *pointer);
	
    pointer = findNext(it, 10);
    // 1 2 3 4 5 6 7 8 9 10 ^
    cheak_result(10, *pointer);
	
    pointer = findNext(it, 10);
    cheak_pointer(pointer);
    
    pointer = findNext(it, 1000);
    cheak_pointer(pointer);

    pointer = findPrevious(it, 1);
    // ^ 1 2 3 4 5 6 7 8 9 10
    cheak_result(1, *pointer);
    
    pointer = findPrevious(it, 1);
    cheak_pointer(pointer);
    
    pointer = findPrevious(it, 1000);
    cheak_pointer(pointer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


// 主要检查连续set，也就是连续进行替换操作 
void test_7(){
	
	printf("\n\n ------------ test_7 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[9] = {20, 12, 15, 5, 14, 10, 5, 9, 3};
	
	int i;
    for(i=0; i<9; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
    // 20 12 15 5 14 10 5 9 3 ^
    // set
    integer = set(it, 1000);
    cheak_result(0, integer);
	
    // check has previous
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
    // 20 12 15 5 14 10 5 9 ^ 3
    pointer = previous(it);
    cheak_result(3, *pointer);
    
    // set
    // 20 12 15 5 14 10 5 9 ^ 1000
    integer = set(it, 1000);
    cheak_result(1, integer);
	
    // set
    integer = set(it, 1000);
    cheak_result(0, integer);
	
    // check has previous
    integer = hasNext(it);
    cheak_result(1, integer);
	
    pointer = next(it);
    // 20 12 15 5 14 10 5 9 1000 ^
    cheak_result(1000, *pointer);
    
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
    integer = set(it, 1001);
    cheak_result(0, integer);

    // ^ 20 12 15 5 14 10 5 9 1000
    reset(it);
	
    integer = set(it, 1001);
    cheak_result(0, integer);
	
    pointer = next(it);
    // 20 ^ 12 15 5 14 10 5 9 1000
    cheak_result(20, *pointer);
    
    integer = set(it, 1001);
    // 1001 ^ 12 15 5 14 10 5 9 1000
    cheak_result(1, integer);

    pointer = previous(it);
    // ^ 1001 12 15 5 14 10 5 9 1000
    cheak_result(1001, *pointer);
    
    pointer = findNext(it, 15);
    // 1001 12 15 ^ 5 14 10 5 9 1000
    cheak_result(15, *pointer);
    
    integer = set(it, 1100);
    // 1001 12 1100 ^ 5 14 10 5 9 1000
    cheak_result(1, integer);

    pointer = previous(it);
    // 1001 12 ^ 1100 5 14 10 5 9 1000
    cheak_result(1100, *pointer);
    
    pointer = findPrevious(it, 12);
    // 1001 ^ 12 1100 5 14 10 5 9 1000
    cheak_result(12, *pointer);
    
    integer = set(it, 1200);
    // 1001 ^ 1200 1100 5 14 10 5 9 1000
    cheak_result(1, integer);
	
    integer = set(it, 1500);
    // 1001 ^ 1200 1100 5 14 10 5 9 1000
    cheak_result(0, integer);
	
    pointer = findNext(it, 1000);
    // 1001 1200 1100 5 14 10 5 9 1000 ^
    cheak_result(1000, *pointer);
    
    pointer = findPrevious(it, 5000);
    // 1001 1200 1100 5 14 10 5 9 1000 ^
    // 因为没有找到，所以原来的位置不变，返回应为NULL 
    cheak_pointer(pointer);
	
    integer = set(it, 1500);
    // 1001 1200 1100 5 14 10 5 9 1000 ^
    cheak_result(0, integer);
	
    pointer = previous(it);
    // 1001 1200 1100 5 14 10 5 9 ^ 1000
    cheak_result(1000, *pointer);
		
    integer = add(it, 10000);
    // 1001 1200 1100 5 14 10 5 9 10000 ^ 1000
    cheak_result(1, integer);
	
    integer = set(it, 20000);
    // 1001 1200 1100 5 14 10 5 9 10000 ^ 1000
    cheak_result(0, integer);
	
    pointer = next(it);
    // 1001 1200 1100 5 14 10 5 9 10000 1000 ^
    cheak_result(1000, *pointer);
		
    pointer = next(it);
    // 1001 1200 1100 5 14 10 5 9 10000 1000 ^
    cheak_pointer(pointer);
	
    integer = set(it, 200);
    // 1001 1200 1100 5 14 10 5 9 1000 ^
    cheak_result(0, integer);
	
	// 重置光标 
    reset(it);
    // ^ 1001 1200 1100 5 14 10 5 9 1000
	
    pointer = next(it);
    // 1001 ^ 1200 1100 5 14 10 5 9 1000
    cheak_result(1001, *pointer);
		
    pointer = previous(it);
    // ^ 1001 1200 1100 5 14 10 5 9 1000
    cheak_result(1001, *pointer);
		
    pointer = previous(it);
    // ^ 1001 1200 1100 5 14 10 5 9 1000
    cheak_pointer(pointer);
	
    integer = set(it, 300);
    // ^ 1001 1200 1100 5 14 10 5 9 1000
    cheak_result(0, integer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


// 这个测试混合测deleteElm，set，hasNext，hasPrevious，hasNext，hasPrevious 
void test_8(){
	
	printf("\n\n ------------ test_8 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	int i;
    for(i=0; i<10; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
    // 1 2 3 4 5 6 7 8 9 10 ^
    integer = deleteElm(it);
    cheak_result(0, integer);
	
    // 1 2 3 4 5 6 7 8 9 10 ^
    integer = hasNext(it);
    cheak_result(0, integer);
	
    integer = hasPrevious(it);
    cheak_result(1, integer);
	
    pointer = previous(it);
    // 1 2 3 4 5 6 7 8 9 ^ 10
    cheak_result(10, *pointer);
    
    integer = set(it, 1000);
    // 1 2 3 4 5 6 7 8 9 ^ 1000
    cheak_result(1, integer);
	
    integer = deleteElm(it);
    cheak_result(0, integer);
	
    pointer = next(it);
    // 1 2 3 4 5 6 7 8 9 1000 ^
    cheak_result(1000, *pointer);
    
    integer = deleteElm(it);
    // 1 2 3 4 5 6 7 8 9 ^
    cheak_result(1, integer);
	
    integer = deleteElm(it);
    cheak_result(0, integer);
	
    pointer = previous(it);
    // 1 2 3 4 5 6 7 8 ^ 9
    cheak_result(9, *pointer);
		
    integer = deleteElm(it);
    // 1 2 3 4 5 6 7 8 ^
    cheak_result(1, integer);
	
    integer = set(it, 2000);
    // 1 2 3 4 5 6 7 8 ^
    cheak_result(0, integer);
	
    pointer = findPrevious(it, 5);
    // 1 2 3 4 ^ 5 6 7 8
    cheak_result(5, *pointer);
		
    pointer = findPrevious(it, 5);
    // 1 2 3 4 ^ 5 6 7 8
    cheak_pointer(pointer);
	
    integer = deleteElm(it);
    // 1 2 3 4 ^ 5 6 7 8
    cheak_result(0, integer);
	
    pointer = findNext(it, 5);
    // 1 2 3 4 5 ^ 6 7 8
    cheak_result(5, *pointer);
	
    pointer = findNext(it, 5);
    // 1 2 3 4 5 ^ 6 7 8
    cheak_pointer(pointer);
	
    integer = deleteElm(it);
    // 1 2 3 4 5 ^ 6 7 8
    cheak_result(0, integer);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}


// 最后一个测试测的是非常特殊的情况 
void test_9(){
	
	printf("\n\n ------------ test_9 ------------ \n");
	
	// 新建链表 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i;
	
	// 第一次 free
    freeIt(it);
	
	// 然后令it为NULL，第一次 
	// 因为链表为NULL，所以调用任何函数都应为"失败" 
    it = NULL;
	
	// 第一次重置光标 
    reset(it);
    
    integer = hasNext(it);
    cheak_result(0, integer);
	
    integer = hasPrevious(it);
    cheak_result(0, integer);
    
    pointer = next(it);
    cheak_pointer(pointer);
	
    pointer = previous(it);
    cheak_pointer(pointer);
	
    pointer = findNext(it, 1000);
    cheak_pointer(pointer);
	
    pointer = findPrevious(it,1000);
    cheak_pointer(pointer);
	
    integer = deleteElm(it);
    cheak_result(0, integer);
	
    integer = set(it, 1);
    cheak_result(0, integer);
	
    // add
    integer = add(it, 1);
    cheak_result(0, integer);
	
    // 第二次 free
    freeIt(it);
	
	// 第二次令it为NULL 
    it = NULL;
	
	// 第二次重置光标
    reset(it);
	
    // 第二次新建链表 
    it = IteratorIntNew();
	
    for(i=0; i<10; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
	// 第三次重置光标
    reset(it);
	
    // 第三次 free
    freeIt(it);
	
	// 第三次令it为NULL 
    it = NULL;
	
	// 第四次重置光标
    reset(it);
	
	// 下面所有的操作应该都为"失败" 
    integer = hasNext(it);
    cheak_result(0, integer);
	
    integer = hasPrevious(it);
    cheak_result(0, integer);
	
    pointer = next(it);
    cheak_pointer(pointer);
	
    pointer = previous(it);
    cheak_pointer(pointer);
	
    pointer = previous(it);
    cheak_pointer(pointer);
	
    pointer = findNext(it, 1000);
    cheak_pointer(pointer);
	
    pointer = findPrevious(it,1000);
    cheak_pointer(pointer);
	
    integer = deleteElm(it);
    cheak_result(0, integer);
	
    integer = set(it,1);
    cheak_result(0, integer);
	
    // add
    integer = add(it, 1);
    cheak_result(0, integer);
	
	// 第五次重置光标
    reset(it);
	
	// 第四次 free 
    freeIt(it);
	
	// 第四次令it为NULL 
    it = NULL;
	
	// 第五次重置光标  
    reset(it);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// 释放内存 
	freeIt(it);
}
/***********************************************************************************/ 



/***********************************************************************************/
// 主函数 
int main(){
	
	// 老师给的sample_test 
	sample_test_log1();
	
	// 大佬写的测试案例（我自行修改了输出，就不用cheak.h了）
	test_0();
	test_1();
//	test_2();	// 第二个测试和老师给的 sample_test_log1 一摸一样，所以忽略 
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();
	
	printf("\n ------------ pass all tests! ------------ \n"); 
	
	return 0;
}
/***********************************************************************************/ 

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



