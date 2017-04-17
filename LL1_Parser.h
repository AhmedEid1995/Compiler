#ifndef LL1_PARSER_H
#define LL1_PARSER_H
#include<bits/stdc++.h>
using namespace std;
class LL1_Parser
{
    public:
        LL1_Parser();
        void execute_parser();
    protected:

    private:
        void read_CFG_file();
        void validate_CFG_file();
        void generate_first_set();
        void generate_follow_set();
        void parse();
        void validate_first(string non_terminal , set < string > s);
        void validate_follow(string non_terminal , set < string > s);
        set<string> get_first(vector < vector < string > > str);
        set<string> get_follow(string str);
        void eliminate_non_terminal_in_first_set (string s);
        void eliminate_non_terminal_in_follow_set (string s);

        vector<string> CFG_file;
        vector<string> CFG_file_after_validation;
       // vector< pair < string , vector < string > > > first_CFG;
        map< string, set < string > > first_CFG;
        map< string, set < string > > follow_CFG;
        vector<string> non_terminals_CFG;
        set<string> non_terminals_CFG_set;
        map < string , vector< vector < string > > > parsed_CFG;
        set<string> non_terminal_recursion;
        string start_non_terminal;

};

#endif // LL1_PARSER_H
