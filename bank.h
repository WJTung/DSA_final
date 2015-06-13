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

	public:
		Bank(){}
        bool existed(string);
        bool existed(char*);
        int login(string,string);
        int login(char*,char*);
        int create(string ,string);
        int create(char* ,char*);
        int deleting(string,string);
        int deleting(char*,char*);
        pair<int, long long> merge(string,string,string,string);
        pair<int, long long> merge(char*,char*,char*,char*);
        int deposit(long long);
        pair<int, long long> withdraw(long long);
        pair<int, long long> transfer(string,long long);
        pair<int, long long> transfer(char*,long long);
        void find_and_print(string);
        void find_and_print(char*);
        int search_and_print(string);
        int search_and_print(char*);
};
void findUncreatedID(string&, int, Bank&);
