#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<map>

using namespace std;
#define MAX 10000
#define sum1toN(n) ((n)*(n+1)/2)

map<string,bool> account;
char word[] = {"abcdefg"};								//帳號可能會出現的字元

int findMinLen(int len,int score){						//在差值=score下，字串最短可能長度
	for(int i = 1;i < len;i++)
		if(sum1toN(len - i) <= score)
			return i;
	return len;
}
void add(string &now,int originLen,int nowLen){			//字串>原長時，要窮舉字串尾的排組	
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

int printAllPostfix(string &origin,string &now,int needed){											//如果score符合 且 now長 > 原字串長，輸出全部可能後綴
	int finded = 0,originLen = origin.length(),nowLen = now.length();
	for(string prv = now; prv <= now && finded < needed; prv = now,add(now,originLen,nowLen)){
		if(!account[now]){
			cout<<now<<' ';
			finded++;
		}
	}
	return finded;
}
int printLenString(string &origin,string &now,int newLen,int score,int idx,int needed){				//固定score和長度，修改字元
	if(sum1toN(newLen - idx) < score)
		return 0;
	int finded = 0;
	if(score - (newLen - idx) == 0){
		unsigned wordIdx;
		for(wordIdx = 0;wordIdx < strlen(word) && word[wordIdx] < origin[idx] && finded < needed;wordIdx++){
			now[idx] = word[wordIdx];
			if(now.length() > origin.length()){
				finded += printAllPostfix(origin,now,needed - finded);
			}
			else if(!account[now]){
				cout<<now<<' ';
				finded++;
			}
		}
		if(finded < needed && idx+1 < newLen){
			now[idx] = origin[idx];
			finded += printLenString(origin,now,newLen,score,idx+1,needed - finded);
		}
		for(wordIdx = wordIdx + 1;wordIdx < strlen(word) && finded < needed; wordIdx++){
			now[idx] = word[wordIdx];
			if(now.length() > origin.length()){
				finded += printAllPostfix(origin,now,needed - finded);
			}
			else if(!account[now]){
				cout<<now<<' ';
				finded++;
			}
		}
		now[idx] = origin[idx];
		return finded;
	}
	else if(score - (newLen - idx) > 0){
		unsigned wordIdx;
		for(wordIdx = 0;wordIdx < strlen(word) && word[wordIdx] < origin[idx] && finded < needed;wordIdx++){
			now[idx] = word[wordIdx];
			finded += printLenString(origin,now,newLen,score - (newLen - idx),idx+1,needed - finded);
		}
		if(finded < needed){
			now[idx] = origin[idx];
			finded += printLenString(origin,now,newLen,score,idx+1,needed - finded);
		}
		for(wordIdx = wordIdx + 1;wordIdx < strlen(word) && finded < needed; wordIdx++){
			now[idx] = word[wordIdx];
			finded += printLenString(origin,now,newLen,score - (newLen - idx),idx+1,needed - finded);
		}
		now[idx] = origin[idx];
		return finded;
	}
	else{
		finded += printLenString(origin,now,newLen,score,idx+1,needed - finded);
		return finded;
	}
}
int printScoreString(string &origin,int score,int needed){									//固定score，修改長度
	int len = origin.length();
	int newLen = findMinLen(len,score), deltaL = len - newLen;
	int finded = 0;
	if(score - sum1toN(len - newLen) == 0){
		string now = origin.substr(0,newLen);
		if(!account[now]){
			cout<<origin.substr(0,newLen)<<' ';
			finded++;
		}
		newLen++;
	}
	for(;finded < needed && newLen <= len;newLen++){
		string now = origin.substr(0,newLen);
		finded += printLenString(origin,now,newLen,score - sum1toN(len - newLen),0,needed - finded);
	}
	string now,es;
	for(now = origin + word[0],es = word[strlen(word) - 1];finded < needed && newLen < len + deltaL;newLen++,now += word[0],es += word[strlen(word) - 1]){
		//for(; now.substr(len,newLen - len) <= es && finded < needed; add(now,len,newLen)){
			finded += printLenString(origin,now,len,score - sum1toN(newLen - len),0,needed - finded);
		//}
		//now[len] = word[0];
	}
	if(finded < needed && score - sum1toN(newLen - len) == 0){
		for(string prv = now; prv <= now && finded < needed; prv = now,add(now,len,newLen)){
			if(!account[now]){
				cout<<now<<' ';
				finded++;
			}
		}
	}
	else if(finded < needed){
		//for(; now.substr(len,newLen - len) <= es && finded < needed; add(now,len,newLen)){
			finded += printLenString(origin,now,len,score - sum1toN(newLen - len),0,needed - finded);
		//}
	}
	return finded;
}
void findUncreatedID(string &origin,int needNum){									//窮舉所有可能score
	int finded = 0;
	for(int score = 1;finded < needNum && score < MAX; score++){
		finded += printScoreString(origin,score,needNum - finded);
	}
	putchar('\n');
	if(finded == needNum)
		printf("successfully find %d strings\n",needNum);
	else
		printf("only find %d strings\n",finded);
	return ;
}

int main(){
	string s;
	while(cin>>s){
		if(account[s] == false){
			puts("create account");
			account[s] = true;
		}
		else{
			printf("recommended account: ");
			findUncreatedID(s,10);
		}
	}
	return 0;
}
