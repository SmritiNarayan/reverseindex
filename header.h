#ifndef header_h
#define header_h

#define max_story_words 1000
#define max_word_length 100

typedef struct node
{
    struct node *prev;
    char data[max_word_length];
    struct node *next;
    int para;
    int line;
    struct anode *right;
} node;

typedef struct anode
{
    struct anode *next;
    int para;
    int line;
}anode;

typedef struct svy
{
	struct node *l;
	int contains;
} svy;

void insertLast(char* data,int para,int line);
void display();
void fileHandle(char* file_name);
void freq_alter();
void free_mem(FILE *file_story,char *p,svy* arr);
void makearray(struct svy* arr);
void makearraySearch(svy* arr,char* sw);
void searchwrd(char* sw);
void normalindex(char* str,char* file_name);

int binarySearch(char *valueToSearch);
int binary_searchStop(char list_of_words[][max_word_length], int size, char *target);
int stop(node* temp);

node* create(node* temp,char* data,int para,int line);
node * middleNode( node * headd , node * lastt);
anode* createa(anode* temp,int para,int line);

#endif
