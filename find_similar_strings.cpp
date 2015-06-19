#include<iostream>
#include<stdio.h>
#include<string.h>
#include "bank.h"

using namespace std;
#define MAX_SCORE 10000
#define WORD_LEN 62
#define sum1toN(n) ((n)*(n+1)/2)
#define ABS(x) ((x>0)?x:-x)

map<string,bool> account;
char word[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};   //帳號可能會出現的字元

int findMinLen(int len,int score){                      //在差值=score下，字串最短可能長度
    for(int i = 1;i < len;i++)
        if(sum1toN(len - i) <= score)
            return i;
    return len;
}
int getNewLen(int originLen,int score){                 //測試是否有後綴長度剛好等於score
    int i;
    for(i = 1;sum1toN(i) < score;i++);
    if(sum1toN(i) == score)
        return originLen + i;
    else 
        return -1;
}
void add(string &now,int originLen,int nowLen){             
    for(int idx = nowLen - 1; idx > originLen; idx--){
        if(now[idx] != word[WORD_LEN - 1]){
            now[idx]++;
            return ;
        }
        else
            now[idx] = word[0];
    }
    now[originLen]++;
    if(now[originLen] > word[WORD_LEN - 1])
        now[originLen] = word[0];
    return;
}

//字串>原長時，要窮舉字串尾的排組   
int printAllPostfix(string &origin,int originLen,string &now,int score,int needed,Bank &bank){
    int finded = 0,newLen = getNewLen(originLen,score);
    //cout<<" newLen: "<<newLen<<' ';
    if(newLen == -1) return 0;
    else{
        for(int i = 0;i < newLen - originLen;i++)
            now += word[0];
    }
    for(string prv = now; prv <= now && finded < needed; prv = now,add(now,originLen,newLen)){
        if(!bank.existed(now)){
            cout<<now;
            finded++;
            if(finded < needed)
                cout<<',';
        }
    }
    now = now.substr(0,originLen);
    return finded;
}
//output 同score字串，字典序由小到大
int printSameScoreString(string &origin,int originLen,string &now,int nowIdx,int score,int needed,int changeNum,Bank &bank){
    int finded = 0;
    if(nowIdx == originLen){            //超過原長，如果某長度可以符合score，直接字典序輸出
        finded += printAllPostfix(origin,originLen,now,score,needed - finded,bank);
        return finded;
    }
    if(nowIdx != 0 && sum1toN(originLen - nowIdx) - changeNum * (originLen - nowIdx) == score){ //刪掉idx後string剛好符合score
        string shortNow = now.substr(0,nowIdx);
        if(!bank.existed(shortNow)){
            cout<<shortNow;
            finded++;
            if(finded < needed)
                cout<<',';
        }
        if(finded == needed)
            return finded;
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
                cout<<now;
                finded++;
                if(finded < needed)
                    cout<<',';
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
                cout<<now;
                finded++;
                if(finded < needed)
                    cout<<',';
            }
        }
    }
    now[nowIdx] = origin[nowIdx];
    return finded;
}
/////////////////////////////////find uncreated ID main function//////////////////////////////////
void findUncreatedID(string &origin,int needNum,Bank &bank){                        //窮舉所有可能score,find uncreated ID
    int finded = 0;
    for(int score = 1;finded < needNum && score < MAX_SCORE; score++){
        string now = origin;
        finded += printSameScoreString(origin,origin.length(),now,0,score,needNum - finded,0,bank);
    }
    putchar('\n');
/*
    if(finded == needNum)
        printf("successfully find %d strings\n",needNum);
    else
        printf("only find %d strings\n",finded);
*/
    return ;
}

////////////////////////////////////////find created ID////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int getScore(string &sa,string &sb){
    int na = sa.length(),nb = sb.length();
    int score = sum1toN(ABS(na-nb));
    int minLen = min(na,nb);
    for(int i = 0;i < minLen;i++){
        if(sa[i] != sb[i])
            score += minLen - i;
    }
    return score;
}
void insertion(int idx,string *IDs,int *scores){
    if(idx == 0) return ;
    if(scores[idx-1] > scores[idx] || (scores[idx-1] == scores[idx] && IDs[idx-1] > IDs[idx])){
        swap(scores[idx-1],scores[idx]);
        swap(IDs[idx-1],IDs[idx]);
        insertion(idx-1,IDs,scores);
    }
    return ;
}
////////////////////////////////find created ID main function//////////////////////////////////////////
void findCreatedID(string &origin,int needNum,Bank &bank){                          //窮舉已存在帳號,找出score最小ID needNum個
    string IDs[needNum];
    int scores[needNum];
    int num = 0;
    for(bank.setBeginIter(); bank.isEndIter() == false; bank.nextIter()){
        string nowString = bank.getIter()->ID;
        int nowScore = getScore(origin,nowString);
        if(num < needNum){
            IDs[num] = nowString;
            scores[num] = nowScore;
            insertion(num,IDs,scores);
            num++;
        }
        else if(scores[num-1] > nowScore || (scores[num-1] == nowScore && IDs[num-1] > nowString)){
            IDs[num-1] = nowString;
            scores[num-1] = nowScore;
            insertion(num-1,IDs,scores);
        }
    }
    for(int i = 0;i < num;i++){
        cout<<IDs[i];
        if(i < num - 1)
            putchar(',');
    }
    putchar('\n');
    return ;
}
//////////////////////////////////////////////////////////////////////////////////////////////
