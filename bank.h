
struct Account{
    string ID;
    string hash_password; 
    long long money;
    Account(string id,string hash,long long m = 0):ID(id),hash_password(hash),money(m){}
};

class Bank{
	private:
		
	public:
		Bank(){}
		bool existed(string);
		bool existed(char*);
		int login(string);
		int login(char*);
        bool create(string ,string);
        bool create(char* ,char*);
        int deleting(string,string);
        int deleting(char*,char*);
        int merge(string,string,string,string);
        int merge(char*,char*,char*,char*);
        bool deposit(long long);
        bool withdraw(long long);
        int transfer(string,long long);
        int transfer(char*,long long);
        void find_and_print(string);
        void find_and_print(char*);
        bool search_and_print(string);
        bool search_and_print(char*);

};
