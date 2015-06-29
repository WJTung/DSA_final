#include<iostream>
#include<stdio.h>
#include<string.h>
#include "bank.h"

using namespace std;
#define MAX_SCORE 10000
#define WORD_LEN 62
#define sum1toN(n) ((n)*(n+1)/2)
#define ABS(x) ((x>0)?x:-(x))

char word[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};   //帳號可能會出現的字元

int findMinLen(int len,int score){                      //在差值=score下，字串最短可能長度
    for(int i = 1;i < len;i++)
        if(sum1toN(len - i) <= score)
            return i;
    return len;
}
int getAddLen(int score){                 //測試是否有後綴長度剛好等於score
    int left = 1,right = score + 1,mid;
    while(left != right - 1){
        mid = (left+right)/2;
        if(sum1toN(mid) > score)
            right = mid;
        else
            left = mid;
    }
    if(sum1toN(left) == score)
        return left;
    else
        return -1;
    /*int i;
    for(i = 1;sum1toN(i) < score;i++);
    if(sum1toN(i) == score)
        return originLen + i;
    else 
        return -1;*/
}
void add(char *now,int nowLen){             
    for(int idx = nowLen - 1; idx > 0; idx--){
        if(now[idx] != word[WORD_LEN - 1]){
            now[idx]++;
            return ;
        }
        else
            now[idx] = word[0];
    }
    now[0]++;
    if(now[0] > word[WORD_LEN - 1])
        now[0] = word[0];
    return;
}

//字串>原長時，要窮舉字串尾的排組   
int printAllPostfix(int originLen,char *now,int score,int needed,Bank &bank){
    //printf("YO ");
    int finded = 0,addLen = getAddLen(score);
    char *addNow = NULL;
    if(addLen < 0) return 0;
    else{
        addNow = new char[addLen+1];
        memset(addNow,word[0],sizeof(char) * addLen);
        addNow[addLen] = '\0';
    }
    for(char *addPrv = addNow; strcmp(addPrv,addNow) <= 0 && finded < needed; addPrv = addNow,add(addNow,addLen)){
        strcat(now,addNow);
        if(!bank.existed(now)){
            printf("%s",now);
            finded++;
            if(finded < needed)
                putchar(',');
        }
        now[originLen] = '\0';
    }
    //printf("OY ");
    if(addNow != NULL)
        delete addNow;
    return finded;
}
//output 同score字串，字典序由小到大
int printSameScoreString(const char *origin,int originLen,char *now,int nowIdx,
                         int score,int needed,int changeNum,Bank &bank){
    int finded = 0;
    if(nowIdx == originLen){            //超過原長，如果某長度可以符合score，直接字典序輸出
        finded += printAllPostfix(originLen,now,score,needed - finded,bank);
        return finded;
    }
    if(nowIdx != 0 && sum1toN(originLen - nowIdx) - changeNum * (originLen - nowIdx) == score){ //刪掉idx後string剛好符合score
        char *shortNow = new char[MAX_STRLEN];
        strncpy(shortNow,now,nowIdx);
        shortNow[nowIdx] = '\0';
        if(!bank.existed(shortNow)){
            printf("%s",shortNow);
            finded++;
            if(finded < needed)
                putchar(',');
        }
        if(finded == needed)
            return finded;
        delete shortNow;
    }

    int wordIdx = 0;
    if(originLen - nowIdx < score){                                 //將now[idx]調小(字典序)
        for(;finded < needed && word[wordIdx] < origin[nowIdx];wordIdx++){
            now[nowIdx] = word[wordIdx];
            finded += printSameScoreString(origin,originLen,now,nowIdx+1,score-(originLen-nowIdx),needed-finded,changeNum+1,bank);
        }
    }
    else if(originLen - nowIdx == score){
        for(;finded < needed && word[wordIdx] < origin[nowIdx];wordIdx++){
            now[nowIdx] = word[wordIdx];
            if(!bank.existed(now)){
                printf("%s",now);
                finded++;
                if(finded < needed)
                    putchar(',');
            }
        }
    }

    now[nowIdx] = origin[nowIdx];
    if(finded == needed){
        return finded;
    }
    else{                               //不變，直接測下一個idx
        finded += printSameScoreString(origin,originLen,now,nowIdx+1,score,needed - finded,changeNum,bank);
    }

    if(originLen - nowIdx < score){                                 //將now[idx]調大(字典序)
        for(wordIdx = wordIdx + 1;finded < needed && wordIdx < WORD_LEN;wordIdx++){
            now[nowIdx] = word[wordIdx];
            finded += printSameScoreString(origin,originLen,now,nowIdx+1,score-(originLen-nowIdx),needed-finded,changeNum+1,bank);
        }
    }
    else if(originLen - nowIdx == score){
        for(wordIdx = wordIdx + 1;finded < needed && wordIdx < WORD_LEN;wordIdx++){
            now[nowIdx] = word[wordIdx];
            if(!bank.existed(now)){
                printf("%s",now);
                finded++;
                if(finded < needed)
                    putchar(',');
            }
        }
    }
    now[nowIdx] = origin[nowIdx];
    return finded;
}
/////////////////////////////////find uncreated ID main function//////////////////////////////////
void findUncreatedID(const char *origin,int needNum,Bank &bank){      //窮舉所有可能score,find uncreated ID
    int finded = 0;
    int originLen = strlen(origin);
    char *now = new char[MAX_STRLEN];
    for(int score = 1;finded < needNum && score < MAX_SCORE; score++){
        strcpy(now,origin);
        finded += printSameScoreString(origin,originLen,now,originLen - score,score,needNum - finded,0,bank);
    }
    putchar('\n');
    delete now;
    return ;
}

////////////////////////////////////////find created ID////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int getScore(const char *sa,const char *sb){
    int na = strlen(sa),nb = strlen(sb);
    int score = sum1toN(ABS(na-nb));
    int minLen = min(na,nb);
    for(int i = 0;i < minLen;i++){
        if(sa[i] != sb[i])
            score += minLen - i;
    }
    return score;
}
void insertion(int idx,char *IDs[],int *scores,char *nowString,int nowScore){
    while(idx > 0 && (scores[idx-1] > nowScore || (scores[idx-1] == nowScore && strcmp(IDs[idx-1],nowString) > 0 ) ) ){
        IDs[idx] = IDs[idx-1];
        scores[idx] = scores[idx-1];
        idx--;
    }
    IDs[idx] = nowString;
    scores[idx] = nowScore;
    return ;
}
////////////////////////////////find created ID main function//////////////////////////////////////////
void findCreatedID(const char *origin,int needNum,Bank &bank){    //窮舉已存在帳號,找出score最小ID needNum個
    char *IDs[needNum];
    int scores[needNum];
    int num = 0;
    std::list<Account *>::iterator i;
    for(i = bank.get_begin(); i != bank.get_end(); ++i){
        char *nowString = bank.get_element(i)->ID;
        int nowScore = getScore(origin,nowString);
        //printf("%s---*",nowString);
        if(num < needNum){
            insertion(num,IDs,scores,nowString,nowScore);
            num++;
        }
        else if(scores[num-1] > nowScore || (scores[num-1] == nowScore && strcmp(IDs[num-1] , nowString) > 0)){
            insertion(num-1,IDs,scores,nowString,nowScore);
        }
    }
    for(int i = 0;i < num;i++){
        printf("%s",IDs[i]);
        if(i < num - 1)
            putchar(',');
    }
    putchar('\n');
    return ;
}
//////////////////////////////////////////////////////////////////////////////////////////////
