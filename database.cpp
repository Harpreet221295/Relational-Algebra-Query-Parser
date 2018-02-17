#include "relation.h"
#include "database.h"
#include <sstream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stack>
#include<iostream>
#include<fstream>

using namespace std;



db::db(){
	
	NoOfRelations=0;
}

void db::InsertRelation(string str,relation r){
	if(RelationNames.find(str)!=RelationNames.end()){
		cout<<" Table already exist"<<endl;
	}
	else{
		tables.push_back(str);
		RelationNames[str]=NoOfRelations++;
		mydb.push_back(r);
	}
}

relation db::GetRelation(string str){
	if(RelationNames.find(str)==RelationNames.end()){
		cout<<"Table does not exist"<<endl;
		relation r=relation();
		return r;
	}
	return mydb[RelationNames[str]];
}

void db::ShowTables(){
	vector<string>::iterator itr;
	for(itr = tables.begin(); itr!=tables.end(); itr++){
		cout<<*itr<<endl;
	}
}








relation db::project(relation r,string str){

	map<string,int> duplicates;
	std::istringstream ss(str);
	std::string token;
	vector<string> Values;
	vector<int> Indices;
	int index;
	while(std::getline(ss, token, ',')){
		if(r.HasColumn(token)){
			index=r.GetColumn(token);
			Indices.push_back(index);
			Values.push_back(token);
		}
		else{
			cout<<"column list is not correct"<<endl;
			break;
		}
	}

	vector<string>::iterator itrV;
	vector<int>::iterator itrI;
	string arg="";

	for(itrV=Values.begin(),itrI=Indices.begin();itrV!=Values.end();itrV++,itrI++){
		arg=arg+*itrV+" ";
		if(r.GetType(*itrI)==0)
			arg=arg+"int";
		else if(r.GetType(*itrI)==1)
			arg=arg+"double";
		else
			arg=arg+"string";
		arg=arg+",";
	}
	arg.pop_back();

	//cout<<arg<<endl;
	
	relation temp(arg); 

	int n=r.NoOfRecords();
	int m=Values.size();
	
	for(int i=0;i<n;i++){
		string add="";
		for(int j=0;j<m;j++){
			if(r.GetType(Indices[j])==0)
				add=add+to_string(r.AccessElement(i,j).int_value);
			else if(r.GetType(j)==1)
				add=add+to_string(r.AccessElement(i,j).double_value);
			else
				add=add+r.AccessElement(i,j).string_value;
			add=add+",";
		}
		if(duplicates.find(add)==duplicates.end()){
			temp.insert(add);
			duplicates[add]=1;
		}
	}
	return temp;
}




relation db::select(relation r, string str){

	std::istringstream ss(str);
	std::string token;


	if(str.find("&") != string::npos && str.find("|") !=string::npos){
		cout<<"Implementation only for & or or connector not mixed";
		relation r = relation();
		return r;
	}

	else if(str.find("&") !=string::npos){
		

		vector<string> conditions;
		while(std::getline(ss, token, '&')){
    	    conditions.push_back(token);
		}

		vector<string> FirstArg;
		vector<string> SecondArg;
		vector<string> operatorT;
		vector<string>::iterator itr;
		vector<string> temp;
		for(itr=conditions.begin();itr!=conditions.end();itr++){

			ss.clear();
			ss.str("");
			ss.str(*itr);
			if(itr->find("=") != string::npos){
				operatorT.push_back("=");
				while(getline(ss,token,'=')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
			else if(itr->find(">") != string::npos){
				operatorT.push_back(">");
				while(getline(ss,token,'>')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
			else if(itr->find("<") != string::npos){
				operatorT.push_back("<");
				while(getline(ss,token,'<')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
		}

		vector<int> FirstArgIndex;
		for(itr=FirstArg.begin();itr!=FirstArg.end();itr++){
			FirstArgIndex.push_back(r.GetColumn(*itr));
		}



		int n=r.NoOfRecords();
		int m=operatorT.size();
		int h =r.NoOfColumns();
		relation tempRelation(r.GetStructure());


		for(int i=0;i<n;i++){
			bool result=true;
			for(int j=0;j<m;j++){
				int a =r.GetType(FirstArgIndex[j]);
				

				
				if(operatorT[j]=="="){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value != second.int_value){
								result=false;
								break;
							}
						}
						else if(a==1){
							double g = first.double_value-second.double_value;
							if(g>0.0000000001 && g < -0.0000000001){
								result=false;
								break;
							}
						}
						else if(a==2){
							if(first.string_value != second.string_value){
								result=false;
								break;
							}
						}

					}
					else{
						 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value != atoi(SecondArg[j].c_str())){
								result=false;
								break;
							}
						}
						else if( a == 1){
							double g =(r.AccessElement(i,FirstArgIndex[j])).double_value-stod(SecondArg[j].c_str()); 
							if(g > 0.00000000001 || g< -0.00000000001){
								result=false;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value != SecondArg[j].c_str()){
								result=false;
								break;
							}
						}
					}					
				}



				
				else if(operatorT[j] == ">"){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value <= second.int_value){
								result=false;
								break;
							}
						}
						else if(a==1){
							//double g = first.double_value-second.double_value;
							if(first.double_value <= second.double_value){
								result=false;
								break;
							}
						}
						else if(a==2){
							if(first.string_value <= second.string_value){
								result=false;
								break;
							}
						}
					}
					else{
						int a =r.GetType(FirstArgIndex[j]); 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value <= atoi(SecondArg[j].c_str())){
								result=false;
								break;
							}
						}
						else if( a == 1){
							if((r.AccessElement(i,FirstArgIndex[j])).double_value<=stod(SecondArg[j].c_str())){
								result=false;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value <= SecondArg[j]){
								result=false;
								break;
							}
						}
					}	
				}

				else if(operatorT[j] == "<"){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value >= second.int_value){
								result=false;
								break;
							}
						}
						else if(a==1){
							//double g = first.double_value-second.double_value;
							if(first.double_value >= second.double_value){
								result=false;
								break;
							}
						}
						else if(a==2){
							if(first.string_value >= second.string_value){
								result=false;
								break;
							}
						}
					}
					else{
						int a =r.GetType(FirstArgIndex[j]); 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value >= atoi(SecondArg[j].c_str())){
								result=false;
								break;
							}
						}
						else if( a == 1){
							if((r.AccessElement(i,FirstArgIndex[j])).double_value >= stod(SecondArg[j].c_str())){
								result=false;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value >= SecondArg[j]){
								result=false;
								break;
							}
						}
					}	
				}


			}

			if(result == true){
				string add="";

				for(int k=0;k<h;k++){
					if(r.GetType(k) == 0)
						add = add+to_string(r.AccessElement(i,k).int_value)+",";
					else if(r.GetType(k) == 1)
						add = add+to_string(r.AccessElement(i,k).double_value)+",";
					else if(r.GetType(k) == 2)
						add = add+r.AccessElement(i,k).string_value+",";
				}
				add.pop_back();
				tempRelation.insert(add);
			}
			
		}

		return tempRelation;
	}

	else{

		vector<string> conditions;
		while(std::getline(ss, token, '|')){
    	    conditions.push_back(token);
		}

		vector<string> FirstArg;
		vector<string> SecondArg;
		vector<string> operatorT;
		vector<string>::iterator itr;
		vector<string> temp;
		for(itr=conditions.begin();itr!=conditions.end();itr++){

			ss.clear();
			ss.str("");
			ss.str(*itr);
			if(itr->find("=") != string::npos){
				operatorT.push_back("=");
				while(getline(ss,token,'=')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
			else if(itr->find(">") != string::npos){
				operatorT.push_back(">");
				while(getline(ss,token,'>')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
			else if(itr->find("<") != string::npos){
				operatorT.push_back("<");
				while(getline(ss,token,'<')){
					temp.push_back(token);
				}
				SecondArg.push_back(temp.back());
				temp.pop_back();
				FirstArg.push_back(temp.back());
				temp.pop_back();
			}
		}

		vector<int> FirstArgIndex;
		for(itr=FirstArg.begin();itr!=FirstArg.end();itr++){
			FirstArgIndex.push_back(r.GetColumn(*itr));
		}





		int n=r.NoOfRecords();
		int m=operatorT.size();
		int h =r.NoOfColumns();
		relation tempRelation(r.GetStructure());




		for(int i=0;i<n;i++){
			bool result=false;
			for(int j=0;j<m;j++){
				int a =r.GetType(FirstArgIndex[j]);
				

				
				if(operatorT[j]=="="){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value == second.int_value){
								result=true;
								break;
							}
						}
						else if(a==1){
							double g = first.double_value-second.double_value;
							if(g<0.0000000001 && g > -0.0000000001){
								result=true;
								break;
							}
						}
						else if(a==2){
							if(first.string_value == second.string_value){
								result=true;
								break;
							}
						}

					}
					else{
						 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value == atoi(SecondArg[j].c_str())){
								result=true;
								break;
							}
						}
						else if( a == 1){
							double g =(r.AccessElement(i,FirstArgIndex[j])).double_value-stod(SecondArg[j].c_str()); 
							if(g < 0.00000000001 || g> -0.00000000001){
								result=true;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value == SecondArg[j].c_str()){
								result=true;
								break;
							}
						}
					}					
				}



				
				else if(operatorT[j] == ">"){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value > second.int_value){
								result=true;
								break;
							}
						}
						else if(a==1){
							//double g = first.double_value-second.double_value;
							if(first.double_value > second.double_value){
								result=true;
								break;
							}
						}
						else if(a==2){
							if(first.string_value > second.string_value){
								result=true;
								break;
							}
						}
					}
					else{
						int a =r.GetType(FirstArgIndex[j]); 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value > atoi(SecondArg[j].c_str())){
								result=true;
								break;
							}
						}
						else if( a == 1){
							if((r.AccessElement(i,FirstArgIndex[j])).double_value > stod(SecondArg[j].c_str())){
								result=true;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value > SecondArg[j]){
								result=true;
								break;
							}
						}
					}	
				}

				else if(operatorT[j] == "<"){
					if(r.HasColumn(SecondArg[j])){
						datatype first = r.AccessElement(i,FirstArgIndex[j]);
						datatype second = r.AccessElement(i,r.GetColumn(SecondArg[j]));
						if(a==0){
							if(first.int_value < second.int_value){
								result=true;
								break;
							}
						}
						else if(a==1){
							//double g = first.double_value-second.double_value;
							if(first.double_value < second.double_value){
								result=true;
								break;
							}
						}
						else if(a==2){
							if(first.string_value < second.string_value){
								result=true;
								break;
							}
						}
					}
					else{
						int a =r.GetType(FirstArgIndex[j]); 
						if( a == 0){
							if((r.AccessElement(i,FirstArgIndex[j])).int_value < atoi(SecondArg[j].c_str())){
								result=true;
								break;
							}
						}
						else if( a == 1){
							if((r.AccessElement(i,FirstArgIndex[j])).double_value < stod(SecondArg[j].c_str())){
								result=true;
								break;
							}	
						}
						else if(a==2){
							if((r.AccessElement(i,FirstArgIndex[j])).string_value < SecondArg[j]){
								result=true;
								break;
							}
						}
					}	
				}


			}

			if(result == true){
				string add="";

				for(int k=0;k<h;k++){
					if(r.GetType(k) == 0)
						add = add+to_string(r.AccessElement(i,k).int_value)+",";
					else if(r.GetType(k) == 1)
						add = add+to_string(r.AccessElement(i,k).double_value)+",";
					else if(r.GetType(k) == 2)
						add = add+r.AccessElement(i,k).string_value+",";
				}
				add.pop_back();
				tempRelation.insert(add);
			}
			
		}



	return tempRelation;


	}
	
	
}



relation db::rename(relation r, string str){

		relation nr = r;
		vector<string>  input = ConvertStringToVector(str);
		vector<string>::iterator itr;

		itr = input.begin();

		string new_name = *itr;
		
		itr++;
		int i;

		for(i=0; itr!=input.end(); itr++,i++){

			nr.ReplaceColumnName(r.GetColumnName(i), *itr);

		}

		InsertRelation(new_name,nr);
		
		return nr;
}


relation db::Cartesian(relation r1, string r1_name, relation r2, string r2_name){

	vector<string> v1 = r1.GetColumnVector();
	vector<string> v2 = r2.GetColumnVector();

	vector<string>::iterator itr;
	string str = "";

	for(itr = v1.begin(); itr != v1.end(); itr++){

		str+=r1_name;
		str+=".";
		str+=*itr;
		str+=" ";
		int DataType = r1.GetType(r1.GetColumn(*itr));

		DataType != 0 ? DataType==1 ? str+="double" : str+="string" : str+="int";
		str+=",";
	}

	for(itr = v2.begin(); itr != v2.end(); itr++){

		str+=r2_name;
		str+=".";
		str+=*itr;
		str+=" ";
		int DataType = r2.GetType(r2.GetColumn(*itr));

		DataType != 0 ? DataType==1 ? str+="double" : str+="string" : str+="int";
		str+=",";
	}

	str.pop_back();

	relation nr = relation(str);

	int n1 = r1.NoOfRecords();
	int n2 = r2.NoOfRecords();

	for(int i = 0;i<n1;i++){
		for(int j=0;j<n2;j++){

			vector<datatype> a,b,newv;
			a = r1.GetTuple(i);
			b = r2.GetTuple(j);
			newv.reserve(a.size() + b.size());
			newv.insert(newv.end(),a.begin(),a.end());
			newv.insert(newv.end(),b.begin(),b.end());

			nr.InsertTuple(newv);
		}
	}

	return nr;
}

relation db::Natural(relation r1, relation r2){

	vector<string> v1 = r1.GetColumnVector();
	vector<string> v2 = r2.GetColumnVector();
	vector<string> common;

	map<string,int> IsPresent;

	vector<string>::iterator itr1,itr2;

	for(itr1 = v1.begin(); itr1 != v1.end(); itr1++){
		IsPresent[*itr1] = 1;
	}


	vector<string> resultcols = v1;

	for(itr2=v2.begin(); itr2 != v2.end(); itr2++){
		

		if(IsPresent.find(*itr2) != IsPresent.end())
			common.push_back(*itr2);
		else{
			resultcols.push_back(*itr2);
		}
	}
	
	int n1 = r1.NoOfRecords();
	int n2 = r2.NoOfRecords();


	string inputcols="";

	
	for(itr1 = resultcols.begin(); itr1 != resultcols.end(); itr1++){

		if(r1.HasColumn(*itr1)){
			inputcols+=*itr1;
			inputcols+=" ";
			int t = r1.GetType(r1.GetColumn(*itr1));
			string s = t != 0? t == 1 ? "double" : "string" : "int";
			inputcols+=s;
		}

		else{
			inputcols+=*itr1;
			inputcols+=" ";
			int t = r2.GetType(r2.GetColumn(*itr1));
			string s = t != 0? t == 1 ? "double" : "string" : "int";
			inputcols+=s;
		}
		inputcols+=",";
	}

		
	
	inputcols.pop_back();
	

	relation nr = relation(inputcols);

	int flag = 0;

	

	for(int i=0;i<n1;i++){
		for(int j=0;j<n2;j++){

			flag = 0;

			for(itr2 = common.begin(); itr2 != common.end(); itr2++){

	
				int index_i = r1.GetColumn(*itr2);

				int index_j = r2.GetColumn(*itr2);

				datatype element_i = r1.AccessElement(i,index_i);
				datatype element_j = r2.AccessElement(j,index_j);


				if(element_i.type != element_j.type){
					flag =1;
					break;
				}

				else if(element_i.type == 0 && element_i.int_value != element_j.int_value){
					flag =1;
					break;	
				}
				else if(element_i.type == 1 && (element_i.double_value-element_j.double_value>0.0000000000001 || element_i.double_value-element_j.double_value<-0.0000000000001)){
					flag =1;
					break;
				}
				else if(element_i.type == 2 && element_i.string_value != element_j.string_value){
					flag=1;
					break;
				}

			}


			if(flag == 0){
					
				vector<datatype> addrow;

				for(itr2 = resultcols.begin(); itr2 != resultcols.end(); itr2++){

					if(r1.HasColumn(*itr2)){

						addrow.push_back(r1.AccessElement(i,r1.GetColumn(*itr2)));
					}
					else{

						addrow.push_back(r2.AccessElement(j,r2.GetColumn(*itr2)));
					}

				}

				nr.InsertTuple(addrow);

			}


		}
	}

	return nr;
}


relation db::Union(relation r1, relation r2){

	string s1 = r1.GetStructure();
	string s2 = r2.GetStructure();

	if (s1 != s2){
		cout<<"Column structure of two relations is not same"<<endl;
		relation nr = relation();
		return nr; 
	}
	else{

		map<string,int> IsPresent;
		relation nr = r1;

		int n1 = r1.NoOfRecords();

		string str;
		
		for(int i=0;i<n1;i++){

			str = ConvertToString(r1.GetTuple(i));
			IsPresent[str] = true;
		}

		int n2 = r2.NoOfRecords();

		for(int i=0;i<n2;i++){
			
			str = ConvertToString(r2.GetTuple(i));

			if(IsPresent.find(str) == IsPresent.end()){
				nr.InsertTuple(r2.GetTuple(i));
			}
		}

		return nr;
	}

}

relation db::Intersection(relation r1, relation r2){

	string s1 = r1.GetStructure();
	string s2 = r2.GetStructure();

	if (s1 != s2){
		cout<<"Column structure of two relations is not same"<<endl;
		relation nr = relation();
		return nr; 
	}
	else{

		map<string,int> IsPresent;
		relation nr = relation(r1.GetStructure());

		int n1 = r1.NoOfRecords();

		string str;
		
		for(int i=0;i<n1;i++){

			str = ConvertToString(r1.GetTuple(i));
			IsPresent[str] = true;
		}

		int n2 = r2.NoOfRecords();

		for(int i=0;i<n2;i++){
			
			str = ConvertToString(r2.GetTuple(i));

			if(IsPresent.find(str) != IsPresent.end()){
				nr.InsertTuple(r2.GetTuple(i));
			}
		}

		return nr;
	}

}



relation db::ParseString(string str){
	stack<string> Roperator;
	stack<char> para;
	stack<string> arglist;
	stack<relation> Srelation;

	relation r,r2;
	string oper;
	string tabl;
	string arg;
	int i=0;
	int offset1=0;
	int offset2=0;
	int flag = 0;

	if(str.find(" ")!=-1){
		cout<<"syntax error"<<endl;
		return r;
	}
	


	while(str[i]!='\0'){
		if(str[i] == '('){
			para.push('(');
			offset2=i;
			//cout<<offset1<<" "<<offset2<<endl;
			oper=str.substr(offset1,offset2-offset1);
			//cout<<oper<<endl;
			offset1=i+1;
			
			if(oper != "project" && oper != "select" && oper != "cartesian" && oper != "intersection" && oper != "union" && 
							oper != "natural" && oper != "rename"){
				cout<<"syntax error"<<endl;
				break;
			}

			else{
				Roperator.push(oper);
				flag=0;
			}
			
		}

		if(str[i] == '['){
			offset2=i-1;
			if(offset1<offset2){
				tabl=str.substr(offset1,offset2-offset1);
				r = GetRelation(tabl);
				Srelation.push(r);
			}
			offset1=i+1;
		}

		if(str[i] == ']'){
			offset2=i;
			arg=str.substr(offset1,offset2-offset1);
			arglist.push(arg);
			
		}

		if(str[i] == ')'){

			if(flag == 1){

				offset2 = i;
				Srelation.push(GetRelation(str.substr(offset1,offset2-offset1)));
				flag = 0;
			}

			para.pop();
			oper = Roperator.top();
			Roperator.pop();
			

			if(oper == "project"){
				arg = arglist.top();
				arglist.pop();
				r = Srelation.top();
				Srelation.pop();
				Srelation.push(project(r,arg));
			}
			
			else if(oper == "select"){
				
				arg = arglist.top();
				arglist.pop();
				r = Srelation.top();
				Srelation.pop();
				Srelation.push(select(r,arg));	 
			}

			else if(oper == "cartesian"){

				r2 = Srelation.top();
				Srelation.pop();
				r = Srelation.top();
				Srelation.pop();

				Srelation.push(Cartesian(r,"I",r2,"II"));

			}

			else if(oper == "union"){

				r2 = Srelation.top();
				Srelation.pop();
				r = Srelation.top();
				Srelation.pop();

				Srelation.push(Union(r,r2));



			}

			else if(oper == "intersection"){

				r2 = Srelation.top();
				Srelation.pop();
				r = Srelation.top();
				Srelation.pop();

				Srelation.push(Intersection(r,r2));

			}

			else if(oper == "natural"){

				r2 = Srelation.top();
				Srelation.pop();
				r = Srelation.top();
				Srelation.pop();

				Srelation.push(Natural(r,r2));
			}


			else if(oper == "rename"){

				arg = arglist.top();
				arglist.pop();
				r = Srelation.top();
				Srelation.pop();
				Srelation.push(rename(r,arg));	 

			}



			offset1=i+1;
		}

		if(str[i] == ':'){


			if(str[i-1] != ')'){

				flag = 1;
				offset2 = i;
				Srelation.push(GetRelation(str.substr(offset1,offset2-offset1)));
				offset1 = i+1;
			}

			else if(str[i-1] == ')'){

				flag = 1;
				offset1 = i+1;
			}
		}
		i++;
	}

	if(!para.empty()){
		cout<<"invalid syntax";
		relation mr;
		return mr;
	}
	return Srelation.top();
}






















