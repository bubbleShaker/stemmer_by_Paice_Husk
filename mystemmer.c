#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE (256)
#define TRUE 1
#define FALSE -1

void reverse(char *msg, size_t maxsize){
    size_t i;
    size_t len = strnlen(msg, maxsize);
    for (i = 0; i < len / 2; i++){
        msg[i] ^= msg[len - 1 - i];
        msg[len - 1 - i] ^= msg[i];
        msg[i] ^= msg[len - 1 - i];
    }
    //引用:https://blackwhitebear.hateblo.jp/entry/2018/01/05/170034
}

char* substr(const char *src, int m, int n){
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++){
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
    //引用:https://www.techiedelight.com/ja/implement-substr-function-c/
}

struct Rule {
    char* checkStr;
    char* shouldFirst;
    int eraseNum;
    char* newStr;
    char continueChar;
};

char str[1024];
struct Rule tuples[10];
int isEnded;
int isChanged;
int haveChanged;
int tlen;

void makeTuples(int ind,char* cStr,char* shFst,int erNum,char* nStr,char cChar){
    tuples[ind].checkStr=cStr;
    tuples[ind].shouldFirst=shFst;
    tuples[ind].eraseNum=erNum;
    tuples[ind].newStr=nStr;
    tuples[ind].continueChar=cChar;
}

void makeRule(int n){
    tlen=n;
    makeTuples(0,"a","",1,"",'.');
    makeTuples(1,"sei","",3,"y",'>');
    makeTuples(2,"ycn","",2,"t",'>');
    makeTuples(3,"tne","",3,"",'>');
    makeTuples(4,"mu","*",2,"",'.');
    makeTuples(5,"gni","",3,"",'>');
    makeTuples(6,"re","",2,"",'>');
}

int isVowel(char c){
    if(c=='a')return TRUE;
    if(c=='i')return TRUE;
    if(c=='u')return TRUE;
    if(c=='e')return TRUE;
    if(c=='o')return TRUE;
    return FALSE;
}

int isCons(char c){
    if(isVowel(c)==TRUE)return FALSE;
    else return TRUE;
}

void exStem(int ind){
    char* nStr=(char*)calloc(strlen(str),sizeof(char));
    strcpy(nStr,str);
    nStr[strlen(nStr)-tuples[ind].eraseNum]='\0';
    strcat(nStr,tuples[ind].newStr);
    int isOk=FALSE;
    for(int i=0;i<3;i++){
        if(isVowel(nStr[i])==TRUE)isOk=TRUE;
        if(nStr[i]=='y')isOk=TRUE;
    }
    if(isOk==TRUE){
        strcpy(str,nStr);
        free(nStr);
        return;
    }
    free(nStr);
}

void checkRule(int ind){
    if(strcmp(tuples[ind].shouldFirst,"*")==0){
        if(haveChanged==TRUE)return;
    }
    reverse(str,MAXSIZE);
    int searchLen=strlen(tuples[ind].checkStr);
    char* searchStr=substr(str,0,searchLen);
    if(strcmp(searchStr,tuples[ind].checkStr)==0){
        reverse(str,MAXSIZE);
        if(isVowel(str[0])==TRUE){
            if((strlen(str)-tuples[ind].eraseNum)<2)return;
        }
        if(isCons(str[0])==TRUE){
            if((strlen(str)-tuples[ind].eraseNum)<3)return;
            if((strlen(str)-tuples[ind].eraseNum)==3){
                exStem(ind);
                return;
            }
        }
        str[strlen(str)-tuples[ind].eraseNum]='\0';
        strcat(str,tuples[ind].newStr);
        reverse(str,MAXSIZE);
        if(tuples[ind].continueChar=='.'){
            isEnded=TRUE;
        }
        isChanged=TRUE;
        haveChanged=TRUE;
    }
    reverse(str,MAXSIZE);
}

void stem(){
    int shouldSearch=TRUE;
    while(shouldSearch==TRUE){
        isEnded=FALSE;
        isChanged=FALSE;
        for(int i=0;i<tlen;i++){
            checkRule(i);
            if(isEnded==TRUE){
                shouldSearch=FALSE;
                break;
            }
            if(i==tlen-1){
                if(isChanged==FALSE){
                    shouldSearch=FALSE;
                    break;
                }
            }
        }
    }
}

int main() {
    makeRule(7);
    while(fgets(str, sizeof(str), stdin) != NULL){
        haveChanged=FALSE;
        str[strlen(str)-1]='\0';//改行文字を消す
        stem();
        printf("%s\n", str);
    }
    return 0;
}