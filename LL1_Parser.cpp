#include "LL1_Parser.h"
#include <iostream>
#include <fstream>
#include<bits/stdc++.h>
typedef map<string, vector<vector < string > > >::const_iterator MapIterator;

using namespace std;
LL1_Parser::LL1_Parser()
{
    //ctor
}
/*
    This method used to read the CFG file
    Return type is void
*/
void LL1_Parser::read_CFG_file(){
    ifstream in("CFG.txt");
    //generate error according to no file found with this name
    if(!in) {
        cout << "Cannot open input file.\n";
    }
    char str[255];
    int i = 0;
    while(in) {
        in.getline(str, 255);
        //create vector with all lines for CFG file
        if(in) CFG_file.push_back(str);
    }
    in.close();
}
/*
    This method is used to validate CFG file and
    eliminate the new lines in the input file
    return value is Void
*/
void LL1_Parser::validate_CFG_file(){
    for(int i = 0 ; i < CFG_file.size() ;){
        string temp = CFG_file[i];
        //case we need to combine the more than one line together
        if(temp[0] == '|'){
            int index = 0;
            string previous = CFG_file[i-1];
            previous += " ";
            //get all oring together
            while(index < CFG_file.size() && temp[0] == '|'){
                previous += temp;
                previous += " ";
                // add special char to flag the string
                CFG_file[i + index] = "$";
                index++;
                temp = CFG_file[i + index];

            }
            //add it to modified vector of CFG
            CFG_file_after_validation.push_back(previous);
            i += index;
        }else if(CFG_file[i][0] != '$'){
            if((i+1 < CFG_file.size() && CFG_file[i+1][0] != '|') || i+1 == CFG_file.size())
                CFG_file_after_validation.push_back(temp);
            i++;
        }else{
            i++;
        }
    }
    //deallocate vector of CFG file before validating
    CFG_file.clear();
}
/*
    This method is used to compute first for nonterminals
    return value is void
*/
void LL1_Parser::generate_first_set(){
    //iterate throw all non terminal and get first for each one
    for (MapIterator iter = parsed_CFG.begin(); iter != parsed_CFG.end(); iter++){
        string non_terminal = iter->first;
        set < string > temp = get_first(parsed_CFG[non_terminal]);
        //update first after calculating it
        first_CFG[non_terminal] = temp;
    }
    //validate first for non terminal and eliminate non terminal from first
    for(int i = non_terminals_CFG.size() - 1 ; i >=0 ; i--){
        validate_first(non_terminals_CFG[i] , first_CFG[non_terminals_CFG[i] ]);
    }

    for(map < string , set<string > >::iterator it = first_CFG.begin() ; it != first_CFG.end() ; ++it ){
        cout<<it->first<<endl;
        cout<<"---------------"<<endl;
        set< string> temp = it->second;
        for(set<string>::iterator i = temp.begin() ; i != temp.end() ; ++i){
            cout<<*i <<endl;
        }
        cout<<endl;
    }
    cout<<"-------------------------------------------------------"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
}


/*
    This method is used to get first for each production and
    if it is \L we skip and try to get next first

*/
set <string> LL1_Parser::get_first(vector < vector<string> > s){
    set < string > result;
    for(int i = 0 ; i < s.size() ; i++){
        bool flag =  false;
        for(int j = 0 ; j < s[i].size() ; j++){
            //if first is nor epsilon
            if(s[i][j] != "'\\L'"){
                result.insert(s[i][j]);
                flag = true;
                break;
            }
        }
        //first is epsilon
        if(!flag){
            result.insert("'\\L'");
        }

    }
    return result;
}
/*
    this method is used to eliminate non terminals from being first in first set
    by making a recursive call to eliminate it
*/
void LL1_Parser::validate_first(string non_terminal , set < string > s){
    set < string > ans;
    for (set<string>::iterator it=s.begin(); it!=s.end(); ++it){
        if(non_terminals_CFG_set.find(*it) == non_terminals_CFG_set.end() ){
            ans.insert(*it);
        }else{
            //case where we have non terminal in first set
            non_terminal_recursion.clear();
            eliminate_non_terminal_in_first_set (*it );
            //update first after eliminating the non terminal
            for(set <string>::iterator i = non_terminal_recursion.begin() ; i != non_terminal_recursion.end() ; ++i){
                ans.insert(*i);
            }
        }
    }
    first_CFG[non_terminal] = ans;

}
/*
    this method is used to replace non terminal in the first by its first set
*/
void LL1_Parser::eliminate_non_terminal_in_first_set (string s){
    if(non_terminals_CFG_set.find(s) == non_terminals_CFG_set.end() ){
        non_terminal_recursion.insert(s);
        return;
    }
    set<string> temp = first_CFG[s];
    for(set <string>::iterator it = temp.begin() ; it != temp.end() ; ++it){
        eliminate_non_terminal_in_first_set(*it);
    }
    return;
}


void LL1_Parser::generate_follow_set(){
    for(int i = 0 ; i < non_terminals_CFG.size() ; i++){
        string non_terminal = non_terminals_CFG[i];
        set < string > st = get_follow(non_terminal);
        if(i == 0 ){
            st.insert("'$'");
        }
        follow_CFG[non_terminal] = st;
    }

     for(int i = non_terminals_CFG.size() - 1 ; i >=0 ; i--){
        validate_follow(non_terminals_CFG[i] , follow_CFG[non_terminals_CFG[i] ]);
    }

    for(map < string , set<string > >::iterator it = follow_CFG.begin() ; it != follow_CFG.end() ; ++it ){
        cout<<it->first<<endl;
        cout<<"---------------"<<endl;
        set< string> temp = it->second;
        for(set<string>::iterator i = temp.begin() ; i != temp.end() ; ++i){
            cout<<*i <<endl;
        }
        cout<<endl;
    }

}

set < string > LL1_Parser::get_follow(string target){
	bool found = false;
	set < string > st;
	for(map<string, vector<vector < string > > >::iterator it = parsed_CFG.begin(); it != parsed_CFG.end() ; ++it){
		string non_terminal = it->first;
		vector < vector < string > > temp = it->second;
		for(int i = 0 ; i < temp.size() ; i++){
	        found = false;
	        for(int j = 0 ; j < temp[i].size() ; j++){
	            if(temp[i][j] == target){
	                found = true;
	            }else if(found && non_terminals_CFG_set.find(temp[i][j]) == non_terminals_CFG_set.end() && temp[i][j] != "'\\L'"){
	                //terminal case
	                st.insert(temp[i][j]);
	                found = false;
	                break;
	            }else if(found && non_terminals_CFG_set.find(temp[i][j]) == non_terminals_CFG_set.end() && temp[i][j] == "'\\L'"){
	                // Do nothing
	            }else if(found && non_terminals_CFG_set.find(temp[i][j]) != non_terminals_CFG_set.end()){
	                //non terminal case
	                bool flag_epsilon = false;
	                set < string > first_non_terminal = first_CFG[temp[i][j]];
	                for(set < string >::iterator it = first_non_terminal.begin() ; it != first_non_terminal.end() ; ++it){
	                    if(*it == "'\\L'"){
	                        flag_epsilon = true;
	                    }else{
	                        st.insert(*it);
	                    }
	                }
	                if(!flag_epsilon){
	                    found = false;
	                    break;
	                }
	            }
	        }
	        if(found){
	            if(non_terminal != target)
	            	st.insert(non_terminal);
        	}
    	}
	}

    return st;
}
void LL1_Parser::validate_follow(string non_terminal , set < string > s){
    set < string > ans;
    for (set<string>::iterator it=s.begin(); it!=s.end(); ++it){
        if(non_terminals_CFG_set.find(*it) == non_terminals_CFG_set.end() ){
            ans.insert(*it);
        }else{
            //case where we have non terminal in first set
            non_terminal_recursion.clear();
            eliminate_non_terminal_in_follow_set (*it );
            //update first after eliminating the non terminal
            for(set <string>::iterator i = non_terminal_recursion.begin() ; i != non_terminal_recursion.end() ; ++i){
                ans.insert(*i);
            }
        }
    }
    follow_CFG[non_terminal] = ans;

}




void LL1_Parser::eliminate_non_terminal_in_follow_set (string s){
    if(non_terminals_CFG_set.find(s) == non_terminals_CFG_set.end() ){
        non_terminal_recursion.insert(s);
        return;
    }
    set<string> temp = follow_CFG[s];
    for(set <string>::iterator it = temp.begin() ; it != temp.end() ; ++it){
        eliminate_non_terminal_in_follow_set(*it);
    }
    return;
}





void LL1_Parser:: parse (){
    for(int i = 0 ; i < CFG_file_after_validation.size() ; i++){
        std::string str= CFG_file_after_validation[i];
        std::istringstream buf(str);
        std::istream_iterator<std::string> beg(buf), end;

        std::vector<std::string> tokens(beg, end); // done!
        string non_terminal = tokens[1];
        non_terminals_CFG.push_back(non_terminal);
        non_terminals_CFG_set.insert(non_terminal);
        vector<string> subset;
        vector< vector < string > > temp;
        for( int j = 3 ; j < tokens.size() ; j++){
            if(tokens[j] == "|"){
                temp.push_back(subset);
                subset.clear();
            }else{
                subset.push_back(tokens[j]);
            }
        }
        temp.push_back(subset);
        parsed_CFG[non_terminal] = temp;
        if(i == 0){
            start_non_terminal = non_terminal;
        }
    }

    /*for (MapIterator iter = parsed_CFG.begin(); iter != parsed_CFG.end(); iter++){
        string x = iter->first;
        cout<<"------------"<<x<<endl;
        vector < vector<string> >  temp = parsed_CFG[x];
        for(int i = 0 ; i < temp.size() ; i++){
            for(int j = 0 ; j < temp[i].size() ; j++){
                cout<<temp[i][j]<<endl;
            }
            cout<<"----"<<endl;
        }
        cout<<endl;
    }*/


}

/*
    This method used to start executing the parser
    Step 1 : Read the CFG file
*/
void LL1_Parser::execute_parser(){
    // read CFG file
    read_CFG_file();
    // validate CFG file by appending broken lines together
    validate_CFG_file();

    parse();
    // generate first set for given CFG file
    generate_first_set();

    generate_follow_set();




}
