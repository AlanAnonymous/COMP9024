说明：
  代码大概215行
  备注大概380行
  测试大概1000行（当然，测试程序是大佬们写的，我只是搬运工）


此assignment常见错误：
  1. Segmentation fault: 当指针是NULL的情况下，对其进行不恰当的操作，将会出现这种错误，所以检查指针
  2. 程序没输出： 一般都是当指针为NULL的情况下，用了->prev，->next或->data，则将会出现这种错误

一般解决方法： 
  加上判断指针是否为NULL的条件，例如：
  if(pointer != NULL){
    XXXXXXX;
  }


最后，说明一下这里上传的代码和我提交给老师的思路做法并不一样
下面就是我当时的做法（仅包含开头部分）
稍微解释一下：
cursor定义为始终指向^的前面那个节点；
direction代表方向，向前向后或无方向，用于deleteElm和set函数


// struct Node
typedef struct Node {
	struct Node *prev;	// previous of node
	struct Node *next;	// next of node
	int data;			      // data, value
} Node;

// struct IteratorIntRep
typedef struct IteratorIntRep {
	// 'Node' is a name which has already been defined
	Node *head;     // head has 3 property: previous, next and data(values)
	Node *cursor;   // cursor alse has 3 property: previous, next and values
	int direction;	// -1 means forward direction, 1 means reverse direction, and 0 means do not move
} IteratorIntRep;
