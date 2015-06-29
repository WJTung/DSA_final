int match(const char *regexp, const char *ID)
{
    if(*regexp == '\0')
        return (*ID == '\0');
    if(*regexp == '?')
    {
        if(*ID == '\0')
            return 0;
        return match(regexp + 1, ID + 1);
    }
    if(*regexp == '*')
    {
        const char *position = ID;
        while(*position != '\0')
        {
            if(match(regexp + 1, position))
                return 1;
            position++;
        }
        return match(regexp + 1, position); // *position = '\0' 
    }
    if(*regexp != *ID)
        return 0;
    return match(regexp + 1, ID + 1);
}
