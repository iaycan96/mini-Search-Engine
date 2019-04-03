#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <dirent.h>
#define NO_OF_ALPHABETS 26
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define ALPHABET_SIZE (26)
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
int totalNumberOfFiles=0;
struct TrieNode{
	struct TrieNode *children[ALPHABET_SIZE];
	bool isEndOfWord;
	int nodeIndex[10];
	char * filename;
	int numberOfFiles;
	char fname[10][20];
};
struct TrieNode *root;// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void){
	struct TrieNode *pNode = NULL;
	pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
	if (pNode){
		int i;
		pNode->isEndOfWord = false;
		for (i = 0; i < ALPHABET_SIZE; i++) 
			pNode->children[i] = NULL;
	}
	return pNode; 
}
void insert(struct TrieNode *root, const char *key,int keyIndex, char*filename){
	int level; 
	int length = strlen(key); 
	int index;
	struct TrieNode *pCrawl = root;
	for (level = 0; level < length; level++){
		index = CHAR_TO_INDEX(key[level]); 
		if (!pCrawl->children[index]) 
			pCrawl->children[index] = getNode();
		pCrawl = pCrawl->children[index]; 
	}
	pCrawl->isEndOfWord = true;
	pCrawl->numberOfFiles = pCrawl->numberOfFiles + 1;
	if(pCrawl->filename!=NULL){
		if(strcmp(pCrawl->filename,filename)==0){
			pCrawl->numberOfFiles=pCrawl->numberOfFiles-1;
		}
	}
	pCrawl->filename=filename;
	strcpy(pCrawl->fname[pCrawl->numberOfFiles-1],filename);
	pCrawl->nodeIndex[pCrawl->numberOfFiles-1]=keyIndex;
}
void readtxt(char * fName,char *filename){
	int indexcount=0;
	FILE * file;
	char word1[50];
	int i;
	for(i=0;i<50;i++){
		word1[i]=NULL;			//equalize each character of word to null
	}
	char c1;
	int counter1=0;
	file =fopen(fName,"r");		//opening file on reading mode
	if (!file) {
		return 1;                    //if file couldn't found
	}
	while(1){
		if( feof(file) ){
			insert(root,word1,indexcount-strlen(word1),filename);
			break ;			//if end of the file, loop 	ends
		}
		c1 = fgetc(file);		//assings each character of file to c1
		if(c1==' '||c1=='.'||c1==','||c1=='?'||c1=='!')			//if character is '  ' or ' ,' or ' . 'or ' ?'  each characters until
		{  														// these term inserted to the char array(word1)
			if(strlen(word1)>=1){	//if words length is smaller than 1(if there is
			                        //repetetive space or '.' or etc. does not inserts to the list.
				insert(root,word1,indexcount-strlen(word1),filename);
				for(i=0;i<50;i++){
					word1[i]=NULL;			//equalize each character of word to null
				}
				counter1=0;
				indexcount++;
				continue;
			}
		}
		if(c1>=65&&c1<=90){					//if letters are uppercase makes them lower from ascii code
			c1=c1+32;
		}
		if(c1>=97&&c1<=122){					//if character is letter inserts this letter to word1's next element
			word1[counter1]=c1;
			counter1++;
		}
		indexcount++;
	}
	fclose(file);							//closing file
}
void printWords(struct TrieNode* rootnode, char* wordArray, int pos,char*prefix,int option,char *takenfiles[50],int totalfiles ){
	if(rootnode == NULL)
		return;
	if(rootnode->isEndOfWord){
		printWord(wordArray, pos,prefix,rootnode->nodeIndex,rootnode->numberOfFiles,option,rootnode,takenfiles,totalfiles);
	}
	for(int i=0; i<NO_OF_ALPHABETS; i++){
		if(rootnode->children[i] != NULL){
			wordArray[pos] = i+'a';
			printWords(rootnode->children[i], wordArray, pos+1,prefix,option,takenfiles,totalfiles);
		}
	}
}
int prepareprefix(struct TrieNode* rootnode,char*prefix,int option ,char*takenfiles[50]){
    struct TrieNode * pCrawl=rootnode;
    int index;
    char wordArray[100];
    for (int i=0;i<strlen(prefix);i++){
        index=CHAR_TO_INDEX(prefix[i]);
        if (!pCrawl->children[index])
            return 0;
        pCrawl=pCrawl->children[index];
    }
    printWords(pCrawl,wordArray,0,prefix,option,takenfiles,0);
    return 1;
}
void printWord(char* str, int n,char*prefix,int nodeIndex[10],int numberOfFiles, int option,struct TrieNode* rootnode,char *takenfiles[50],int totalfiles){
	if(option==1){
	    printf("%s",prefix);
		for(int i=0; i<n; i++){
			printf("%c",str[i]);
		}
        printf("\n");
        for (int j = 0; j <rootnode->numberOfFiles ;j++) {
            printf("     =>File name is: %s,Index number is: %d\n",rootnode->fname[j],nodeIndex[j]);
        }
        printf("\n");
        printf("\n");
	}
	else if(option==2){
	    int count=0;
	    for (int i=0;i<totalfiles;i++){
           for (int j = 0; j <rootnode->numberOfFiles ; j++) {
               if(!strcmp(takenfiles[i],rootnode->fname[j])){
                   count++;
                   break;
               }
           }
	   }
	   if(count==totalfiles){
           for(int i=0; i<n; i++){
               printf("%c",str[i]);
           }
           printf("\n");
	   }
	}
}
void getPath(){
	DIR *d;
	struct dirent *dir;
	char directory[200];
	char path[256];
	printf("Enter the directory:");
	fgets(directory, sizeof(directory), stdin);
	directory[strlen(directory)-1]=NULL;
	d = opendir(directory);

	if (d){
		while ((dir = readdir(d)) != NULL)
		{
			if(dir->d_name[strlen(dir->d_name)-1]=='t'&&dir->d_name[strlen(dir->d_name)-2]=='x'&&
			   dir->d_name[strlen(dir->d_name)-3]=='t'&&dir->d_name[strlen(dir->d_name)-4]=='.'){
                totalNumberOfFiles++;
				strcpy(path,directory);

				strcat(path,"/");
				strcat(path,dir->d_name);
				readtxt(path,dir->d_name);

			}
		}
		closedir(d);
	}
}
int main(){
	char* opt[10];
	int option,checkprefix;
	char filenames[100];
	printf("Welcome to our program(-.-)\n\n");
	char prefix[50];
	root = getNode();
	getPath();
	char wordArray[100];
	char *takenFiles[20];
	while(1) {
        printf("-----------------------------\n1-)    For Prefix enter 1:\n2-)    For Common Words enter 2:\n3-)    For exit enter 3:\n");
        fgets(opt, sizeof(opt), stdin);
        option = atoi(&opt);
        printf("\n");
        if (option == 1) {
            printf("Enter the prefix:");
            fgets(prefix, sizeof(prefix), stdin);
            prefix[strlen(prefix) - 1] = NULL;

            checkprefix= prepareprefix(root,prefix,option,takenFiles);
            if(!checkprefix)
                perror("There is no word with this prefix");
        }
        else if (option == 2) {
            printf("Enter the filenames:");
            fgets(filenames, sizeof(filenames), stdin);
            //printf("%s,%d\n",filenames,strlen(filenames));
            filenames[strlen(filenames)-1]=NULL;
            char s[1]=" ";
            for (int k = 1; k <strlen(s) ; k++) {
                s[k]=NULL;
            }
            int i=0;
            char*token;
            token=strtok(filenames,s);
            printf("%s   %s,%d\n",s,filenames,strlen(filenames));
            while( token != NULL ) {
                takenFiles[i]=token;
                printf("i:%d filename=%s\n",i,takenFiles[i]);
                i++;
                token = strtok(NULL, s);
            }
            printf("Common words of all %d files are:\n", i);
            printWords(root, wordArray, 0, prefix, option,takenFiles,i);
            for (int j = 0; j <i ; j++) {
                takenFiles[j]==NULL;
            }
        }
        else if(option==3){
            printf("Good BYEE!!");
            return 0;
        }
        else{
            printf("Yoooo, Wrong input. Pleasee try againn!!!");
        }
    }
}