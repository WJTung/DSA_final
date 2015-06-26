#include <vector>
#include <utility>
#include <string>
#include <string.h>
#include <map>
#define ID_NOT_FOUND -1
#define WRONG_PS -2
#define SUCCESS -3
#define ID_EXIST -4
#define FAIL -5
#define NO_RECORD -6
#define MAX_STRLEN 200
using namespace std;
struct History{
    char* give_ID;
    char* get_ID;
    int money;
    int transfer_time;
    History(const char* const give_id,const char* const get_id, int amount, int time):money(amount),transfer_time(time){
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
        Account_history = new std::vector<History *>;
    }
    Account(){
        ID = NULL;
    }
    ~Account(){
        //delete ID;
    }
};
class Node{
    private:
        Account *current_account;
        std::map<char, Node*> children_map;
    public:
        Node() { current_account = nullptr; }
        ~Node() { children_map.clear(); }
        Node *find_child(char);
        friend class Ternary_tree;
};
class Ternary_tree{
    private:
        Node *root;
    public:
        Ternary_tree();
        ~Ternary_tree();
        Account *find(const char *);
        void insert(const char *, Account*);
        void erase(const char *);
        Node *get_root();
};
class Bank{
    private:
        Ternary_tree Account_ternary_tree;
        std::vector<History*> Transfer_history;
        Account *last_login;
        int transferred_number;
    public:
        Bank(){transferred_number = 0;}
        bool existed(const char *);
        int login(const char *, const string&);
        int create(const char *, const string&);
        int deleting(const char *, const string&);
        pair<int, int> merge(const char *, const string& , const char *, const string&);
        int deposit(const int&);
        pair<int, int> withdraw(const int&);
        pair<int, int> transfer(const char *, const int&);
        void find_and_print(const char *);
        int search_and_print(const char *);
};
void findUncreatedID(char *, int, Bank&);
void findCreatedID(char *, int, Bank&);
void find_match_ID(Node *, const char *);
