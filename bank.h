#include <vector>
#include <utility>
#include <string>
#include <string.h>
#include <unordered_map>
#include <algorithm>
#include "MurmurHash3.h"
#define ID_NOT_FOUND -1
#define WRONG_PS -2
#define SUCCESS -3
#define ID_EXIST -4
#define FAIL -5
#define NO_RECORD -6
#define MAX_STRLEN 200
#define Map_Size 200000
using namespace std;
struct History{
    char* give_ID;
    char* get_ID;
    int money;
    int transfer_time;
    History(const char *give_id,const char *get_id, int amount, int time):money(amount),transfer_time(time){
        give_ID = new char[MAX_STRLEN];
        strcpy(give_ID,give_id);
        get_ID = new char[MAX_STRLEN];
        strcpy(get_ID,get_id);
    }
    ~History(){
        delete give_ID;
        delete get_ID;
    }
};
struct Account{
    char* ID;
    string hash_password; 
    int money;
    std::vector<History *> *Account_history;
    Account(const char *id,string hash,int m = 0):hash_password(hash),money(m){
        ID = new char[strlen(id)+1];
        strcpy(ID,id);
    }
    Account(){
        ID = NULL;
    }
    ~Account(){
        //delete ID;
    }
};
struct str_equal
{
    size_t operator() (const char *a, const char *b) const
    {   
        return (strcmp(a, b) == 0);
    }
};
struct str_hash
{
    size_t operator() (const char *str) const
    {
        return murmur(str);
    }
};
class Bank{
    private:
        Account *iterator;
        std::unordered_map<const char *, Account, str_hash, str_equal>::iterator mapIter;
        std::unordered_map<const char *, Account, str_hash, str_equal> Account_map;
        std::vector<History*> Transfer_history;
        std::unordered_map<const char *, Account, str_hash, str_equal>::iterator last_login;
        int transferred_number;
    public:
        Bank(){transferred_number = 0, Account_map.reserve(Map_Size);}
        bool existed(const char *);
        int login(const char *, const string&);
        int create(const char *, const string&);
        int deleting(const char *, const string&);
        pair<int, int> merge(const char * , const string& , const char *, const string&);
        int deposit(const int&);
        pair<int, int> withdraw(const int&);
        pair<int, int> transfer(const char *, const int&);
        void find_and_print(const char *);
        int search_and_print(const char *);
        void setBeginIter(void);
        bool isEndIter(void);
        void nextIter(void);
        const Account* getIter(void);
};
void findUncreatedID(const char *, int, Bank&);
void findCreatedID(const char *, int, Bank&);
int match(const char *, const char *);
