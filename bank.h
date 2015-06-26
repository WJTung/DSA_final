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
#define LETTER_NUM 62
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
    Account(const char* const id,string hash,int m = 0):hash_password(hash),money(m){
        ID = new char[strlen(id)+1];
        strcpy(ID,id);
        Account_history = new std::vector<History*>;
    }
    Account(){
        ID = nullptr;
    }
    ~Account(){
        delete ID;
        delete Account_history;
    }
};
struct Node{
    Account *current_account;
    Node* parent;
    int num_children;
    Node* children[LETTER_NUM];
    Node(Account *account = nullptr, Node* p = nullptr){ 
        current_account = account;
        parent = p;
        num_children = 0;
        for(int i = 0;i < LETTER_NUM;i++)
            children[i] = nullptr;
    }
    ~Node(){
        delete current_account;
        for(int i = 0;i < LETTER_NUM;i++)
            delete children[i];
    }
};
class Trie{
    private:
        Node *root;
    public:
        Trie(){
            root = new Node(nullptr,nullptr);
        }
        Trie(Node* r){
            root = r;
        }
        ~Trie(){
            delete root;
        }
        Node* const getRoot(void){
            return root;
        }
        Node* findNode(char const *);
        Account* find(char const *);
        Account* insert(char const *,string,int);
};
class Bank{
    private:
        Trie Account_trie;
        std::vector<History*> Transfer_history;
        Account *last_login;
        int transferred_number;
    public:
        Bank(){transferred_number = 0;}
        bool existed(char* const);
        int login(char* const , const string&);
        int create(char const *, const string&);
        int deleting(char const *, const string&);
        pair<int, int> merge(char* const , const string& , char* const , const string&);
        int deposit(const int&);
        pair<int, int> withdraw(const int&);
        pair<int, int> transfer(char* const , const int&);
        void find_and_print(const char* const);
        int search_and_print(const char* const);
        friend void findCreatedID(const char* const, int, Bank&);
};
void findUncreatedID(const char* const, int, Bank&);
//void findCreatedID(char*, int, Bank&);
int match(const char* const,char* const);
