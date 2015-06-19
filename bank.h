#include <vector>
#include <utility>
#include <string>
#define ID_NOT_FOUND -1
#define WRONG_PS -2
#define SUCCESS -3
#define ID_EXIST -4
#define FAIL -5
#define NO_RECORD -6
using namespace std;
struct History{
    string give_ID;
    string get_ID;
    int money;
    int transfer_time;
    History(string give_id, string get_id, int amount, int time):give_ID(give_id), get_ID(get_id), money(amount), 
    transfer_time(time){}
};
struct Account{
    string ID;
    string hash_password; 
    int money;
    std::vector<History *> *Account_history;
    Account(string id,string hash,int m = 0):ID(id),hash_password(hash),money(m){}
    Account(){}
};

class Bank{
    private:
        Account *iterator;
        std::vector<History> Transfer_history;
        int transferred_number;
    public:
        Bank(){transferred_number = 0;}
        bool existed(const string&);
        int login(const string& , const string&);
        int create(const string& , const string&);
        int deleting(const string& , const string&);
        pair<int, int> merge(const string& , const string& , const string& , const string&);
        int deposit(const int&);
        pair<int, int> withdraw(const int&);
        pair<int, int> transfer(const string& , const int&);
        void find_and_print(const string&);
        int search_and_print(const string&);
        void setBeginIter(void);
        bool isEndIter(void);
        void nextIter(void);
        const Account* getIter(void);
};
void findUncreatedID(string&, int, Bank&);
void findCreatedID(string&, int, Bank&);
int match(const string &,const string &, int, int, const int, const int);
