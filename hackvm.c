#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// constants
#define MAX_CYCLES 10000
#define STACKSIZE 10000
#define MEMORYSIZE 16384

// globals
int Memory[MEMORYSIZE] = {};
int programCounter = 0;
int cycleCounter = 0;
char* Code;

typedef struct {
	int size;
	int items[STACKSIZE];
} STACK;

STACK OperandStack;
STACK CallStack;

void showStack()
{
	int i;
	if (OperandStack.size == 0)
		printf("[]\n");
	else
	{
		printf("[");
		for (i=0; i < OperandStack.size-1; i++)
			printf("%d, ",OperandStack.items[i]);
		printf("%d]\n",OperandStack.items[i]);
	}
}

void init(STACK *ps)
{
	ps->size = 0;
}

void Push(int v)
{
	if ((v < INT_MIN) || (v > INT_MAX)) {
		printf("ERROR: integer overflow\n");
		exit(-1);
	} else if (OperandStack.size == STACKSIZE) {
		printf("ERROR: stack overflow\n");
		exit(-1);
	} else 
		OperandStack.items[OperandStack.size++] = v;
}

int Pop()
{
	if (OperandStack.size == 0){
		printf("ERROR: stack underflow\n");
		exit(-1);
	} else 
		return OperandStack.items[--OperandStack.size];
}
	
void DoPrintChar()
{
	printf("%c",Pop()&0x7F);
}

void DoPrintInt()
{
	printf("%d",Pop());
}

void DoAdd()
{
	Push(Pop()+Pop());
}

void DoSub()
{
	int a = Pop();
	int b = Pop();
	Push(b-a);
}

void DoMul()
{
	Push(Pop()*Pop());
}

void DoDiv()
{
	int a = Pop();
	int b = Pop();
	Push(b/a);
}
	
void DoCmp()
{
	int a = Pop();
	int b = Pop();
	if (a < b)
		Push(1);
	else if (a > b)
		Push(-1);
	else
		Push(0);
}

void DoGoTo()
{
	programCounter += Pop();
}
	
void DoGoToIfZero()
{
	int offset = Pop();
	if (Pop() == 0)
		programCounter += offset;
}

void DoCall()
{
	CallStack.size++;
	CallStack.items[CallStack.size] = programCounter;
	programCounter = Pop();
}
	
void DoReturn()
{
	programCounter = CallStack.items[CallStack.size];
	CallStack.size--;
}

void DoPeek()
{
	int addr = Pop();
	if (addr < 0 || addr > MEMORYSIZE) {
		printf("memory read access violation @%d\n",addr);
		exit(-1);
	} else
		Push(Memory[addr]);
}

void DoPoke()
{
	int addr = Pop();
	if (addr < 0 || addr >= MEMORYSIZE) {
		printf("memory write access violation @%d\n",addr);
		exit(-1);
	} else
		Memory[addr] = Pop();
}
	
void DoPick()
{
	int where = Pop();
	if ((where < 0) || (where > OperandStack.size)) {
		printf("out of stack bounds @%d\n",where);
		exit(-1);
	}
	Push(OperandStack.items[OperandStack.size-1-where]);
}
	
void DoRoll()
{
	int where = Pop();
	if ((where < 0) || (where >= OperandStack.size)) {
		printf("out of stack @%d\n",where);
		exit(-1);
	}
	int v = OperandStack.items[OperandStack.size-1-where];
	int i;
	for (i = OperandStack.size-1-where; i<OperandStack.size-1; i++)
	{
		OperandStack.items[i] = OperandStack.items[i+1];
	}
	OperandStack.size--;
	Push(v);
}

void DoDrop()
{
	Pop();
}

void DoEnd()
{
	programCounter = strlen(Code);
}

void DoNothing()
{
	;
}

void OPS(char c)
{
	switch( c )
	{
		case ' ':
			DoNothing();
			break;
		case '\n':
			DoNothing();
			break;
		case 'p':
			DoPrintInt();
			break;
		case 'P':
			DoPrintChar();
			break;
		case '0':
			Push(0);
			break;
		case '1':
			Push(1);
			break;
		case '2':
			Push(2);
			break;
		case '3':
			Push(3);
			break;
		case '4':
			Push(4);
			break;
		case '5':
			Push(5);
			break;
		case '6':
			Push(6);
			break;
		case '7':
			Push(7);
			break;
		case '8':
			Push(8);
			break;
		case '9':
			Push(9);
			break;
		case '+':
			DoAdd();
			break;
		case '-':
			DoSub();
			break;
		case '*':
			DoMul();
			break;
		case '/':
			DoDiv();
			break;
		case ':':
			DoCmp();
			break;
		case 'g':
			DoGoTo();
			break;
		case '?':
			DoGoToIfZero();
			break;
		case 'c':
			DoCall();
			break;
		case '$':
			DoReturn();
			break;
		case '<':
			DoPeek();
			break;
		case '>':
			DoPoke();
			break;
		case '^':
			DoPick();
			break;
		case 'v':
			DoRoll();
			break;
		case 'd':
			DoDrop();
			break;
		case '!':
			DoEnd();
			break;
		default:
			printf("ERROR: '%c' is not a valid command!\n",c);
			exit(-1);
	}
}

char* ReadFile(FILE *f)
{	
	// obtain file size
	fseek(f,0,SEEK_END);
	long fsize = ftell(f);
	rewind(f);
	
	// read file contents into buffer
	char* buffer = (char*)malloc(sizeof(char)*fsize);
	fread(buffer,1,fsize,f);
	fclose(f);
	buffer[fsize] = '\0';
	return buffer;
}

void StartMem(char* init_mem)
{
	int i;
	int j = 0;
	int k = 0;
	char* to;
	for (i=0; i<strlen(init_mem); i++)
	{
		if (init_mem[i] == ',')
		{
			to = (char*)malloc(sizeof(char)*(i-j+1));
			strncpy(to,init_mem+j,i-j+1);
			j = i+1;
			Memory[k] = atoi(to);
			k++;
		}
	}
	to = (char*)malloc(sizeof(char)*(i-j+1));
	strncpy(to,init_mem+j,i-j+1);
	Memory[k] = atoi(to);
}

int main(int argc, char *argv[])
{
	int Trace = 0;
	char* mem;
	int i = 1;
	char op_code;
	
	init(&OperandStack);
	init(&CallStack);
	
	if (argc < 2){
		printf("hackvm [--init <init-mem-filename>] [--trace] <code-filename>\nThe format for the initial memory file is: cell0,cell1,...\n");
		exit(-1);
	}

	while (i < argc)
	{
		if (strcmp(argv[i],"--init") == 0)
		{
			i++;
			FILE *f = fopen(argv[i],"rb");
			mem = ReadFile(f);
			StartMem(mem);
			int k;
		}
		else if (strcmp(argv[i],"--trace") == 0)
			Trace = 1;
		else {
			FILE *f = fopen(argv[i],"rb");
			Code = ReadFile(f);
		}
		i++;
	}
	
	while (programCounter != strlen(Code))
	{
		op_code = Code[programCounter];
		if (Trace)
			printf("@%d %c ",programCounter,op_code);	
		programCounter++;
		cycleCounter++;
		if (cycleCounter > MAX_CYCLES)
		{
			printf("too many cycles\n");
			exit(-1);
		}
		OPS(op_code);
		if (Trace)
			showStack();
	}
	
	return 0;
}
