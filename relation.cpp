// IMPLEMENTATION FILE of relation.h 

#include<iostream>
#include "relation.h"
#include <sstream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stack>
using namespace std;

relation::relation(){}

relation::relation(string s){

	structure=s;
    std::istringstream ss(s);
	std::string token,token1;
    stringstream ss1;
    int i=0;
	while(std::getline(ss, token, ',')) {
		//cout<<token<<endl;
		ss1.clear();
		ss1.str("");
    	ss1<<token;
    	ss1>>token1;
    	//cout<<token1<<endl;
    	columns.push_back(token1);
    	col_int[token1]=i;
    	ss1>>token1;
    	//cout<<token1<<endl;
    	if(token1.compare("int") ==0 || token1.compare("INT")==0 )
    		Dtypes.push_back(0);
    	else if(token1.compare("double")==0 || token1.compare("DOUBLE")==0)
    		Dtypes.push_back(1);
    	else if(token1.compare("string")==0 || token1.compare("STRING")==0)
    		Dtypes.push_back(2);
    	else{
    		cout<<"invalid syntax"<<endl;
    		break;	
    	}
    	i++;
	}
}

void relation::PrintColumn(){
	vector<string>::iterator itr;
	vector<int>::iterator itr_type;

	for(itr=columns.begin(),itr_type=Dtypes.begin(); itr!=columns.end(); itr++,itr_type++){
		cout<<*itr<<" "<<*itr_type<<endl;
	}
}

void relation::insert(string s){
	std::istringstream ss(s);
	std::string token;
	int i=0;
	vector<datatype> temp;
	while(std::getline(ss, token, ',')) {

		//cout<<token<<endl;
		struct datatype cell;
		if(Dtypes[i]==0){
			//cout<<atoi(token.c_str())<<endl
			cell.type = 0;
			cell.int_value=atoi(token.c_str());
		}
		else if(Dtypes[i]==1){
			std::string::size_type sz;
			cell.type = 1;
			cell.double_value= stod(token.c_str(),&sz);
		}
		else {
			cell.type = 2;
			cell.string_value=token;
		}
		temp.push_back(cell);
		i++;
	}
	table.push_back(temp);
}

void relation::ShowRelation(){


	vector<string> strt = GetColumnVector();
	vector<string>::iterator itr;

	for(itr = strt.begin(); itr != strt.end(); itr++){
		cout<<*itr<<" ";
	}
	cout<<endl;
	
	int m=table[0].size();
	int n=table.size();
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(Dtypes[j]==0){
				cout<<table[i][j].int_value;
			}
			else if(Dtypes[j]==1){
				cout<<table[i][j].double_value;
			}
			else
				cout<<table[i][j].string_value;
			cout<<" ";
		}
		cout<<endl;
	}

	cout<<endl<<endl;

}

string relation::GetColumnName(int index){
	
	return columns.at(index);
}

bool relation::HasColumn(string str){
	if(col_int.find(str)==col_int.end())
		return false;
	return true;
}
int relation::GetColumn(string str){
	
	return col_int[str];
}

int relation::NoOfRecords(){
	
	return table.size();
}

int relation::GetType(int j){
	
	return Dtypes[j];
}

datatype relation::AccessElement(int i,int j){
	
	return table[i][j];	
}

void relation::ReplaceColumnName(string ocol_name, string newcol_name){

	int index = GetColumn(ocol_name);
	columns[index] = newcol_name;
	map<string,int>::iterator it;
	it = col_int.find(ocol_name);
	col_int.erase(ocol_name);
	col_int[newcol_name]=index;

}

string relation::GetStructure(){
	
	return structure;
}

int relation::NoOfColumns(){
	
	return columns.size();
}


vector<string> relation::GetColumnVector(){
	
	return columns;
}

vector<datatype> relation::GetTuple(int index){
	
	return table.at(index);
}
void relation::InsertTuple(vector<datatype> row){
	
	table.push_back(row);
}



string ConvertToString(vector<datatype> v){

	vector<datatype>::iterator itr;
	string str="";

	for(itr = v.begin(); itr!=v.end(); itr++){

		str+=ExtractDataType(*itr);
	}
	return str;
}

string ExtractDataType(datatype d){


	if(d.type == 0){
		return to_string(d.int_value);
	}
	else if(d.type == 1){
		return to_string(d.double_value);
	}
	else
		return d.string_value;
}

string ConvertVectorToString(vector<string> vs){

	string str = "";
	vector<string>::iterator itr;

	for(itr = vs.begin(); itr != vs.end(); itr++){

		str+=*itr;
		str+=",";
	}

	str.pop_back();
	return str;
}

vector<string> ConvertStringToVector(string s){

	std::istringstream ss(s);
	std::string token;
	
	vector<string> result;
	while(std::getline(ss, token, ',')) {

		result.push_back(token);

	}

	return result;

}