#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "bank.h"
#include <string>
using namespace std;
int main()
{
    string command, ID, password, ID1, password1, ID2, password2; 
    int status_code;
    int num;
    Bank bank;
    while(cin>>command)
    {
        if(command == "login")
        {
            cin>>ID>>password;
            status_code = bank.login(ID, password);
            if(status_code == ID_NOT_FOUND)
                cout<<"ID "<<ID<<" not found"<<endl;
            else if(status_code == WRONG_PS)
                puts("wrong password");
            else
                puts("success");
        }
        else if(command == "create")
        {
            cin>>ID>>password;
            status_code = bank.create(ID, password);
            if(status_code == ID_EXIST)
            {
                cout<<"ID "<<ID<<" exists, ";
                findUncreatedID(ID, 10, bank);
            }
            else if(status_code == SUCCESS)
                puts("success");
        }
        else if(command == "delete")
        {
            cin>>ID>>password;
            status_code = bank.deleting(ID, password);
            if(status_code == ID_NOT_FOUND)
                cout<<"ID "<<ID<<" not found"<<endl;
            else if(status_code == WRONG_PS)
                puts("wrong password");
            else
                puts("success");
        }
        else if(command == "merge")
        {
            cin>>ID1>>password1>>ID2>>password2;
            std::pair <int, int> response;
            response = bank.merge(ID1, password1, ID2, password2);
            if(std::get<0>(response) == ID_NOT_FOUND)
                cout<<"ID "<<ID1<<" not found"<<endl;
            else if(std::get<1>(response) == ID_NOT_FOUND) 
                cout<<"ID "<<ID2<<" not found"<<endl;
            else if(std::get<0>(response) == WRONG_PS) 
                puts("wrong password1");
            else if(std::get<1>(response) == WRONG_PS) 
                puts("wrong password2");
            else
            {
                cout<<"success, "<<ID1<<" has "<<std::get<1>(response)<<" dollars"<<endl;
            }
        }
        else if(command == "deposit")
        {
            int balance;
            cin>>num;
            balance = bank.deposit(num);
            cout<<"success, "<<balance<<" dollars in current account"<<endl;
        }           
        else if(command == "withdraw")
        {
            cin>>num;
            std::pair <int, int> response;
            response = bank.withdraw(num);
            if(std::get<0>(response) == FAIL)
                cout<<"fail, "<<std::get<1>(response)<<" dollars only in current account"<<endl;
            else
                cout<<"success, "<<std::get<1>(response)<<" dollars left in current account"<<endl;
        }
        else if(command == "transfer")
        {
            cin>>ID>>num;
            std::pair <int, int> response;
            response = bank.transfer(ID, num);
            if(std::get<0>(response) == ID_NOT_FOUND){
                cout<<"ID "<<ID<<" not found, ";
                findCreatedID(ID,10,bank);
            }
            else if(std::get<0>(response) == FAIL)
                cout<<"fail, "<<std::get<1>(response)<<" dollars only in current account"<<endl;
            if(std::get<0>(response) == SUCCESS)
                cout<<"success, "<<std::get<1>(response)<<" dollars left in current account"<<endl;
        }
        else if(command == "find")
        {
            cin>>ID;
            bank.find_and_print(ID);
        }
        else if(command == "search")
        {
            cin>>ID;
            status_code = bank.search_and_print(ID);
            if (status_code == ID_NOT_FOUND)
                cout<<"ID "<<ID<<" not found"<<endl;
            else if (status_code == NO_RECORD)
                puts("no record");
            else if (status_code == SUCCESS);
        }
    }
}
