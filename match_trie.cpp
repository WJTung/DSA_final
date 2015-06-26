#include "bank.h"
#include "stdio.h"
void find_all_possibility(Node *node, const char *regexp)
{
    if(node == nullptr)
        return;
    find_match_ID(node, regexp);
    if(node->num_children != 0)
    {
        int i;
        for(i = 0; i < LETTER_NUM; i++)
            find_all_possibility(node->children[i], regexp);
    }
}
void find_match_ID(Node *node, const char *regexp)
{
    if(node == nullptr)
        return;
    char current_char = (*regexp);
    if(current_char == '\0')
    {
        if(node->current_account != nullptr)
            printf("%s ", node->current_account->ID);
        else
            return;
    }
    else if(current_char == '*')
        find_all_possibility(node, regexp + 1);
    else if(node->num_children == 0)
        return;
    else if(current_char == '?')
    {
        int i;
        for(i = 0; i < LETTER_NUM; i++)
            find_match_ID(node->children[i], regexp + 1);
    }
    else
        find_match_ID(node->children[char2Index(current_char)], regexp + 1);
}

