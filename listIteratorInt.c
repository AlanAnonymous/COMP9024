/*
  listIteratorInt.c : list Iterator ADT implementation
  Written by Alan
  Date: 2018-2019
*/

// �� 
#include <stdlib.h>		// malloc
#include <stdio.h>
#include <assert.h>		// assert(�Ҿ��ÿ�д�ɲ�д)
#include "listIteratorInt.h"

/*
  �ѵ���ʵ����add��delete��������������Ϊ����������Ƚ϶�
  ����������д����Ψһ���ҵ�д���Ƚ�������Ҳ���ǱȽ�ɵ��ʽ :D 
*/


// ----------------------------------------------------------------------------------
// �ڵ㣨����̶�д���� 
typedef struct Node {
	// ǰ 
	struct Node *prev;
	// �� 
	struct Node *next;
	// ���� 
	int data;
} Node;

// ������/��װ������������ӽڵ㣬ָ������ݣ�
// ����������ô������ô�����������������ô��
typedef struct IteratorIntRep {
	// ͷ������ģ������������ʼ�ڵ����ֹ�ڵ�(��ֹһ�����NULL)�� 
	Node *head;
	// �����������
	// ǰ��� 
	Node *cursor_prev;
	// ���� 
	Node *cursor_next;
	/*
	  ����������˼��ǰ���ָ�� ^ ǰ���Ǹ�������ָ�� ^ �����Ǹ�
	    ��1�� 1 2 ^ 3 4
	          cursor_prev ָ�� 2
			  cursor_next ָ�� 3
			 
	    ��2�� ^ 5 6 7
	          cursor_prev ָ�� NULL
			  cursor_next ָ�� 5
			 
	    ��3�� 8 9 ^
	          cursor_prev ָ�� 9
			  cursor_next ָ�� NULL
	*/
	
	// Ȼ��delete����Ҫ������һ��������ɾ��Ԫ�أ������ٶ���һ��ָ�룬ָ��Ҫɾ����Ԫ��
	// ���Ҹ������⣬del���ָ��ֻ����next��previous��findNext��findPrevious�в���ָ��
	// ������������Ӧ������ΪNULL 
	// ���仰˵�����Ҫɾ���Ľڵ�del����ΪNULLʱ˵���ɹ���ΪNULL����ʧ�� 
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
// �½�˫������ 
IteratorInt IteratorIntNew(){
	// ��˫������doubly_linked_list����ռ�
	struct IteratorIntRep *doubly_linked_list = malloc(sizeof(struct IteratorIntRep));
	// doubly_linked_list��ΪNULL�ż��� 
	assert(doubly_linked_list != NULL);
	// ��ʼ���������е�ָ�� 
	// ��˫������doubly_linked_list��ͷ��ΪNULL 
	doubly_linked_list -> head = NULL;
	// ������ʼ�� 
	doubly_linked_list -> cursor_prev = NULL;
	doubly_linked_list -> cursor_next = NULL;
	// ��del��ʼ�� 
	doubly_linked_list->del = NULL;
	// ���ã����ش�˫������ 
	return doubly_linked_list;
}


// ���ù�굽˫������Ŀ�ͷ 
/*
  Ҳ����ҪŪ�����֣� ^  head  ��ǰ���ָ��NULL������ָ��head�� 
*/ 
void reset(IteratorInt it){
	// ���������ڲ��ܽ������²���
	// �ж�������Ҫ�Ƿ�ֹ�������ڻ���ΪNULL���ֳ�������� 
	if(it){
		// ǰ���
		it->cursor_prev = NULL;
		// ���� 
		it->cursor_next = it->head;
		
		// ������4�����̾���delΪNULL 
		it->del = NULL;
	}
}


// ��������Ӻ������½��ڵ㺯�� 
// ��Ҫ��"listIteratorInt.h"������������������ 
Node *newNode(int v){
	// ���½ڵ����ռ� 
	Node *new_node = malloc(sizeof(struct Node));
	// assert һ�� 
	assert(new_node != NULL);
	// �ڵ���������� 
	new_node->data = v;
	// �ڵ��ǰ�ͺ� 
	new_node->prev = NULL;
	new_node->next = NULL;
	// �����ú󣬷�������ڵ�
	return new_node;
}


// ��˫�����������Ԫ�� 
// ������Ϊ�գ���ô���Ԫ�������� cursor_prev λ�� 
// Ҳ����˵��new_node��ӵ���cursor_prevλ�ã�cursor_prev����һλ 
/*
  ���Ԫ�ع�3�ֲ�ͬ�����(�ڶ��������ʵ���Թ���Ϊ1�����)
    1.    NULL
    2.1   head  ^
    2.2   head  node_1  ^
    2.3   head  ^  node_1
    2.4   node_1  ^  node_2
    3.    ^  head
  
  ��ʵҲ������3�������
  (��Ϊ���Ԫ��������cursor_prevλ�ã����Բ���Ҫ����cursor_next��λ��)
    1. head==NULL && cursor_prev==NULL
	2. head!=NULL && cursor_prev!=NULL
	3. head!=NULL && cursor_prev==NULL
*/
int add(IteratorInt it, int v){
	
	printf("  add --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		// ��Ϊ�������ڣ���ôit->XX����д���ǷǷ��� 
		return 0;
	}
	
	// �½��ڵ� 
	Node *new_node = newNode(v);
	
	// �����û��return 0�Ļ����ǿ϶��ǳɹ��ˣ�return 1�� 
	 
	// 3�������һ��д��
	// ֱ��д��"if��else if��else if"���ö�������������һ�� 
	// �������1��Ҳ��������Ϊ�յ���� 
	// ��ʵ it->cursor_prev==NULL û�б�Ҫд����Ϊͷ�ڵ�ΪNULL��������û�нڵ㣬
	// ��ô cursor_prev �϶�Ϊ NULL 
	if(it->head==NULL && it->cursor_prev==NULL){
		// �൱�ڳ�ʼ��ͷ�ڵ� 
		it->head = new_node;
	}
	// �������2������Ϊ�գ�cursor_prev��ΪNULL 
	else if(it->head!=NULL && it->cursor_prev!=NULL){
		// 2.4(��2.3)��һ����������Կ����ȿ�2.4��Ȼ��������Ϊ��������
		// ��󾫼������У�Ҳ���Ǻ���һ��д
		// ��2.1��2.2��2.3��2.4����һ��д��д�����£� 
		// ���ȣ��ı� new_node ��ǰ�ͺ� 
		new_node->prev = it->cursor_prev;
		new_node->next = it->cursor_prev->next;
		// Ȼ���ж� 
		if(new_node->prev != NULL){
			// ��Ϊ�ղ��ܼ�������ΪNULL��û��next����prev 
			// д�� new_node->prev->next ��һ�㣬�����׳���Ҳ�����һ�� 
			// ��Ҳ����˵��д�� it->cursor_prev->next Ҳ�У� 
			new_node->prev->next = new_node;
		}
		if(new_node->next != NULL){
			new_node->next->prev = new_node;
		}
	}
	// �������3������Ϊ�գ�cursor_prevΪNULL 
	// Ҳ���ǹ����head֮ǰ�����Բ���ڵ�Ļ�����Ҫ�嵽head֮ǰ 
	// �����������Ҫ����head 
	else if(it->head!=NULL && it->cursor_prev==NULL){
		// �������ڵ����� 
		it->head->prev = new_node;
		new_node->next = it->head;
		// ����head
		it->head = new_node;
	}
	
	// ����֮��cursor_prev��Ҫ�����ã�Ҳ����ָ���²�����Ǹ��ڵ� 
	// ��cursor_next������Ҫ���� 
	// �����������Ǽ򻯺��д��
	// ��Ϊÿ���ж������ж�Ҫд�����Կ���ֱ�������д���� 
	it->cursor_prev = new_node;
	
	// ������4�����̾���delΪNULL 
	it->del = NULL;
	
	// �ɹ������Է���1 
	return 1;
}


// ��˫�������еĹ�������û����һ�� 
/*
  һ��2�����������һ�ַǳ����⣬�ڶ��ֱȽ����⣬���������ձ������ 
    1.  �������� 
	2.1 ������û�нڵ㣨Ҳ����û��ͷ�ڵ㣩 
	2.2 ����������ĩβ 
*/ 
int hasNext(IteratorInt it){
	
	printf("  hasNext --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		// ��Ϊ�������ڣ���ôit->XX����д���ǷǷ��� 
		return 0;
	}
	
	// Ȼ�����������Ǵ��ڵ� 
	// ��ʱ��ͷ�ڵ����ΪNULL��ǰ������ΪNULL���������ΪNULL
	// ��һ�������
	// "������û�нڵ㣨Ҳ����û��ͷ�ڵ㣩"��"��� ^ ������ĩβ"���Ժ���һ��д 
	// ��������ֻ��Ҫ��������� 
	if(it->cursor_next == NULL){
		// �����ڷ���0֮ǰ��Ҫ��delΪNULL����Ϊ���ﲢû�гɹ�����hasNext���� 
		it->del = NULL;
		return 0;
	}
	 
	// �����������䶼û��return 0����ô��˵��˫������Ĺ�����϶�����һ��
	// ����Ӧ�÷���1�������ڷ���1֮ǰ��Ҫ��del���� 
	// ������4�����̾���delΪNULL 
	it->del = NULL;
	return 1;
}


// ��˫�������еĹ��ǰ����û����һ��
/*
  ����Ļ�������2�������
    1.  ��������
	2.1 ������û�нڵ㣨Ҳ����û��ͷ�ڵ㣩
	2.2 ���������Ŀ�ͷ
*/ 
int hasPrevious(IteratorInt it){
	
	printf("  hasPrevious --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return 0;
	}
	
	// Ȼ���������
	// ��ʱ��ͷ�ڵ����ΪNULL��ǰ������ΪNULL���������ΪNULL
	// ��һ�������
	// "������û�нڵ㣨Ҳ����û��ͷ�ڵ㣩"��"��� ^ ������ͷ"���Ժ���һ��д 
	// ��������ֻ��Ҫ��ǰ������ 
	if(it->cursor_prev == NULL){
		// �����ڷ���0֮ǰ��Ҫ��delΪNULL����Ϊ���ﲢû�гɹ�����hasPrevious���� 
		it->del = NULL;
		return 0;
	}
	
	// �����������䶼û��return 0����ô��˵��˫������Ĺ��ǰ��϶�����һ��
	// ����Ӧ�÷���1�������ڷ���1֮ǰ��Ҫ��del���� 
	// ������4�����̾���delΪNULL 
	it->del = NULL;
	return 1;
}


// ָ�뺯�� 
// ʹ�������һ�������ƶ�
int *next(IteratorInt it){
	
	printf("  next --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return NULL;
	}
	
	// Ȼ�󣬿���û����һ����ֱ����hasNext�����жϾ��� 
	if(hasNext(it)){
		// �еĻ����ȸı�delָ��ָ�� 
		it->del = it->cursor_next;
		// Ȼ���ƶ���꣨ǰ/���꣩
		it->cursor_prev = it->cursor_next;
		it->cursor_next = it->cursor_next->next;
		// ��󷵻�Ҫɾ����ֵ
		// ����ע�������д������Ϊ��ָ�뺯�������Բ���ֱ�ӷ���(it->del->data)
		// ��Ϊ��ָ�뺯������Ҫ��ָ�����͵ķ���ֵ��
		// �����Ҫ������������ķ���ֵ�Ļ�����Ҫ��ͬ�����͵�ָ����գ� 
		// ����������ָ��ָ�����ݵĵ�ַ���ݴ�data 
		int *p = &(it->del->data);
		return (p);
	}
	
	// ��������if�����ж�Ϊfalse��Ҳ����û�н���if������ִ�г���
	// ��ô��˵��ʧ���ˣ����������⣬����NULL 
	// ��Ȼд�ɣ� else{return NULL;} Ҳ�� 
	// �����ڷ���NULL֮ǰ��Ҫ��delΪNULL����Ϊ���ﲢû�гɹ�����next���� 
	it->del = NULL;
	return NULL;
}


// ָ�뺯�� 
// ʹ�������һ�������ƶ�
int *previous(IteratorInt it){
	
	printf("  previous --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return NULL;
	}
	
	// Ȼ�󣬿���û��һ����ֱ����hasPrevious�����жϾ��� 
	if(hasPrevious(it)){
		// �еĻ����ȸı�delָ��ָ��
		it->del = it->cursor_prev;
		// Ȼ���ƶ���꣨ǰ/���꣩
		it->cursor_next = it->cursor_prev;
		it->cursor_prev = it->cursor_prev->prev;
		// ��󷵻�Ҫɾ����ֵ
		// ��Ϊ��ָ�뺯������Ҫ��ָ�����͵ķ���ֵ��
		// �����Ҫ������������ķ���ֵ�Ļ�����Ҫ��ͬ�����͵�ָ����գ� 
		// ����������ָ��ָ�����ݵĵ�ַ���ݴ�data
		int *p = &(it->del->data);
		return (p);
	}
	
	// ��������if�����ж�Ϊfalse��Ҳ����û�н���if������ִ�г���
	// ��ô��˵��ʧ���ˣ����������⣬����NULL 
	// ��Ȼд�ɣ� else{return NULL;} Ҳ��
	// �����ڷ���NULL֮ǰ��Ҫ��delΪNULL����Ϊ���ﲢû�гɹ�����previous����
	it->del = NULL;
	return NULL;
}


// ɾ����괦��Ԫ�أ���ʵ��ɾ��delָ��Ľڵ㣩
/*
  ��2��������ۣ�
    
    1. ͷ�ڵ���Ҫ���� 
      1.1  ^  head(del)  node
      1.2  ^  head(del)
      1.3  head(del)  ^  node
      1.4  head(del)  ^
    
    2. ͷ�ڵ㲻��Ҫ���� 
	  2.1  head  node_1(del)  ^  node_2
	  2.2  head  node_1(del)  ^
	  2.3  head/node  ^  node_1(del)  node_2
	  2.4  head/node  ^  node_1(del)
*/ 
int deleteElm(IteratorInt it){
	
	printf("  deleteElm --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return 0;
	}
	
	// ����ɹ�����4������֮һ��Ҳ����del��ΪNULL 
	// �ĸ��������� it->del = NULL �Ĳ�������Ϊ����������ж����� 
	// ��del���ָ������ָ���ʱ��Ž���ɾ������ 
	// ������ʵ�Զ��ų���headΪNULL���������ΪdelΪNULL����ôhead�϶�ΪNULL 
	if(it->del != NULL){
		// ���ȸı�ǰ�����ָ�򣨷��������ֲ��ù�꣩
		// ��Ϊit->del���ڣ����Զ�it->del������prev��next
		it->cursor_prev = it->del->prev;
		it->cursor_next = it->del->next;
		
		// ��Ҫ����head�������Ҳ������Ҫɾ��head�ڵ� 
		if(it->del == it->head){
			// �ƶ�head�ڵ㣬��head�ڵ����һ���ķ����ƶ� 
			it->head = it->head->next;
			// �ж��µ�head�ڵ��Ƿ�ΪNULL 
			if(it->head != NULL){
				// ����ΪNULL����ı�head�ڵ��prevΪNULL 
				it->head->prev = NULL;
			}
			// �ͷ�delָ��Ľڵ�
			free(it->del);
			// Ȼ����delָ��NULL����Ϊ deleteElm ������3������֮һ
			// ��������˼���ǣ���del����ָ��ԭ���ĵ�ַ��������ΪNULL
			it->del = NULL;
		} else {
			// ����Ҫ����head�������Ҳ���ǲ���Ҫɾ��head�ڵ�
			// ��ע�⣬ȫ��it->del�Ļ��ܾ�����룩
			// �ı�Ҫdel��ǰ�ڵ��nextָ��
			it->del->prev->next = it->del->next;
			// �ж�del��next�Ƿ���� 
			if(it->del->next != NULL){
				// ���ڵĻ����ı�del�ĺ�ڵ��prevָ��
				it->del->next->prev = it->del->prev;
			}
			// �ͷ�delָ��Ľڵ� 
			free(it->del);
			// Ȼ����delָ��NULL����Ϊ deleteElm ������3������֮һ
			// ��������˼���ǣ���del����ָ��ԭ���ĵ�ַ��������ΪNULL 
			it->del = NULL;
		}
		
		// ��if-else֮�󷵻�1������ɹ��ˣ���Ϊ����if(it->del != NULL)�оʹ���ɹ���
		return 1;
	}
	
	// �������if������������������㣬Ҳ���� it->del == NULL������û��return 1 
	// ��ô��˵��ʧ���ˣ�ֱ�ӷ���0���ɣ���ʱ����Ҫ����del����Ϊdel�������NULL�� 
	return 0;
}


// �滻Ԫ�أ��Ҿ�����replace���������������ֱ��һЩ���㲻����ʦΪʲôҪ����set�� 
int set(IteratorInt it, int v){
	
	printf("  set --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return 0;
	}
	
	// ����ɹ�����4������֮һ��Ҳ����del��ΪNULL 
	// �ĸ��������� it->del = NULL �Ĳ�������Ϊ����������ж����� 
	// ��del���ָ������ָ���ʱ��Ž����滻���� 
	if(it->del != NULL){
		// ֱ�Ӹ�ֵ 
		it->del->data = v;
		// Ȼ��del��ΪNULL����Ϊ������̲���4������֮һ
		it->del = NULL;
		// ��󷵻�1������ɹ��� 
		return 1;
	}
	
	// �������if������������������㣬Ҳ���� it->del == NULL������û��return 1 
	// ��ô��˵��ʧ���ˣ�ֱ�ӷ���0���ɣ���ʱ����Ҫ����del����Ϊdel�������NULL�� 
	return 0;
}


// �ӹ��λ�ÿ�ʼ������������û�нڵ��ֵΪv��
// �ҵ��˵Ļ����ƶ���꣬������v�����򷵻�NULL 
int *findNext(IteratorInt it, int v){
	
	printf("  findNext --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return NULL;
	}
	
	// û����һ�� 
	if(!hasNext(it)){
		// ��Ϊʧ���ˣ�del��Ҫ��ΪNULL
		it->del = NULL;
		return NULL;
	}
	
	// ѭ���ҳ��ڵ�
	Node *the_node = it->cursor_next;
	while(the_node != NULL){
		// �� it->cursor_next ��ʼ����
		// �ҵ��˵Ļ� 
		if(the_node->data == v){
			// ����ǰ���� 
			it->cursor_prev = the_node;
			it->cursor_next = the_node->next;
			// ��������delָ�룬Ҳ������delָ��the_node���Ǹ��ڵ�
			it->del = the_node;
			// ����return�ҵ������ֵ
			// ��Ϊ��ָ�뺯������Ҫ��ָ�����͵ķ���ֵ��
			// �����Ҫ������������ķ���ֵ�Ļ�����Ҫ��ͬ�����͵�ָ����գ� 
			// ����������ָ��ָ�����ݵĵ�ַ���ݴ�data 
			int *p = &(the_node->data);
			return (p);
		}
		// the_node ����һ���ķ����ƶ� 
		the_node = the_node->next;
	}
	
	// û���ҵ���Ӧ��ֵ����ô�ͷ���NULL
	// �����ڷ���֮ǰ����Ϊʧ���ˣ�del��Ҫ��ΪNULL
	it->del = NULL;
	return NULL;
}


// �ӹ��λ�ÿ�ʼ������������û�нڵ��ֵΪv��
// �ҵ��˵Ļ����ƶ���꣬������v�����򷵻�NULL 
// ��Ϊ����˫���������� findNext �� findPrevious ��д���ǲ��� 
int *findPrevious(IteratorInt it, int v){
	
	printf("  findPrevious --- ");
	
	// ���ȣ���ֹ����������������������
	if(!it){
		return NULL;
	}
	
	// û����һ�� 
	if(!hasPrevious(it)){
		// ��Ϊʧ���ˣ�del��Ҫ��ΪNULL
		it->del = NULL;
		return NULL;
	}
	
	// �����������Ĳ�ͬ�� 
	Node *the_node = it->cursor_prev;
	while(the_node != NULL){
		// �� it->cursor_prev ��ʼ����
		// �ҵ��˵Ļ� 
		if(the_node->data == v){
			// ����ǰ���꣨���������Ĳ�ͬ�� 
			it->cursor_next = the_node;
			it->cursor_prev = the_node->prev;
			// ��������delָ�룬Ҳ������delָ��the_node���Ǹ��ڵ�
			it->del = the_node;
			// ����return�ҵ������ֵ
			// ��Ϊ��ָ�뺯������Ҫ��ָ�����͵ķ���ֵ��
			// �����Ҫ������������ķ���ֵ�Ļ�����Ҫ��ͬ�����͵�ָ����գ� 
			// ����������ָ��ָ�����ݵĵ�ַ���ݴ�data 
			int *p = &(the_node->data);
			return (p);
		}
		// the_node ����һ���ķ����ƶ������������Ĳ�ͬ�� 
		the_node = the_node->prev;
	}
	
	// û���ҵ���Ӧ��ֵ����ô�ͷ���NULL
	// �����ڷ���֮ǰ����Ϊʧ���ˣ�del��Ҫ��ΪNULL
	it->del = NULL;
	return NULL;
}


// ���ĸ���������malloc����������Ҫ�ڴ�ĺ�����free�ĸ� 
// ������ģ���������malloc�������еĽڵ�Ҳ����malloc��������Ҫ�ͷ�������
// ���Ⱦ�Ҫ�ͷ������еĽڵ�ռ�õ��ڴ棬Ȼ���ͷ�����ռ�õ��ڴ� 
void freeIt(IteratorInt it){
	// �����أ��ÿ��������������ֳ�������� 
	if(!it){
		// ֱ�ӽ��� 
		return;
	}
	
	// ��㶨������ָ��ڵ㣬һ��ָ��ǰ�ڵ㣬һ��ָ��ǰ�ڵ����һ���ڵ�
	// ��Ҫ�ͷŵĽڵ��� temp ָ��Ľڵ� 
	Node *current, *temp;
	current = it->head;
	// ��whileѭ�������һЩ
	while(current != NULL){
		// temp��ʱ��ţ�ÿ���Զ�����Ϊcurrent 
		temp = current;
		// ��current���� 
		current = current->next;
		// �ͷ�currentǰ���Ǹ��ڵ� 
		free(temp);
	}
	
	// �ͷ��������еĽڵ�����ͷ�����ռ�õ��ڴ�
	free(it);
	
	// ����return����������� 
	return;
}
// ==================================================================================



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// ���մ���Ҫɾ������İ��������������к���
// ���һ�Ҫɾ������ÿ�������е�printf���� 

/***********************************************************************************/
// ��������� 
void cheak_result(int correct_answer, int the_output){
	if(correct_answer == the_output){
		printf("��\n");
	} else {
		printf("������\n");
		printf(" --- should return: %d\n", correct_answer);
		printf(" --- but output is: %d\n", the_output);
		// �����ˣ���ʹ���������˳����������� 
		printf(" ============ program termination ============ \n");
		exit(0);
	}
}


// ���ָ���Ƿ�Ϊ�� 
void cheak_pointer(int *pointer){
	if(pointer == NULL){
		printf("��\n");
	} else {
		printf("������\n");
		printf("pointer is not NULL!\n");
	}
}


// ��ӡ������ 
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
	// ��Ϊ previous ������������ hasPrevious ���������
	// ���Դ�ӡ�������� " previous ---   hasPrevious --- " ������
	// �����ǶԵģ���Ϊ����û���˳�������Ҳ��ӡ��һ�� ��
	cheak_result(25, *pointer);
	
	pointer = previous(it);
	cheak_result(33, *pointer);
	
	pointer = next(it);
	// ��Ϊ next ������������ hasNext ���������
	// ���Դ�ӡ�������� " next ---   hasNext --- " ������
	// �����ǶԵģ���Ϊ����û���˳�������Ҳ��ӡ��һ�� ��
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
	  ��ʵֻҪͨ����ʦ������� sample_test_log1 �Ϳ����ýϸߵķ����� 
	  ��Ϊ��ʦ���ղ��Եİ���Ҳ���Ǻܱ�̬��ֻ���н�Ϊ��������
	   
	  ���油��Ĳ�����Ҫ�ǲ� sample_test_log1 ��û�в���ĺ���
	  ���� findPrevious��hasNext ���ຯ�� 
	  Ȼ�󻹲���һЩ�Ƚ����⣬����ͼ����������� 
	  �������Ĳ���ȫ��ͨ������ô8�ֿ϶�����
	  ������10�֣�����2���Ǹ�ʽ��ע���֣�8���Ǵ�ȫ����ȷ�ĵ÷֣�
	   
	  ����Ȥ�Ļ������Բ���һ���ڴ��Ƿ����й©
	  ��ʦ��û�н����ڴ�й©��ֻ����free��������Ҳû�в��ڴ��Ƿ�й©
	*/
	
}


void test_0(){
	
	printf("\n\n ------------ test_0 ------------ \n");
	
	// �½����� 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[9] = {20, 12, 15, 5, 14, 10, 5, 9, 3};
	
	int i;
	for(i=0; i<9; i++){
		integer = add(it, test_array[i]);
		cheak_result(1, integer);
		
		// �ж�hasNext�Ĳ���
	    integer = hasNext(it);
	    cheak_result(0, integer);
	
	    // �ж�hasPrevious�Ĳ���
	    integer = hasPrevious(it);
	    cheak_result(1, integer);
	
	    pointer = next(it);
	    // ������ǵ������ж��Ƿ�ΪNULL
		//////////////////////////////// 
	    cheak_pointer(pointer);
	    ////////////////////////////////
		
		// �ж� previous
	    pointer = previous(it);
	    cheak_result(test_array[i], *pointer);
		
		// �ж� next
	    pointer = next(it);
	    cheak_result(test_array[i], *pointer);
	    
		// ����ǰ�ƶ�������ƶ�������û����
	}
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// �ͷ��ڴ�
	freeIt(it);
}


// test_1�ǿ������������� 
void test_1(){
	
	printf("\n\n ------------ test_1 ------------ \n");
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// ���ﲻ�½�����ֱ��������ΪNULL
	// ��������ζ�����еĺ����ķ��صĶ���"ʧ��"
	IteratorInt it = NULL; 
	
	int integer;
	int *pointer;
	
	// �ж����
    integer = add(it, 1);
    cheak_result(0, integer);
	
    // �ж�û��next�Ĳ���
    integer = hasNext(it);
    cheak_result(0, integer);
	
    // �ж��Ƿ����Previous������
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


//// test_2���� sample_test_log1
//void test_2(){
//	
//	printf("\n\n ------------ test_2 ------------ \n");
//	
//	// �½����� 
//	IteratorInt it = IteratorIntNew();
//	
//	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	int integer;
//	int *pointer;
//	 
//	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	
//	// �ͷ��ڴ� 
//	freeIt(it);
//}


void test_3(){
	
	printf("\n\n ------------ test_3 ------------ \n");
	
	// �½����� 
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


void test_4(){
	
	printf("\n\n ------------ test_4 ------------ \n");
	
	// �½����� 
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
	
    // ���ù��
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


void test_5(){
	
	printf("\n\n ------------ test_5 ------------ \n");
	
	// �½����� 
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


void test_6(){
	
	printf("\n\n ------------ test_6 ------------ \n");
	
	// �½����� 
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
    // ֮ǰ��next��������Ҫ��������˻���ԭ����next��ѯ�Ľ����10
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


// ��Ҫ�������set��Ҳ�������������滻���� 
void test_7(){
	
	printf("\n\n ------------ test_7 ------------ \n");
	
	// �½����� 
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
    // ��Ϊû���ҵ�������ԭ����λ�ò��䣬����ӦΪNULL 
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
	
	// ���ù�� 
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


// ������Ի�ϲ�deleteElm��set��hasNext��hasPrevious��hasNext��hasPrevious 
void test_8(){
	
	printf("\n\n ------------ test_8 ------------ \n");
	
	// �½����� 
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
	
	// �ͷ��ڴ� 
	freeIt(it);
}


// ���һ�����Բ���Ƿǳ��������� 
void test_9(){
	
	printf("\n\n ------------ test_9 ------------ \n");
	
	// �½����� 
	IteratorInt it = IteratorIntNew();
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	int integer;
	int *pointer;
	int test_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i;
	
	// ��һ�� free
    freeIt(it);
	
	// Ȼ����itΪNULL����һ�� 
	// ��Ϊ����ΪNULL�����Ե����κκ�����ӦΪ"ʧ��" 
    it = NULL;
	
	// ��һ�����ù�� 
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
	
    // �ڶ��� free
    freeIt(it);
	
	// �ڶ�����itΪNULL 
    it = NULL;
	
	// �ڶ������ù��
    reset(it);
	
    // �ڶ����½����� 
    it = IteratorIntNew();
	
    for(i=0; i<10; i++){
        integer = add(it, test_array[i]);
        cheak_result(1, integer);
    }
	
	// ���������ù��
    reset(it);
	
    // ������ free
    freeIt(it);
	
	// ��������itΪNULL 
    it = NULL;
	
	// ���Ĵ����ù��
    reset(it);
	
	// �������еĲ���Ӧ�ö�Ϊ"ʧ��" 
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
	
	// ��������ù��
    reset(it);
	
	// ���Ĵ� free 
    freeIt(it);
	
	// ���Ĵ���itΪNULL 
    it = NULL;
	
	// ��������ù��  
    reset(it);
    
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	// �ͷ��ڴ� 
	freeIt(it);
}
/***********************************************************************************/ 



/***********************************************************************************/
// ������ 
int main(){
	
	// ��ʦ����sample_test 
	sample_test_log1();
	
	// ����д�Ĳ��԰������������޸���������Ͳ���cheak.h�ˣ�
	test_0();
	test_1();
//	test_2();	// �ڶ������Ժ���ʦ���� sample_test_log1 һ��һ�������Ժ��� 
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();
	
	printf("\n ------------ pass all test! ------------ \n"); 
	
	return 0;
}
/***********************************************************************************/ 

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



