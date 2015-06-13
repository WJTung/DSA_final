#include "md5.h"
#include "bank.h"
bool Bank::existed(const string &ID)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    return (i != Account_map.end());
}
int Bank::login(const string &ID, const string &password)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    string hash_password = md5(password);
    if(i == Account_map.end())
        return ID_NOT_FOUND;
    else if(hash_password != (i->second).hash_password)
        return WRONG_PS;
    else
    {
        last_login = i;
        return SUCCESS;
    }
}
int Bank::create(const string &ID, const string &password)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    if(i != Account_map.end())
        return ID_EXIST;
    else
    {
        Account new_account(ID, md5(password), 0);
        Account_map[ID] = new_account;
        return SUCCESS;
    }
}
int Bank::deleting(const string &ID, const string &password)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    string hash_password = md5(password);
    if(i == Account_map.end())
        return ID_NOT_FOUND;
    else if(hash_password != (i->second).hash_password)
        return WRONG_PS;
    else
    {
        Account_map.erase(i);
        return SUCCESS;
    }
}
pair<int, long long> Bank::merge(const string &ID1, const string &password1, const string &ID2, const string &password2)
{
    map<string, Account>::iterator i1 = Account_map.find(ID1);
    map<string, Account>::iterator i2 = Account_map.find(ID2);
    string hash_password1 = md5(password1);
    string hash_password2 = md5(password2);
    std::pair<int, int> ans;
    if(i1 == Account_map.end())
        ans = std::make_pair (ID_NOT_FOUND, 0);
    else if(hash_password1 != (i1->second).hash_password)
        ans = std::make_pair (WRONG_PS, 0);
    else if(i2 == Account_map.end())
        ans = std::make_pair (SUCCESS, ID_NOT_FOUND);
    else if(hash_password2 != (i2->second).hash_password)
        ans = std::make_pair (SUCCESS, WRONG_PS);
    else
    {
        int i, hi1=0, hi2=0;
        (i1->second).money += (i2->second).money;
        int h1 = (i1->second).Account_history.size(); 
        int h2 = (i2->second).Account_history.size(); 
        std::vector<History *> new_Account_history;
        while(hi1 != h1 && hi2 != h2)
        {
            if(((i1->second).Account_history)[hi1]->transfer_time < ((i2->second).Account_history)[hi2]->transfer_time)
            {
                new_Account_history.push_back(((i1->second).Account_history)[hi1]);
                hi1++;
            }
            else if(((i1->second).Account_history)[hi1]->transfer_time == ((i2->second).Account_history)[hi2]->transfer_time)
            {
                new_Account_history.push_back(((i1->second).Account_history)[hi1]);
                hi1++;
                hi2++;
            }
            else
            {
                new_Account_history.push_back(((i1->second).Account_history)[hi2]);
                hi2++;
            }
        }
        while(hi1 != h1)
        {
            new_Account_history.push_back(((i1->second).Account_history)[hi1]);
            hi1++;
        }
        while(hi2 != h2)
        {
            new_Account_history.push_back(((i1->second).Account_history)[hi2]);
            hi2++;
        }
        for(i = 0; i < h2; i++)
        {
            if(((i2->second).Account_history)[i]->give_ID == ID2)
                ((i2->second).Account_history)[i]->give_ID = ID1;
            if(((i2->second).Account_history)[i]->get_ID == ID2)
                ((i2->second).Account_history)[i]->give_ID = ID1;
        }
        Account_map.erase(i2);
        ans = std::make_pair (SUCCESS, (i1->second).money);
    }
    return ans;
}
long long Bank::deposit(const long long &money)
{
    (last_login->second).money += money;
    return (last_login->second).money ;
}
pair<int, long long> Bank::withdraw(const long long &money)
{
    if((last_login->second).money >= money)
    {
        (last_login->second).money -= money;
        std::pair<bool, long long> ans = std::make_pair(SUCCESS, (last_login->second).money);
        return ans;
    }
    else
    {
        std::pair<bool, long long> ans = std::make_pair(FAIL, (last_login->second).money);
        return ans;
    }
}
pair<int, long long> Bank::transfer(const string &ID, const long long &money)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    std::pair<int, long long> ans;
    if(i == Account_map.end())
        ans = std::make_pair(ID_NOT_FOUND, 0);
    else if((last_login->second).money < money)
        ans = std::make_pair(FAIL, (last_login->second).money);
    else
    {
        (last_login->second).money -= money;
        Transfer_history.push_back(History((last_login->first), ID, money, transferred_number));
        transferred_number++;
        (last_login->second).Account_history.push_back(&Transfer_history[Transfer_history.size() - 1]);
        ans = std::make_pair(SUCCESS, (last_login->second).money);
    }
    return ans;
}
void find_and_print(const string &ID){}
int search_and_print(const string &ID)
{
    return 0;
}
void Bank::setBeginIter(void){
    mapIter = Account_map.begin();
    return ;
}
bool Bank::isEndIter(void){
    return mapIter == Account_map.end();
}
void Bank::nextIter(void){
    mapIter++;
}
const Account* Bank::getIter(void){
    return &(mapIter->second);
}
