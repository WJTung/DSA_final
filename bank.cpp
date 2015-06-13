#include "md5.h"
bool Bank::existed(string ID)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    return (i != Account_map.end());
}
int Bank::login(string ID, string password)
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
bool Bank::create(string ID, string password)
{
    map<string, Account>::iterator i = Account_map.find(ID);
    if(i != Account_map.end())
        return ID_EXIST;
    else
    {
        Account new_account(ID, password, 0);
        Account_map.insert(new_account);
        return SUCCESS;
    }
}
int Bank::deleting(string ID, string password)
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
pair<int, long long> Bank::merge(string ID1, string password1, string ID2, string password2)
{
    map<string, Account>::iterator i1 = Account_map.find(ID1);
    map<string, Account>::iterator i2 = Account_map.find(ID2);
    string hash_password1 = md5(password1);
    string hash_password2 = md5(password2);
    std::pair<int, int> ans;
    if(i1 == Account_map.end())
        ans = std::make_pair (ID_NOT_FOUND, 0);
    else if(hash_password1 != (i->second).hash_password1)
        ans = std::make_pair (WRONG_PS, 0);
    else if(i2 == Account_map.end())
        ans = std::make_pair (SUCCESS, ID_NOT_FOUND);
    else if(hash_password2 != (i->second).hash_password2)
        ans = std::make_pair (SUCCESS, WRONG_PS);
    else
    {
        (i1->second).money += (i2->second).money;
        int h1 = (*(i1->second).Account_history).size(); 
        int h2 = (*(i2->second).Account_history).size(); 
        int i, hi1, hi2;
        std::vector<History *> new_Account_history;
        while(hi1 != h1 && hi2 != h2)
        {
            if((*((i1->second).Account_history))[hi1]->transfer_time < (*((i2->second).Account_history))[h21]->transfer_time)
            {
                new_Account_history.push_back((*((i1->second).Account_history))[hi1]);
                hi1++;
            }
            else if((*((i1->second).Account_history))[hi1]->transfer_time == (*((i2->second).Account_history))[h21]->transfer_time)
            {
                new_Account_history.push_back((*((i1->second).Account_history))[hi1]);
                hi1++;
                hi2++;
            }
            else
            {
                new_Account_history.push_back((*((i1->second).Account_history))[hi2]);
                hi2++;
            }
        }
        while(hi1 != h1)
        {
            new_Account_history.push_back((*((i1->second).Account_history))[hi1]);
            hi1++;
        }
        while(hi2 != h2)
        {
            new_Account_history.push_back((*((i1->second).Account_history))[hi2]);
            hi2++;
        }
        for(i = 0; i < h2; i++)
        {
            if((*((i2->second).Account_history))[i]->give_ID == ID2)
                (*((i2->second).Account_history))[i]->give_ID = ID1;
            if((*((i2->second).Account_history))[i]->get_ID == ID2)
                (*((i2->second).Account_history))[i]->give_ID = ID1;
        }
        Account_map.erase(i2);
        ans = std::make_pair (SUCCESS, (i1->second).money);
    }
    return ans;
}
long long Bank::deposit(long long money)
{
    (last_login->second).money += money;
    return (last_login->second).money ;
}
pair<bool, long long> Bank::withdraw(long long money)
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
pair<int, long long> Bank::transfer(string ID, long long money)
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
        History((last_login->first), ID, money);
        Transfer_history.push_back(History);
        (*(last_login->second).Account_history).push_back(Transfer_history.end() - 1);
        ans = std::make_pair(SUCCESS, (last_login->second).money);
    }
}
void find_and_print(string ID)
int search_and_print(string ID)
