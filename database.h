#ifndef DATABASE_H
#define DATABASE_H

#include "relation.h"
#include<string>
#include<vector>
#include<map>


using namespace std;

/* class to represent Database objects*/
class db{
	vector<relation> mydb;
	vector<string> tables;
	map<string,int> RelationNames;
	int NoOfRelations; 
public:
	
	// constructor
	db();
	// Insert a relation in database with given name
	void InsertRelation(string,relation);
	// Return relation given its name
	relation GetRelation(string);
	// Display the names of all the relations contained in it
	void ShowTables();


	/* operators */

	// Project operator which projects the specified columns of the relation denoted by strings 
	relation project(relation,string);
	
	// Select operator which outputs the rows which satisfies certain conditions denoted by string
	relation select(relation,string);
	
	// Cartesian operator used to compute cartesian product of two relations
	relation Cartesian(relation,string,relation,string);
	
	// Natural operator used to compute natural join of two relations 
	relation Natural(relation,relation);

	// Union operator used to compute union of two queries or a query and relation or two relations
	relation Union(relation,relation);

	// Intersection operator used to compute union of two queries or a query and relation or two relations
	relation Intersection(relation r1,relation r2);
	
	// Rename operator used to denote the relation by some other name and even its columns also by some other name
	relation rename(relation,string);

	/* Parsing */

	relation ParseString(string);

};

#endif