#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include <time.h>

node* head = NULL;
node* last = NULL;

int num_words = 0;
int para=1;
int line_no=1;

int user_words=0;
clock_t start, end;
double cpu_time_used;

void fileHandle(char* file_name)
{

	char data[max_word_length];
	char line[max_story_words] = {0};
	char *p;
	char *punct="\n\t ,!.:;?*@#^<>\\+[]{}/&%_=()\"-0123456789";
	svy* arr = (svy*) malloc(26 * sizeof(svy));//changed

	FILE *file_story = fopen (file_name, "r");

	if (file_story==NULL)
	{
		printf("Unable to open \n");
		return ;
	}

	/* read each line in file */
	while (fgets (line, max_word_length, file_story) != NULL)
	{
		if(line[0]==13 || line[0]=='\n' )
		{
			para++;
			line_no=0;
		}
		/* tokenize line into words removing punctuation chars in punct */
		for (p = strtok (line, punct); p != NULL; p = strtok (NULL, punct))
		{
			char *c = p;       /* save start address of p */
			for (; *c; c++)
			{
					if ('A' <= *c && *c <= 'Z')
					    *c += 32;
                    if (*c == 13)
                        *c = 0 ;
			}
			if(p[0] != 13 && p[0] != 0)
			{
				strcpy(data,p);
				insertLast(data,para,line_no);
			}
		}
		line_no++;
	}
	freq_alter();
	display();
	printf("\nTotal words: %d\n",num_words);
	printf("\nEnter the no. of words you require index for: ");
	scanf("%d",&user_words);
	int h=0;
	char user_arr[user_words][max_story_words];
	printf("Enter your %d words:\n",user_words);
	while(h<user_words)
    {
        scanf("\t%s",user_arr[h]);
        h++;
    }
    h=0;
	printf("\nWithout Indexing:");
	while(h<user_words)
    {
        start=clock();
        normalindex(user_arr[h], file_name);
		end=clock();
		h++;
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tTime taken=%lf\n",cpu_time_used);

    }
    printf("\nReverse Index Search:\n");
    h=0;
    while(h<user_words)
    {
        start=clock();
        searchwrd(user_arr[h]);
        end=clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        h++;
        printf("\tTime taken=%lf\n",cpu_time_used);

    }
	makearray(arr);
	h=0;
	printf("\nReverse Indexing modified:\n");
	printf("Smriti Vidhu Yash (SVY) Search.\n");
	while(h<user_words)
    {
        start=clock();
        makearraySearch(arr,user_arr[h++]);
		end=clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tTime taken=%lf\n",cpu_time_used);

    }

	//retrieve();

	printf("\nThanks for searching.\n");
	free_mem(file_story,p,arr);

	return ;
}

void insertLast(char* data,int para,int line)
{
	node* temp = NULL;
	node* q=head;
	node* p= NULL;
	int checker;
	temp = create(temp,data,para,line);
	checker = stop(temp);
	if(checker == 1)
	{
		//printf("%s",temp->data);
		while((q != NULL) && (strcmp (q -> data, data) < 0))//traversing
		{
			p=q;
            q=q->next;
		}
		if(q == NULL)
		{
			if(p== NULL)//list is empty
			{
				head = temp;
				last = head;
			}
			else//insert last
			{
				p -> next = temp;
				temp->prev=p;
				last = temp;
			}
		}
		else
		{
			if(p== NULL)
			{
				temp -> next = q;
				q->prev=temp;
				head = temp;
			}
			else
			{
				p -> next = temp;
				temp->prev=p;
				temp -> next = q;
				q->prev=temp;
			}
		}
		num_words++;
	}
}

node* create(node* temp,char* data,int para,int line)
{
	temp = (node*)malloc(sizeof(node));
	temp->prev =  NULL;
	temp->next =  NULL;
	temp->right = NULL;
	strcpy(temp -> data, data);
	temp -> para = para;
	temp -> line = line;
	return temp;
}

anode* createa(anode* temp,int para,int line)
{
	temp = (anode*)malloc(sizeof(anode));
	temp->next = NULL;
	temp -> para = para;
	temp -> line = line;
	return temp;
}

int stop(node* temp)
{
    //please add new words alphabetically so that we dont make in repititions and its easy to debug
	char keys[][max_word_length] = 	{
	"a","above","after","all","also","am","an","and","answer","any","are","as","at","b","be","because","but","by","c","d","e","else","f",
	"for","from","g","give","got","h","had","her","him","how","i","if","in","into","is","it","j","k","l","m","mine","much","my","n","no",
	"nor","not","o","of", "off","only","or","our","p","q","r","s","such","t","that","the","their","them","then","there","they","this",
	"those","to","u","upon","v","very","w","was","were","when","where","who","with","would","x","y","yet","you","your","z"
	};

	int total = (sizeof(keys) / max_word_length);
	int check = 0;
	check = binary_searchStop(keys, total, temp -> data);
	if(strlen(temp->data) < 3 )
		check = -1;
	return check;
}

int binary_searchStop(char list_of_words[][max_word_length], int size, char *target)
{
    int bottom= 0;
    int mid;
    int top = size - 1;

    while(bottom <= top){

		mid = (bottom + top)/2;
        if (strcmp(list_of_words[mid], target) == 0)
		{
            return -1;
        }
		else if (strcmp(list_of_words[mid], target) > 0)
		{
            top = mid - 1;
        }
		else if (strcmp(list_of_words[mid], target) < 0)
		{
            bottom = mid + 1;
        }
    }
	return 1;
}
void display()
{
	node* temp = head;
	anode* temp2 = NULL ;
	if (temp == NULL)
    {
		printf("\nList empty to display ");
        return;
    }
    printf("\nLinked list elements from begining : ");

    while (temp->next != NULL)
    {
		printf("\nWord: %s \tParagraph: %d \tLine: %d", temp->data, temp -> para,temp -> line);
		temp2=temp->right;
        while(temp2!= NULL)
        {
            printf("\tParagraph: %d \tLine: %d",temp2 -> para,temp2 -> line);
            temp2=temp2->next;
        }
        temp = temp->next;
    }
	printf("\nWord: %s \tParagraph: %d \tLine: %d", temp->data, temp -> para,temp -> line);
}

void freq_alter()
{
	node* q=head;
	node* temp = NULL;
	while(q->next !=NULL)
    {
		anode* tempa = NULL;
		while(strcmp(q->data,q->next->data)==0)
		{
			int pp=q->next->para;
			int ll=q->next->line;
			tempa=createa(tempa,pp,ll);

			if(q->right == NULL)
			{
				q->right=tempa;
			}

			else
			{
				anode* t=q->right;
				while(t->next!= NULL)
					t=t->next;
				t->next=tempa;
			}
			temp=q->next->next;
			if(q->next->next)
			{
				q->next->next->prev=q;
				q->next=temp;
			}
		}
		q=q->next;
	}

	last = create(last,"$$",-1,-1);
	q->next = last;
	last->prev = q;
}



void makearraySearch(svy* arr,char* sw)
{
	printf(" To index: %s \n\n",sw);
	int found = 0;
	int x = sw[0] - 97;
	node* temp = arr[x].l;
	while(temp != NULL)
	{
		if(strcmp(sw,temp->data)==0)
		{
			printf("%s\n",temp->data);
			printf("  Found at:\n\tPara %d and Line %d\n",temp->para,temp->line);
			anode* atemp = temp->right;
			while(atemp!=NULL)
			{
				printf("\tPara %d and Line %d\n",atemp->para,atemp->line);
				atemp = atemp->next;
			}
			found = 1;
			break;
		}
		temp = temp->next;
	}
	if(found == 0)
		printf("\tNot Found.\n");	
}

void makearray(svy* arr)
{
	node* q=head;
	int x=0;
	int prev_x=x;
	int i;
	for(i=0;i<26;i++)//initialising contains to zero. or it has junk values that cant be checked
	{
		arr[i].contains=0;
	}
	while(q->next!=NULL)
    {
        x=q->data[0]-97;
        prev_x=x;
        arr[x].contains=1;
        arr[x].l=q;

        while((x==prev_x)&&(q->next!=NULL))
		{
            q=q->next;
            x=q->data[0]-97;
		}
        q->prev->next=NULL;//once the letter type changes end the sublist
		q->prev = NULL;
	}
}

int binarySearch(char *valueToSearch)
{
    int found=0;
    node * p= head;
    node * q= last;
    //int flag = 0;
	while(q->next==NULL || q->next!= p)
    {
		if(strcmp(p->data,q->data)==0)
            break;
		node* middle= middleNode(p,q);


		if(middle==NULL)
		{
			return 0;
		}

		if( strcmp(middle->data,valueToSearch )==0)
		{
			printf("  Found at:\n Para %d and Line %d ",middle->para,middle->line);
			found=1;
			anode* r=middle->right;
			while(r!=NULL)
			{

				printf("\n Para %d and Line %d ",r->para,r->line);
				r=r->next;
			}
			return 1;
		}

		else if (strcmp(middle->data,valueToSearch )<0 )
		{
			p = middle->next;
		}
		else
		{
			q = middle;
		}
	}

	if(found==0)
		printf("not found");
	return 0;
}

node * middleNode( node * headd , node * lastt)
{
	if( headd == NULL )
		return NULL;
	node * slowPtr = headd;

	if(headd -> next != NULL)
	{

		node * fastPtr = headd -> next;

		while ( fastPtr != lastt )
		{
			fastPtr = fastPtr -> next ;

			if( fastPtr != lastt )
			{
				slowPtr = slowPtr -> next ;      // Notice that for each loop iteration :
				fastPtr = fastPtr -> next ;      // slowPtr moves just one location while fastPtr moves two nodes at a time.
			}
		}

		return slowPtr ;        // At the end , the slowPtr will be pointing to the middle node
	}
	else
		return NULL;
}

void searchwrd(char* sw)
{
	printf("\n To index: %s \n\n",sw);
	int found = 0;
	node* q=head;
	while(q != NULL)
	{
		if (strcmp(q -> data, sw)==0)
		{
			printf("  Found at:\n\n\tPara %d and Line %d \n",q->para,q->line);
			anode* r=q->right;
			while(r!=NULL)
			{
				printf("\tPara %d and Line %d \n",r->para,r->line);
				r=r->next;
			}
			found = 1;
		}
		q=q->next;
    }
	if(found == 0)
		printf("\tWord not Found.\n");
}

void free_mem(FILE *file_story,char *p,svy* arr)
{
	node* temp = head;
	if(head == NULL)
		return ;
	else
	{
		while(temp!= NULL)
		{
			node* tempA = temp ;
			temp = temp -> next;
			free(tempA);
		}
	}
	free(file_story);
    free(p);
	free(arr);
	head= NULL;
}

void normalindex(char* str,char* file_name)
{
	int paragraph = 1;
	printf("\n To index: %s \n\n",str);
	char data[max_word_length];
	char line[max_story_words] = {0};
	char *p;
	char *punct="\n\t ,!.:;?*@#^<>\\+[]{}/&%_=()\"-0123456789";
	FILE *file_story = fopen (file_name, "r");
	int found = 0;
	if (file_story==NULL)
	{
		printf("Unable to open \n");
		return ;
	}

	/* read each line in file */
	while (fgets (line, max_word_length, file_story) != NULL)
	{
		if(line[0]==13 || line[0]=='\n')
		{
			paragraph++;
			line_no=0;
		}
		/* tokenize line into words removing punctuation chars in punct */
		for (p = strtok (line, punct); p != NULL; p = strtok (NULL, punct))
		{
			char *c = p;       /* save start address of p */
			for (; *c; c++)
			{
				if ('A' <= *c && *c <= 'Z')
					*c += 32;
				if (*c == 39 || *c == 13)
					*c = 0;
			}
			if(p[0] != 13 && p[0] != 0)
			{
				strcpy(data,p);
				if(strcmp(str,data)==0)
                {
                    printf("\tPara %d -> Line %d \n",paragraph,line_no);
					found = 1;
                }
			}

		}
		line_no++;
	}
	if(found == 0)
		printf("\tNot Found.\n");
}

