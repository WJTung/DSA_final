void find_all_possibility(Node *node, const char *regexp)
{
    find_match_ID(node, regexp);
    std::map<char, Node*>::iterator i;
    for(i = node->children_map.begin(); i != children_map.end(); ++i)
        find_all_possibility((i->second), regexp);
}
void find_match_ID(Node *node, const char *regexp)
{
    char current_char = (*regexp);
    if(current_char == '\0' && node->current_account != nullptr)
        printf("%s ", current->account->ID);
    else
        return;
    std::map<char, Node*>::iterator i;
    if(current_char == '?')
    {
        for(i = node->children_map.begin(); i != children_map.end(); ++i)
            find_match_ID((i->second), regexp + 1);
    }
    else if(current_char == '*')
        find_all_possibility(node, regexp + 1);
    else
    {
        i = node->children_map.find(current_char);
        if(i == node->children_map.end())
            return;
        find_match_ID((i->second), regexp + 1);
    }
}
