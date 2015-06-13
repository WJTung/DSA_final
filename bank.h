#include <vector>
#include <utility>
#define ID_NOT_FOUND -1
#define WRONG_PS -2
#define SUCCESS -3
#define ID_EXIST -4
#define FAIL -5
#define NO_RECORD -6
struct History{
    string give_ID;
    string get_ID;
    long long money;
}
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
        bool create(string ,string);
        bool create(char* ,char*);
        int deleting(string,string);
        int deleting(char*,char*);
        pair<int, long long> merge(string,string,string,string);
        pair<int, long long> merge(char*,char*,char*,char*);
        bool deposit(long long);
        bool withdraw(long long);
        int transfer(string,long long);
        int transfer(char*,long long);
        void find_and_print(string);
        void find_and_print(char*);
        int  search_and_print(string);
        int search_and_print(char*);
};
