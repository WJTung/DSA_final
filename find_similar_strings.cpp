#include<iostream>
#include<stdio.h>
#include<string.h>
#include "bank.h"

using namespace std;
#define MAX_SCORE 10000
#define sum1toN(n) ((n)*(n+1)/2)
#define ABS(x) ((x>0)?x:-x)

map<string,bool> account;
char word[] = {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};   //帳號可能會出現的字元

int findMinLen(int len,int score){                      //在差值=score下，字串最短可能長度
    for(int i = 1;i < len;i++)
        if(sum1toN(len - i) <= score)
            return i;
    return len;
}
void add(string &now,int originLen,int nowLen){         //字串>原長時，要窮舉字串尾的排組   
    int len = strlen(word);
    for(int idx = nowLen - 1; idx > originLen; idx--){
        if(now[idx] != word[len - 1]){
            now[idx]++;
            return ;
        }
        else
            now[idx] = word[0];
    }
    now[originLen]++;
    if(now[originLen] > word[len - 1])
        now[originLen] = word[0];
    return;
}

int printAllPostfix(string &origin,string &now,int needed,Bank &bank){  //如果score符合 且 now長 > 原字串長，輸出全部可能後綴
    int finded = 0,originLen = origin.length(),nowLen = now.length();
    for(string prv = now; prv <= now && finded < needed; prv = now,add(now,originLen,nowLen)){
        if(!bank.existed(now)){
            cout<<now;
            finded++;
            if(finded < needed)
                cout<<',';
        }
    }
    return finded;
}
int printSameLenString(string &origin,string &now,int newLen,int score,int idx,int needed,Bank &bank){//固定score和長度，修改字元
    if(sum1toN(newLen - idx) < score)
        return 0;
    int finded = 0;
    if(score - (newLen - idx) == 0){
        unsigned wordIdx;
        for(wordIdx = 0;wordIdx < strlen(word) && word[wordIdx] < origin[idx] && finded < needed;wordIdx++){
            now[idx] = word[wordIdx];
            if(now.length() > origin.length()){
                finded += printAllPostfix(origin,now,needed - finded,bank);
            }
            else if(!bank.existed(now)){
                cout<<now;
                finded++;
                if(finded < needed)
                    cout<<',';
            }
        }
        if(finded < needed && idx+1 < newLen){
            now[idx] = origin[idx];
            finded += printSameLenString(origin,now,newLen,score,idx+1,needed - finded,bank);
        }
        for(wordIdx = wordIdx + 1;wordIdx < strlen(word) && finded < needed; wordIdx++){
            now[idx] = word[wordIdx];
            if(now.length() > origin.length()){
                finded += printAllPostfix(origin,now,needed - finded,bank);
            }
            else if(!bank.existed(now)){
                cout<<now;
                finded++;
                if(finded < needed)
                    cout<<',';
            }
        }
        now[idx] = origin[idx];
        return finded;
    }
    else if(score - (newLen - idx) > 0){
        unsigned wordIdx;
        for(wordIdx = 0;wordIdx < strlen(word) && word[wordIdx] < origin[idx] && finded < needed;wordIdx++){
            now[idx] = word[wordIdx];
            finded += printSameLenString(origin,now,newLen,score - (newLen - idx),idx+1,needed - finded,bank);
        }
        if(finded < needed){
            now[idx] = origin[idx];
            finded += printSameLenString(origin,now,newLen,score,idx+1,needed - finded,bank);
        }
        for(wordIdx = wordIdx + 1;wordIdx < strlen(word) && finded < needed; wordIdx++){
            now[idx] = word[wordIdx];
            finded += printSameLenString(origin,now,newLen,score - (newLen - idx),idx+1,needed - finded,bank);
        }
        now[idx] = origin[idx];
        return finded;
    }
    else{
        finded += printSameLenString(origin,now,newLen,score,idx+1,needed - finded,bank);
        return finded;
    }
}
int printSameScoreString(string &origin,int score,int needed,Bank &bank){               //固定score，修改長度
    int len = origin.length();
    int newLen = findMinLen(len,score), deltaL = len - newLen;
    int finded = 0;
    if(score - sum1toN(len - newLen) == 0){
        string now = origin.substr(0,newLen);
        if(!bank.existed(now)){
            cout<<origin.substr(0,newLen)<<' ';
            finded++;
        }
        newLen++;
    }
    for(;finded < needed && newLen <= len;newLen++){
        string now = origin.substr(0,newLen);
        finded += printSameLenString(origin,now,newLen,score - sum1toN(len - newLen),0,needed - finded,bank);
    }
    string now,es;
    for(now = origin + word[0],es = word[strlen(word) - 1];
        finded < needed && newLen < len + deltaL;
        newLen++,now += word[0],es += word[strlen(word) - 1]){
        //for(; now.substr(len,newLen - len) <= es && finded < needed; add(now,len,newLen)){
            finded += printSameLenString(origin,now,len,score - sum1toN(newLen - len),0,needed - finded,bank);
        //}
        //now[len] = word[0];
    }
    if(finded < needed && score - sum1toN(newLen - len) == 0){
        for(string prv = now; prv <= now && finded < needed; prv = now,add(now,len,newLen)){
            if(!bank.existed(now)){
                cout<<now;
                finded++;
                if(finded < needed)
                    cout<<',';
            }
        }
    }
    else if(finded < needed){
        //for(; now.substr(len,newLen - len) <= es && finded < needed; add(now,len,newLen)){
            finded += printSameLenString(origin,now,len,score - sum1toN(newLen - len),0,needed - finded,bank);
        //}
    }
    return finded;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void findUncreatedID(string &origin,int needNum,Bank &bank){                        //窮舉所有可能score,find uncreated ID
    int finded = 0;
    for(int score = 1;finded < needNum && score < MAX_SCORE; score++){
        finded += printSameScoreString(origin,score,needNum - finded,bank);
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
            scores[num] = nowScore;
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
