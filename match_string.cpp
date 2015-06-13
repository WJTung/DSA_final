#include <string>
#include <iostream>
using namespace std;
int match(string &, string &, int, int, const int, const int);
int main()
{
	string regexp, ID;
	while(cin>>regexp>>ID)
	{
		cout<<"regexp is "<<regexp<<" and ID is "<<ID<<endl;
		if(match(regexp, ID, 0, 0, regexp.length(), ID.length()))
			cout<<"Match"<<endl;
		else
			cout<<"Not match"<<endl;
	}
}
int match(string &regexp, string &ID, int position_regexp, int position_ID, const int length_regexp, const int length_ID)
{
	if(position_regexp == length_regexp)
		return (position_ID == length_ID);
    if(position_ID == length_ID && regexp.at(position_regexp) != '*')
        return 0;
	if(regexp.at(position_regexp) == '?')
		return match(regexp, ID, position_regexp + 1, position_ID + 1, length_regexp, length_ID);
	if(regexp.at(position_regexp) == '*')
	{
		int position = position_ID;
		while(position != length_ID)
		{
			if(match(regexp, ID, position_regexp + 1, position, length_regexp, length_ID))
				return 1;
			position++;
		}
		return match(regexp, ID, position_regexp + 1, position, length_regexp, length_ID); // *position = '\0' 
	}
	if(regexp.at(position_regexp) != ID.at(position_ID))
		return 0;
	return match(regexp, ID, position_regexp + 1, position_ID + 1, length_regexp, length_ID);
}
