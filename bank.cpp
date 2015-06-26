#include "md5.h"
#include "bank.h"
Node *Node::find_child(char c)
{
    std::map<char, Node*>::iterator i = children_map.find(c);
    if(i == children_map.end())
        return nullptr;
    return (i->second);
}
Ternary_tree::Ternary_tree()
{
    root = new Node();
}
Ternary_tree::~Ternary_tree()
{
    // Free memory
}
Account *Ternary_tree::find(char* const ID)
{
    Node *current = root;
    while((*ID) != '/0')
    {
        char current_char = (*ID); 
        current = current->find_child(current_char);
        if(current == nullptr)
            return nullptr;
        ID++;
    }
    return current->current_account;
}
void Ternary_tree::insert(char *ID, Account *new_account)
{
    Node *current = root;
    while((*ID) != '/0')
    {
        char current_char = (*ID);
        Node *child = current->find_child(current_char);
        if(child == nullptr)
        {
            Node *tmp = new Node();
            current->children_map[current_char] = tmp;
            current = tmp;
        }
        current = child;
        ID++;
    }
    current.current_account = new_account;
}
void Ternary_tree::erase(char *ID)
{
    Node *current = root;
    while((*ID) != '/0')
    {
        current = current->find_child((*ID));
        ID++;
    }
    current->current_account = nullptr;
}
bool Bank::existed(char* const ID)
{
    return (Account_ternary_tree.find(ID) != nullptr);
}
int Bank::login(char* const ID, const string &password)
{
    Account *i = Account_ternary_tree.find(ID);
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
int Bank::create(char* const ID, const string &password)
{
    Account *i = Account_ternary_tree.find(ID);
    if(i != Account_map.end())
        return ID_EXIST;
    else
    {
        Account *new_account = new Account(ID, md5(password), 0);
        char *key = new char[strlen(ID)+1];
        strcpy(key,ID);
        Account_ternary_tree.insert(key, new_account);
        return SUCCESS;
    }
}
int Bank::deleting(char* const ID, const string &password)
{
    Account *i = Account_ternary_tree.find(ID);
    string hash_password = md5(password);
    if(i == nullptr)
        return ID_NOT_FOUND;
    else if(hash_password != i->hash_password)
        return WRONG_PS;
    else
    {
        Account_ternary_tree.erase(ID);
        return SUCCESS;
    }
}
pair<int, int> Bank::merge(char* const ID1, const string &password1, char* const ID2, const string &password2)
{
    Account *i1 = Account_ternary_tree.find(ID1);
    Account *i2 = Account_ternary.find(ID2);
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
        int h1 = i1->.Account_history->size(); 
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
//        vector<History*>().swap((i1->second).Account_history);
//        vector<History*>().swap((i2->second).Account_history);
        (i1->second).Account_history = new_Account_history;
        Account_ternary_tree.erase(i2);
        ans = std::make_pair (SUCCESS, (i1->second).money);
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
        std::pair<int, int> ans = std::make_pair(SUCCESS, last_login->money);
        return ans;
    }
    else
    {
        std::pair<int, int>ans = std::make_pair(FAIL, last_login->money);
        return ans;
    }
}
pair<int, int> Bank::transfer(char* const ID, const int &money)
{
    Account *i = Account_ternary_tree.find(ID);
    std::pair<int, int> ans;
    if(i == nullptr)
        ans = std::make_pair(ID_NOT_FOUND, 0);
    else if((last_login->second).money < money)
        ans = std::make_pair(FAIL, last_login->money);
    else
    {
        last_login->money -= money;
        i->second.money  += money;
        History *tmp = new History((last_login->first), ID, money, transferred_number);
        Transfer_history.push_back(tmp);
        transferred_number++;
        History *new_history = Transfer_history.at(Transfer_history.size() - 1);
        (last_login->second).Account_history->push_back(new_history);
        (i->second).Account_history->push_back(new_history);
        ans = std::make_pair(SUCCESS, last_login->money);
    }
    return ans;
}
void Bank::find_and_print(const char* const regexp)
{
    map<char* const, Account, strCmp>::iterator i;
    bool first_output = 1;
    for(i = Account_map.begin(); i != Account_map.end(); ++i)
    {
        char* const ID = i->first;
        if(match(regexp, ID))
        {
            if(first_output)
            {
                first_output = 0;
                printf("%s",ID);
            }
            else                
                printf(",%s",ID);
        }
    }
    putchar('\n');
}
int Bank::search_and_print(const char* const ID)
{
    vector<History *> *nowHistory = last_login->second.Account_history;
    //cout<<"last_login_ID: "<<last_login->first<<' ';
    bool noRecord = true;
    for(unsigned int i = 0;i < nowHistory->size();i++){
        //cout<<"history "<<i<<": "<<(nowHistory->at(i)->give_ID)<<"\t to \t"<<(nowHistory->at(i)->get_ID)<<"\t, money\t"<<(nowHistory->at(i)->money);
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
void Bank::setBeginIter(void){
    mapIter = Account_map.begin();
    return ;
}
bool Bank::isEndIter(void){
    return mapIter == Account_map.end();
}
void Bank::nextIter(void){
    ++mapIter;
    return ;
}
const Account* Bank::getIter(void){
    return &(mapIter->second);
}
