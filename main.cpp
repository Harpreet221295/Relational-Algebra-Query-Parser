#include<iostream>
#include"relation.h"
#include"database.h"
#include<fstream>
#include<string.h>
#define SIZE 255
int main(){

	
	// Creating a new database object
	db newData=db();
  

	// creating relation student
  	relation r1("Name string,RollNo int,stipend DOUBLE");

	// Inserting into student
	r1.insert("Rohan,2,20.33");
	r1.insert("Sam,3,90.99");
	r1.insert("Ajay,4,12.56");
	
	// Committing relation student into database
	newData.InsertRelation("student",r1);




	// creating relation teacher 
	relation r2("Name string,RollNo int,stipend DOUBLE");


	// Inserting into teacher
	r2.insert("Ravi,5,89.10");
	r2.insert("Akshay,6,11.09");
	r2.insert("Ajay,4,12.56");
	
	// Committing relation teacher into database
	newData.InsertRelation("teacher",r2);


	
	// creating relation player
	relation pl("RollNo int,Runs int,Average DOUBLE");
	
	// Inserting into player
	pl.insert("2,45,11.98");
	pl.insert("3,19,78.01");
	pl.insert("6,88,13.13");

	// Committing relation player into database
	newData.InsertRelation("player",pl);

	
	/* Reading Queries from file */
	
	char ch[SIZE];
	ifstream fin1;
	fin1.open("Query.txt");
	while(fin1){
		fin1.getline(ch,SIZE);
		if(ch!=NULL && strlen(ch)>1){
			string str(ch);
			cout<<str<<endl;

			// Calling ParseString function to parse the Query and execute the appropriate operator
			relation r = newData.ParseString(str);

			// Displaying the results of Query
			r.ShowRelation();
		}
	}

	return 0;

}



