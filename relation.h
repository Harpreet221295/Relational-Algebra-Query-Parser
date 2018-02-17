#ifndef RELATION_H
#define RELATION_H

#include<string>
#include<vector>
#include<map>

using namespace std;

/* Structure to represent a table entry which could be integer, double or string */
 struct datatype{		
	int type;
	int int_value;				//0
	double double_value;		//1
	string string_value;		//2
};

/* class to represent each relation(instance of relational schema)  */
class relation{

	vector<string> columns;					// Relation schema
	vector<vector<datatype> > table;		//relation
	vector<int> Dtypes;						//array of data types of different columns
	map<string,int> col_int;				// Mapping of column name and column index in a relation
	string structure;						// string concatenated form of relational schema
public:
	relation();		
	// Constructor to construct relation where string denotes the definition of schema						
	relation(string);
	// Insert a set of values in the form of string into relation
	void insert(string);
	// Insert an array of values into relation 
	void InsertTuple(vector<datatype>);
	// print relation schema
	void PrintColumn();
	// print complete relation
	void ShowRelation();
	// Boolean function which returns true if relation contains a particular column name 
	bool HasColumn(string);
	// Return index of column given by string in the relation
	int GetColumn(string);
	// Replacing the original column name in schema with some other name
	void ReplaceColumnName(string, string);
	// Return column name given its index in schema
	string GetColumnName(int);
	// Return an array of column names representing the relational schema
	vector<string> GetColumnVector();
	// Return the number of records in relation
	int NoOfRecords();
	// Return the data type of a column represented by index in relation
	int GetType(int);
	// Return an element in relation given its row and column
	datatype AccessElement(int,int);
	// Return comma separated concatenated form of Relational schema
	string GetStructure();
	// Return the number of columns in relation
	int NoOfColumns();
	// Return a tuple in relation iven its index
	vector<datatype> GetTuple(int);

};

/* Auxillary Functions  */

// Return comma separated strings concatenated string form for an array of relation enteries  
string ConvertToString(vector<datatype>);
// Return the datatype of a particular entry in the relation
string ExtractDataType(datatype);
// Return comma separated strings concatenated string form for an array of strings
string ConvertVectorToString(vector<string>);
// Returns an array of strings by splitting a given string with comma delimiter 
vector<string> ConvertStringToVector(string);

#endif