#include "md5.h"
#include "bank.h"

int char2Index(char c){
    if(isdigit(c))
        return c - '0';
    else if(isupper(c))
        return 10 + c - 'A';
    else
        return 36 + c - 'a';
}
Node* Trie::findNode(char const * ID){                   //是否有此ID的trie節點(不一定有account)
    Node *current = root;
    char const * current_char = ID;
    while((*current_char) != '\0')
    {
        Node* nxt = current->children[char2Index(*current_char)];
        if(nxt == nullptr)
            return nullptr;
        else
            current = nxt;
        current_char++;
    }
    return current;
}
Account* Trie::find(char const * ID)
{
    Node* nowNode = findNode(ID);
    if(nowNode == nullptr)
        return nullptr;
    else
        return nowNode->current_account;
}
bool Trie::insert(char const * ID,string hash_password,int money)       //false: ID_EXISTED
{
    Node* current = root;
    char const * current_char = ID;
#define nxt current->children[char2Index(*current_char)]
    while((*current_char) != '\0')
    {
        if(nxt == nullptr){
            nxt = new Node(nullptr,current);
            (current->num_children)++;
        }
        current = nxt;
        current_char++;
    }
#undef nxt
    if(current->current_account == nullptr){
        current->current_account = new Account(ID,hash_password,money);
        return true;
    }
    else
        return false;
}

bool Bank::existed(char* const ID)
{
    return (Account_trie.find(ID) != nullptr);
}
int Bank::login(char* const ID, const string &password)
{
    Account *i = Account_trie.find(ID);
    string hash_password = md5(password);
    if(i == nullptr)
        return ID_NOT_FOUND;
    else if(hash_password != i->hash_password)
        return WRONG_PS;
    else
    {
        last_login = i;
        return SUCCESS;
    }
}
int Bank::create(char const * ID, const string &password)
{
    if(Account_trie.insert(ID,md5(password),0))
        return SUCCESS;
    else
        return ID_EXIST;
}
int Bank::deleting(char const * ID, const string &password)
{
    Node *i = Account_trie.findNode(ID);
    string hash_password = md5(password);
    if(i == nullptr || i->current_account == nullptr)
        return ID_NOT_FOUND;
    else if(hash_password != i->current_account->hash_password)
        return WRONG_PS;
    else
    {
        Node *current = i, *prev = current -> parent;
        delete current->current_account;
        current->current_account = nullptr;
        ID += strlen(ID) - 1;
        while (prev != nullptr && current -> num_children == 0 && current -> current_account == nullptr){
            delete current;
            prev -> num_children--;
            prev -> children[char2Index(*ID)] = nullptr;
            current = prev;
            prev = prev -> parent;
            ID--;
        }
        
        return SUCCESS;
    }
}
pair<int, int> Bank::merge(char* const ID1, const string &password1, char* const ID2, const string &password2)
{
    Account *i1 = Account_trie.find(ID1);
    Node* node_i2 = Account_trie.findNode(ID2);
    Account *i2 = (node_i2 == nullptr)?nullptr:node_i2->current_account;
    string hash_password1 = md5(password1);
    string hash_password2 = md5(password2);
    std::pair<int, int> ans;
    if(i1 == nullptr)
        ans = std::make_pair (ID_NOT_FOUND, 0);
    else if(i2 == nullptr)
        ans = std::make_pair (SUCCESS, ID_NOT_FOUND);
    else if(hash_password1 != i1->hash_password)
        ans = std::make_pair (WRONG_PS, 0);
    else if(hash_password2 != i2->hash_password)
        ans = std::make_pair (SUCCESS, WRONG_PS);
    else
    {
        int i, hi1=0, hi2=0;
        i1->money += i2->money;
        int h1 = i1->Account_history->size(); 
        int h2 = i2->Account_history->size(); 
        std::vector<History *> *new_Account_history = new vector<History *>;
        while(hi1 != h1 && hi2 != h2)
        {
            if(i1->Account_history->at(hi1)->transfer_time <= i2->Account_history->at(hi2)->transfer_time)
            {
                new_Account_history->push_back(i1->Account_history->at(hi1));
                hi1++;
            }
            else
            {
                new_Account_history->push_back(i2->Account_history->at(hi2));
                hi2++;
            }
        }
        while(hi1 != h1)
        {
            new_Account_history->push_back(i1->Account_history->at(hi1));
            hi1++;
        }
        while(hi2 != h2)
        {
            new_Account_history->push_back(i2->Account_history->at(hi2));
            hi2++;
        }
        for(i = 0; i < h2; i++)
        {
            if(strcmp(i2->Account_history->at(i)->give_ID , ID2) == 0)
                strcpy(i2->Account_history->at(i)->give_ID , ID1);
            if(strcmp(i2->Account_history->at(i)->get_ID , ID2) == 0)
                strcpy(i2->Account_history->at(i)->get_ID , ID1);
        }
        delete i1->Account_history;
        //delete i2->Account_history;
        i1->Account_history = new_Account_history;
        delete i2;
        node_i2->current_account = nullptr;
        ans = std::make_pair (SUCCESS, i1->money);
    }
    return ans;
}
int Bank::deposit(const int &money)
{
    last_login->money += money;
    return last_login->money ;
}
pair<int, int> Bank::withdraw(const int &money)
{
    if(last_login->money >= money)
    {
        last_login->money -= money;
        std::pair<int, int> ans = std::make_pair(SUCCESS, last_login -> money);
        return ans;
    }
    else
    {
        std::pair<int, int> ans = std::make_pair(FAIL, last_login -> money);
        return ans;
    }
}
pair<int, int> Bank::transfer(char* const ID, const int &money)
{
    Account *i = Account_trie.find(ID);
    std::pair<int, int> ans;
    if(i == nullptr)
        ans = std::make_pair(ID_NOT_FOUND, 0);
    else if(last_login -> money < money)
        ans = std::make_pair(FAIL, (last_login -> money));
    else
    {
        last_login -> money -= money;
        i -> money  += money;
        History *new_history = new History((last_login -> ID), ID, money, transferred_number);
        Transfer_history.push_back(new_history);
        transferred_number++;
        last_login -> Account_history -> push_back(new_history);
        i -> Account_history -> push_back(new_history);
        ans = std::make_pair(SUCCESS, (last_login -> money));
    }
    return ans;
}
void Bank::find_and_print(const char *regexp)
{
    find_match_ID(Account_trie.getRoot(), regexp);
    putchar('\n');
}
int Bank::search_and_print(const char* const ID)
{
    vector<History *> *nowHistory = last_login->Account_history;
    bool noRecord = true;
    for(unsigned int i = 0;i < nowHistory->size();i++){
        if(strcmp(nowHistory->at(i)->give_ID , ID) == 0){
            printf("From %s %d\n",ID,nowHistory->at(i)->money);
            noRecord = false;
        }
        else if(strcmp(nowHistory->at(i)->get_ID , ID) == 0){
            printf("To %s %d\n",ID,nowHistory->at(i)->money);
            noRecord = false;
        }
    }
    if(noRecord)
        return NO_RECORD;
    return SUCCESS;
}
