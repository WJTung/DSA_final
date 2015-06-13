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
    long long money;
};
struct Account{
    string ID;
    string hash_password; 
    long long money;
    std::vector<History *> Account_history;
    Account(string id,string hash,long long m = 0):ID(id),hash_password(hash),money(m){}
};

class Bank{
    private:
        Account *iterator;
    public:
        Bank(){}
        bool existed(const string&);
        int login(const string& , const string&);
        int create(const string& , const string&);
        int deleting(const string& , const string&);
        pair<int, long long> merge(const string& , const string& , const string& , const string&);
        long long deposit(const long long&);
        pair<int, long long> withdraw(const long long&);
        pair<int, long long> transfer(const string& , const long long&);
        void find_and_print(const string&);
        int search_and_print(const string&);
        void setBeginIter(void);
        bool isEndIter(void);
        void nextIter(void);
        const Account* getIter(void);
};
void findUncreatedID(string&, int, Bank&);
