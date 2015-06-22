#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "bank.h"
#include <string>
#include <string.h>
using namespace std;


int main()
{
    //int idx = 1;
    char command[MAX_STRLEN], ID[MAX_STRLEN], password[MAX_STRLEN];
    char ID1[MAX_STRLEN], password1[MAX_STRLEN], ID2[MAX_STRLEN], password2[MAX_STRLEN]; 
    int status_code;
    int num;
    Bank bank;
    while(scanf("%s",command) != EOF)
    {
        //cout<<idx<<' ';
        //idx++;
        if(strcmp(command,"login") == 0)
        {
            scanf("%s %s",ID,password);
            string ps(password);
            status_code = bank.login(ID, ps);
            if(status_code == ID_NOT_FOUND)
                printf("ID %s not found\n",ID);
            else if(status_code == WRONG_PS)
                puts("wrong password");
            else
                puts("success");
        }
        else if(strcmp(command,"create") == 0)
        {
            scanf("%s %s",ID,password);
            string ps(password);
            status_code = bank.create(ID, ps);
            if(status_code == ID_EXIST)
            {
                printf("ID %s exists, ",ID);
                findUncreatedID(ID, 10, bank);
            }
            else if(status_code == SUCCESS)
                puts("success");
        }
        else if(strcmp(command,"delete") == 0)
        {
            scanf("%s %s",ID,password);
            string ps(password);
            status_code = bank.deleting(ID, ps);
            if(status_code == ID_NOT_FOUND)
                printf("ID %s not found\n",ID);
            else if(status_code == WRONG_PS)
                puts("wrong password");
            else
                puts("success");
        }
        else if(strcmp(command,"merge") == 0)
        {
            scanf("%s %s %s %s",ID1,password1,ID2,password2);
            string ps1(password1),ps2(password2);
            std::pair <int, int> response;
            response = bank.merge(ID1, ps1, ID2, ps2);
            if(std::get<0>(response) == ID_NOT_FOUND)
                printf("ID %s not found\n",ID1);
            else if(std::get<1>(response) == ID_NOT_FOUND)
                printf("ID %s not found\n",ID2);
            else if(std::get<0>(response) == WRONG_PS) 
                puts("wrong password1");
            else if(std::get<1>(response) == WRONG_PS) 
                puts("wrong password2");
            else
            {
                printf("success, %s has %d dollars\n",ID1,std::get<1>(response));
            }
        }
        else if(strcmp(command,"deposit") == 0)
        {
            int balance;
            scanf("%d",&num);
            balance = bank.deposit(num);
            printf("success, %d dollars in current account\n",balance);
        }           
        else if(strcmp(command,"withdraw") == 0)
        {
            scanf("%d",&num);
            std::pair <int, int> response;
            response = bank.withdraw(num);
            if(std::get<0>(response) == FAIL)
                printf("fail, %d dollars only in current account\n",std::get<1>(response));
            else
                printf("success, %d dollars left in current account\n",std::get<1>(response));
        }
        else if(strcmp(command,"transfer") == 0)
        {
            scanf("%s %d",ID,&num);
            std::pair <int, int> response;
            response = bank.transfer(ID, num);
            if(std::get<0>(response) == ID_NOT_FOUND){
                printf("ID %s not found, ",ID);
                findCreatedID(ID,10,bank);
            }
            else if(std::get<0>(response) == FAIL)
                printf("fail, %d dollars only in current account\n",std::get<1>(response));
            if(std::get<0>(response) == SUCCESS)
                printf("success, %d dollars left in current account\n",std::get<1>(response));
        }
        else if(strcmp(command,"find") == 0)
        {
            scanf("%s",ID);
            bank.find_and_print(ID);
        }
        else if(strcmp(command,"search") == 0)
        {
            scanf("%s",ID);
            status_code = bank.search_and_print(ID);
            if (status_code == NO_RECORD)
                puts("no record");
            else if (status_code == SUCCESS);
        }
    }
}
