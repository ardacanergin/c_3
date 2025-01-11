//Arda Can Ergin 150123060
//this program is created to mimic a String like structure
//program creates a data structure mainly focused on char arrays where you can store a String(char array) in a node and either increment or decrement
//the value of the String buy just simply assigning a key String that is same as a pre-existing node
//There are additional functions such as maxKey and minKey where their return value is either maximum or minimum iterated String key based on
//their count data. Additional there is a one more function to print the lists.

//There are comment lines that contains unnecessary puts and printf commands, I've used them for debugging and didn't delete to show my work
//Also printList function contains another implementation in the comment line that i found easier but didn't use in the program since
//it was giving different output than the example

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//construct the String
typedef struct String {
    char str[25]; //char array to store the string, what program focuses on
    int count; //how many times the key string is called
    struct String *nextCount; //next node in the list based on count order (descending)
    struct String *nextAlpha; //next node in the list based on alphabetical order
} String;

void inc(String** headAlpha, String** headCount, char* key);
/*
increments the count field of a String node that has the same key char array as the parameter. If there is no node that
contains the key, a new node will be added to the list containing that key as its str field.
*/
void dec(String** headAlpha, String** headCount, char* key);
/*
decrement the count of a string that has the same str field as key. If the node that has the key value as their str becomes 0 remove that
String from the list.
*/

char* getMaxKey(String* head);
/*
return the str field of a String node that has the max count
*/

char* getMinKey(String* head);
/*
return the str field of a String node that has the min count
*/

char* printList(String* head, int type);
/*
print the list. List printed differently based on the type parameter. Type (1) : alphabetical order
Type (2) : descending count order
*/

void insert(String** headAlpha,String **headCount, char *key);
//additional function to create and insert nodes into the list

int main() {
    String *headAlpha=NULL; //think it as two different list
    String *headCount=NULL;
    String *head=NULL; //pointer to the first node in the list (I prefer to make head of the list based on the count)

    //classic file read prosedure
    FILE *input;
    input = fopen("input.txt","r");
    if(input==NULL) {
        puts("file open failed");
    }

    char s[25];
    int  printType;
    //int counter=0;

    //read-file
    while(!feof(input)) {
        //puts("inside while loop");
        fscanf(input,"%s",s);

        if(strcmp(s,"inc")==0) {
            //puts("inside inc");
            fscanf(input,"%s",s);
            //printf("current key: %s\n",s);
            inc(&headAlpha,&headCount,s); //increment funciton
            //counter++;
        }else if(strcmp(s,"dec")==0) {
            fscanf(input,"%s",s);
            dec(&headAlpha,&headCount,s); //decrement funciton
            //counter++;
        }
        else if(strcmp(s,"getMaxKey")==0) {
            //counter++;
            printf("%s\n",getMaxKey(headCount));
        }
        else if(strcmp(s,"getMinKey")==0) {
            //counter++;
            printf("%s\n",getMinKey(headAlpha));
        }
        else if(strcmp(s,"printList")==0) {
            fscanf(input,"%d",&printType);
            //counter++;
            if(printType==1){
                printList(headAlpha,1);
            }
            else if(printType==2){
                printList(headCount,2);
            }
            else {
                puts("unrecognized printList tpye!");
            }
        }
    }

    fclose(input);
    //printf("counter: %d",counter);
}

//insert non-existing string to the list
void insert(String** headAlpha,String **headCount, char *key) {
    String *newPtr = malloc(sizeof(String));

    if(newPtr!=NULL) {
        //puts("malloc success");
        strcpy(newPtr->str,key); //copy key to the str of new node
        newPtr->count=1;
        newPtr->nextAlpha=NULL;
        newPtr->nextCount=NULL;


        String *previousAlpha = NULL;
        String *currentAlpha = *headAlpha ;

        String *currentCount = *headCount;

        //puts("outside while");
        //insert new node respect to alphabetic order
        while(currentAlpha!=NULL && strcmp(key,currentAlpha->str)>0) {
            previousAlpha = currentAlpha;
            //puts("match 1 ");
            currentAlpha= currentAlpha->nextAlpha;
            //puts("match 2");
        }
        if(previousAlpha==NULL) { //insert new node at the start of the list
            newPtr->nextAlpha = *headAlpha;
            *headAlpha= newPtr;

        } else { //insert new node into the correct place in the list
            newPtr->nextAlpha = currentAlpha;
            previousAlpha->nextAlpha = newPtr;
        }
        //insert newPtr to the count list
        if (*headCount==NULL) {
            //the list is empty therefore first insertion will be the head
            *headCount = newPtr;
        } else {
            //go to the end of the list
            while(currentCount->nextCount !=NULL) {
                currentCount=currentCount->nextCount;
            }
            //now new node is at the end of the list
            currentCount->nextCount=newPtr;
        }


        //printf("%s - insertion made\n",newPtr->str); //for debug reasons
    } else {
        puts("alphabetic insertion error!");
    }
}

void inc(String** headAlpha, String** headCount, char* key) {
    //increment count by 1 for existing key

    int strCounter=0;

    String *previousAlpha = NULL;
    String *currentAlpha = *headAlpha;

    String *previousCount =NULL;
    String *currentCount = *headCount;

    while(currentAlpha!=NULL ) {
        //puts("inc while inside !");
        if(strcmp(currentAlpha->str,key)==0) {
            currentAlpha->count++;
            strCounter++; //str count determines whether one of the nodes has the key value or is it first occurrence of the key

            //remove the node from the list
            if(*headCount==currentAlpha) {
                *headCount= currentAlpha->nextCount;
            } else {
                while(currentCount!=currentAlpha) {
                    previousCount=currentCount;
                    currentCount=currentCount->nextCount;
                }
                previousCount->nextCount=currentCount->nextCount;
            }

            //re-insert the node
            previousCount=NULL;
            currentCount=*headCount;
            while(currentCount !=NULL && currentCount->count>currentAlpha->count ) {
                previousCount = currentCount; //go to next item
                currentCount=currentCount->nextCount;
            }
            if(previousCount==NULL) { //attach to the beginning
                currentAlpha->nextCount=*headCount;
                *headCount=currentAlpha;
            } else { //insert somewhere between the two nodes
                previousCount->nextCount = currentAlpha;
                currentAlpha->nextCount = currentCount;
            }
        }
        //puts("after if inside while !");
        previousAlpha = currentAlpha;
        currentAlpha = currentAlpha->nextAlpha;
    }

    if(strCounter ==0) { // if counter==0, string is not found in the current list then it does not exist, then insert newString
        insert(headAlpha, headCount,key);
        //puts("non-existing string node added to the list");
        return 0;
    }
}
//implementation is very similar to the inc function
void dec(String** headAlpha, String** headCount, char* key) {
    int strCounter=0;

    String *previousAlpha = NULL;
    String *currentAlpha = *headAlpha;

    String *previousCount =NULL;
    String *currentCount = *headCount;

    while(currentAlpha!=NULL ) {
        //puts("dec while inside !");
        if(strcmp(currentAlpha->str,key)==0) {
            currentAlpha->count--; //decrement the number
            strCounter++;

            //remove the node from the list
            if(*headCount==currentAlpha) {
                *headCount= currentAlpha->nextCount;
            } else {
                while(currentCount!=currentAlpha) {
                    previousCount=currentCount;
                    currentCount=currentCount->nextCount;
                }
                previousCount->nextCount=currentCount->nextCount;
            }
            //if currentAlpha->count == 0 that means the number of strings is 0 and node is not needed thus we can leave it un-inserted and then free
            if(currentAlpha->count!=0) {
                //re-insert the node
                previousCount=NULL;
                currentCount=*headCount;
                while(currentCount !=NULL && currentCount->count>currentAlpha->count ) {
                    previousCount = currentCount; //go to next item
                    currentCount=currentCount->nextCount;
                }
                if(previousCount==NULL) { //attach to the beginning
                    currentAlpha->nextCount=*headCount;
                    *headCount=currentAlpha;
                } else { //insert somewhere between the two nodes
                    previousCount->nextCount = currentAlpha;
                    currentAlpha->nextCount = currentCount;
                }
            } else if(currentAlpha->count==0) {
                //remove from alphabetical ordered list as well (node in the descending number ordered list has already deleted)
                if(previousAlpha==NULL){
                    *headAlpha=currentAlpha->nextAlpha;
                }
                else {
                    previousAlpha->nextAlpha=currentAlpha->nextAlpha;
                }
                //free the memory allocated for that string
                free(currentAlpha);
            }
        }
        //puts("after if inside while !");
        previousAlpha = currentAlpha;
        currentAlpha = currentAlpha->nextAlpha;
    }

    if(strCounter ==0) { // if counter==0, string is not found in the current list then it does not exist, decrementing is not possible -> error
        puts("error: non-existing key decrement");
        return 0;
    }
}

char* printList(String *head, int type) {
    if(type==1) {//alphabetical order (alphabetical order's head pointer is passed as a parameter
        String *currentAlpha = head;
        puts("the list is in alphabetical order: ");
        while(currentAlpha!=NULL) {
            printf("%s (%d)",currentAlpha->str,currentAlpha->count); //print data fields of a node
            printf(" --> ");
            currentAlpha= currentAlpha->nextAlpha; //walk through the list
        }
        printf("NULL\n");
        printf("\n");

    }
    //same operations as type 1
    if(type==2) { //count descending order (count's head pointer passed
        String *currentCount = head;
        puts("the list in descending order:");
        while(currentCount!=NULL) {
            printf("%s (%d) ",currentCount->str,currentCount->count);
            printf("-->");
            currentCount= currentCount->nextCount;
        }
        printf("NULL\n");
        printf("\n");
    }
}
char* getMaxKey(String* head){
    //count descendant order list's first node always gives the maxKey
    return head->str;
}

char* getMinKey(String* head){
    String *lastAlpha = head;

    while(head!=NULL){
        //find the smallest value, iterate trough every node
        if(lastAlpha->count>head->count){
            lastAlpha=head;
        }
        head=head->nextAlpha;
    }

    return lastAlpha->str;
    //this is another solution for this function however the output is different than yours but still true though nevertheless I didn't use it
    //to prevent complications
    /*while(head->nextCount!=NULL){
        head = head->nextCount;
    }
    return head->str;*/
}
