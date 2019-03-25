// C implementation of search and insert operations 
// on Trie 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <dirent.h>
#define NO_OF_ALPHABETS 26
#define MAX_WORD_SIZE 100
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (26) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 

// trie node 
struct TrieNode 
{ 
	struct TrieNode *children[ALPHABET_SIZE]; 

	// isEndOfWord is true if the node represents 
	// end of a word 
	bool isEndOfWord;
	int nodeIndex;
	char* filename;
}; 

// Returns new trie node (initialized to NULLs)

struct TrieNode *root;




struct TrieNode *getNode(void) 
{ 
	struct TrieNode *pNode = NULL; 

	pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 

	if (pNode) 
	{ 
		int i; 

		pNode->isEndOfWord = false; 

		for (i = 0; i < ALPHABET_SIZE; i++) 
			pNode->children[i] = NULL;
	} 

	return pNode; 
} 

// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(struct TrieNode *root, const char *key,int keyIndex, char*filename)
{
//	printf("%d,%s\n",keyIndex,key);
	int level; 
	int length = strlen(key); 
	int index; 

	struct TrieNode *pCrawl = root; 

	for (level = 0; level < length; level++) 
	{ 
		index = CHAR_TO_INDEX(key[level]); 
		if (!pCrawl->children[index]) 
			pCrawl->children[index] = getNode(); 

		pCrawl = pCrawl->children[index]; 
	} 

	// mark last node as leaf 
	pCrawl->isEndOfWord = true;
	pCrawl->nodeIndex=keyIndex;
	pCrawl->filename=filename;
}


// Returns true if key presents in trie, else false 
bool search(struct TrieNode *root, const char *key) 
{

	int level; 
	int length = strlen(key); 
	int index; 
	struct TrieNode *pCrawl = root; 

	for (level = 0; level < length; level++) 
	{ 
		index = CHAR_TO_INDEX(key[level]); 

		if (!pCrawl->children[index]) 
			return false; 

		pCrawl = pCrawl->children[index]; 
	}
//

	return (pCrawl != NULL && pCrawl->isEndOfWord); 
}



void readtxt(char * fName,char *filename){

	int indexcount=0;
	FILE * file;
	char word1[50];
	int i;
	for(i=0;i<50;i++)
	{
		word1[i]=NULL;			//equalize each character of word to null
	}
	char c1;
	int counter1=0;



	file =fopen(fName,"r");		//opening file on reading mode
	if (!file) {
	//	perror("File could not found");
		return 1;                    //if file couldn't found
	}
//perror("burda");
	while(1)
	{

		if( feof(file) )
		{

			insert(root,word1,indexcount-strlen(word1),filename);
		//	printf("%s",word1);
			break ;			//if end of the file, loop 	ends
		}
		c1 = fgetc(file);		//assings each character of file to c1
		//printf("%c",c1);
		if(c1==' '||c1=='.'||c1==','||c1=='?'||c1=='!')			//if character is '  ' or ' ,' or ' . 'or ' ?'  each characters until
		{  														// these term inserted to the char array(word1)

			if(strlen(word1)>=1)	//if words length is smaller than 1(if there is
			{  						//repetetive space or '.' or etc. does not inserts to the list.

				//   insert(&list1,word1);
	//			printf("%s ",word1);
//				printf("\n------\n");
				insert(root,word1,indexcount-strlen(word1),filename);

				//printf("%s",word1);
				for(i=0;i<50;i++)
				{
					word1[i]=NULL;			//equalize each character of word to null
				}

				counter1=0;
				indexcount++;
				continue;
			}
		}
		if(c1>=65&&c1<=90)					//if letters are uppercase makes them lower from ascii code
		{
			c1=c1+32;
		}
		if(c1>=97&&c1<=122)					//if character is letter inserts this letter to word1's next element
		{

			word1[counter1]=c1;
			counter1++;
		}


		indexcount++;
	}

	fclose(file);							//closing file

}


void printWords(struct TrieNode* rootnode, char* wordArray, int pos,char*prefix )
{

	if(rootnode == NULL)
		return;

	if(rootnode->isEndOfWord)
	{
		printWord(wordArray, pos,prefix,rootnode->nodeIndex,rootnode->filename);
	}
	for(int i=0; i<NO_OF_ALPHABETS; i++)
	{
		if(rootnode->children[i] != NULL)
		{
			wordArray[pos] = i+'a';
			//printf("asd:      %s      ",wordArray);
			printWords(rootnode->children[i], wordArray, pos+1,prefix);
		}
	}
}

void printWord(char* str, int n,char*prefix,int nodeIndex, char*filename)
{
	for (int j = 0; j < strlen(prefix); j++) {
		if (str[j]!=prefix[j])
			return;

	}


	for(int i=0; i<n; i++)
	{
		printf("%c",str[i]);
	}
	printf("     =>File name is: %s,Index number is: %d:",filename,nodeIndex);
	printf("\n");
}

void getPath(){
	DIR *d;
	struct dirent *dir;
	char directory[200];

	char path[256];
	printf("Enter the directory:");
	fgets(directory, sizeof(directory), stdin);
	directory[strlen(directory)-1]=NULL;
//	printf("%d   \n",strlen(directory));
//	printf("%s\n",directory);
	d = opendir(directory);

	if (d)
	{
		while ((dir = readdir(d)) != NULL)

		{

			if(dir->d_name[strlen(dir->d_name)-1]=='t'&&dir->d_name[strlen(dir->d_name)-2]=='x'&&
			   dir->d_name[strlen(dir->d_name)-3]=='t'&&dir->d_name[strlen(dir->d_name)-4]=='.'){


//				printf("%s\n",dir->d_name);
				strcpy(path,directory);

				strcat(path,"/");
				strcat(path,dir->d_name);
				readtxt(path,dir->d_name);
//				printf("%s\n",path);
			}
		}
		closedir(d);
	}



}







// Driver 
int main(){


	char prefix[50];
	root = getNode();
	getPath();
	char wordArray[100];
	printf("\nEnter the prefix:");
	fgets(prefix, sizeof(prefix), stdin);
	prefix[strlen(prefix)-1]=NULL;
	printWords(root, wordArray,0,prefix);

	return 0; 
} 

