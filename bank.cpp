#include "md5.h"
#include "bank.h"
#include <stdio.h>
bool Bank::existed(const char *ID)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i = Account_map.find(ID);
    return (i != Account_map.end());
}
int Bank::login(const char *ID, const string &password)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i = Account_map.find(ID);
    string hash_password = md5(password);
    if(i == Account_map.end())
        return ID_NOT_FOUND;
    else if(hash_password != (i->second)->hash_password)
        return WRONG_PS;
    else
    {
        last_login = i;
        return SUCCESS;
    }
}
int Bank::create(const char *ID, const string &password)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i = Account_map.find(ID);
    if(i != Account_map.end())
        return ID_EXIST;
    else
    {
        Account *new_account = new Account(ID, md5(password), 0);
        new_account->Account_history = new vector<History *>;
        char *key = new char[strlen(ID)+1];
        strcpy(key,ID);
        Account_map[key] = new_account;
        Account_list.push_back(new_account);
        std::list<Account *>::iterator last = Account_list.end();
        --last;
        new_account->Account_i = last;
        return SUCCESS;
    }
}
int Bank::deleting(const char *ID, const string &password)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i = Account_map.find(ID);
    string hash_password = md5(password);
    if(i == Account_map.end())
        return ID_NOT_FOUND;
    else if(hash_password != (i->second)->hash_password)
        return WRONG_PS;
    else
    {
        Account_list.erase((i->second)->Account_i);
        Account_map.erase(i);
        return SUCCESS;
    }
}
pair<int, int> Bank::merge(const char *ID1, const string &password1, const char *ID2, const string &password2)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i1 = Account_map.find(ID1);
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i2 = Account_map.find(ID2);
    string hash_password1 = md5(password1);
    string hash_password2 = md5(password2);
    std::pair<int, int> ans;
    if(i1 == Account_map.end())
        ans = std::make_pair (ID_NOT_FOUND, 0);
    else if(i2 == Account_map.end())
        ans = std::make_pair (SUCCESS, ID_NOT_FOUND);
    else if(hash_password1 != (i1->second)->hash_password)
        ans = std::make_pair (WRONG_PS, 0);
    else if(hash_password2 != (i2->second)->hash_password)
        ans = std::make_pair (SUCCESS, WRONG_PS);
    else
    {
        int i, hi1=0, hi2=0;
        (i1->second)->money += (i2->second)->money;
        int h1 = (i1->second)->Account_history->size(); 
        int h2 = (i2->second)->Account_history->size(); 
        std::vector<History *> *new_Account_history = new vector<History *>;
        while(hi1 != h1 && hi2 != h2)
        {
            if((i1->second)->Account_history->at(hi1)->transfer_time <= (i2->second)->Account_history->at(hi2)->transfer_time)
            {
                new_Account_history->push_back((i1->second)->Account_history->at(hi1));
                hi1++;
            }
            else
            {
                new_Account_history->push_back((i2->second)->Account_history->at(hi2));
                hi2++;
            }
        }
        while(hi1 != h1)
        {
            new_Account_history->push_back((i1->second)->Account_history->at(hi1));
            hi1++;
        }
        while(hi2 != h2)
        {
            new_Account_history->push_back((i2->second)->Account_history->at(hi2));
            hi2++;
        }
        for(i = 0; i < h2; i++)
        {
            if(strcmp((i2->second)->Account_history->at(i)->give_ID , ID2) == 0)
                strcpy((i2->second)->Account_history->at(i)->give_ID , ID1);
            if(strcmp((i2->second)->Account_history->at(i)->get_ID , ID2) == 0)
                strcpy((i2->second)->Account_history->at(i)->get_ID , ID1);
        }
        (i1->second)->Account_history = new_Account_history;
        Account_list.erase((i2->second)->Account_i);
        Account_map.erase(i2);
        ans = std::make_pair (SUCCESS, (i1->second)->money);
    }
    return ans;
}
int Bank::deposit(const int &money)
{
    (last_login->second)->money += money;
    return (last_login->second)->money ;
}
pair<int, int> Bank::withdraw(const int &money)
{
    if((last_login->second)->money >= money)
    {
        (last_login->second)->money -= money;
        std::pair<int, int> ans = std::make_pair(SUCCESS, (last_login->second)->money);
        return ans;
    }
    else
    {
        std::pair<int, int> ans = std::make_pair(FAIL, (last_login->second)->money);
        return ans;
    }
}
pair<int, int> Bank::transfer(const char *ID, const int &money)
{
    std::unordered_map<const char *, Account *, str_hash, str_equal>::iterator i = Account_map.find(ID);
    std::pair<int, int> ans;
    if(i == Account_map.end())
        ans = std::make_pair(ID_NOT_FOUND, 0);
    else if((last_login->second)->money < money)
        ans = std::make_pair(FAIL, (last_login->second)->money);
    else
    {
        (last_login->second)->money -= money;
        (i->second)->money  += money;
        History *tmp = new History((last_login->first), ID, money, transferred_number);
        Transfer_history.push_back(tmp);
        transferred_number++;
        History *new_history = Transfer_history.at(Transfer_history.size() - 1);
        (last_login->second)->Account_history->push_back(new_history);
        (i->second)->Account_history->push_back(new_history);
        ans = std::make_pair(SUCCESS, (last_login->second)->money);
    }
    return ans;
}
bool str_cmp(const char *a, const char *b)
{
    return (strcmp(a, b) == -1);
}
void Bank::find_and_print(const char *regexp)
{
    std::vector<const char *> match_ID;
    std::list<Account *>::iterator i;
    for(i = Account_list.begin(); i != Account_list.end(); ++i)
    {
        const char *ID = (*i)->ID;
        if(match(regexp, ID))
            match_ID.push_back(ID);
    }
    int ID_num = match_ID.size(); 
    if(ID_num != 0)
    {
        sort(match_ID.begin(), match_ID.end(), str_cmp);
        printf("%s", match_ID[0]);
        int IDi;
        for(IDi = 1; IDi < ID_num; IDi++)
            printf(" ,%s", match_ID[IDi]);
    }
    putchar('\n');
}
int Bank::search_and_print(const char *ID)
{
    vector<History *> *nowHistory = last_login->second->Account_history;
    bool noRecord = true;
    for(unsigned int i = 0;i < nowHistory->size();i++){
        if(strcmp(nowHistory->at(i)->give_ID , ID) == 0){
            printf("From %s %d\n",ID,nowHistory->at(i)->money);
            noRecord = false;
        }
        else if(strcmp(nowHistory->at(i)->get_ID , ID) == 0){
            printf("To %s %d\n",ID,nowHistory->at(i)->money);
            noRecord = false;
        }
    }
    if(noRecord)
        return NO_RECORD;
    return SUCCESS;
}
std::list<Account *>::iterator Bank::get_begin()
{
    return Account_list.begin();
}
std::list<Account *>::iterator Bank::get_end()
{
    return Account_list.end();
}
Account *Bank::get_element(std::list<Account *>::iterator i)
{
    return (*i);
}
