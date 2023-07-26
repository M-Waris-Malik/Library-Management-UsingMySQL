#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <windows.h>
#include <sstream>
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "abc123#";
const char* DB = "mydb";

class Student{
private:
 string Id;
public:
Student() : Id("") {}

void setId(string id) {
  Id = id;
 }
 
 string getId() {
  return Id;
 }
 
};

class Library{
private:
 string Name;
 int Quantity;
public:
Library() : Name(""), Quantity(0) {}

void setName(string name) {
  Name = name;
}

void setQuantity(int quantity) {
  Quantity = quantity;
}

int getQuantity() {
  return Quantity;
}
string getName() {
  return Name;
}

	
};

void admin(MYSQL* conn, Library l, Student s){
	bool closed = false;
	while(!closed){
	int choice;
cout << "1. Add Book." << endl;
 cout << "2. Add Student." << endl;
 cout << "0. Exit." << endl;
 cout << "Enter Choice: ";
 cin >> choice;
 
 if(choice==1){
 	system("cls");
string name;
int quantity;

cout<<"Book Name: ";
cin>>name;
l.setName(name);

cout<<"Enter Quantity: ";
cin>>quantity;
l.setQuantity(quantity);

int Iq = l.getQuantity();
stringstream ss;
ss<<Iq;
string Sq = ss.str();

string book = "INSERT INTO lib (Name,Quantity) VALUES('"+l.getName()+"', '"+Sq+"') ";
if(mysql_query(conn,book.c_str())){
	cout<<"Error: "<<mysql_error(conn)<<endl;
}
else{
	cout<<"Book Inserted Successfuly"<<endl;
}
 }// if1
 
 else if(choice==2){
 	system("cls");
string id;
 cout << "Enter Student ID: ";
 cin >> id;
 s.setId(id);
 
  string st = "INSERT INTO student (Id) VALUES('" + s.getId() + "')";
 if (mysql_query(conn, st.c_str())) {
 cout << "Error: " << mysql_error(conn) << endl;
 }
else {
  cout << "Student Inserted Successfully" << endl; 
}
}//if2
else if(choice ==0){
	closed = true;
	cout<<"System is closing"<<endl;
}
	}//while
Sleep(3000);
}//function

//display function
void display(MYSQL* conn){
	system("cls");
cout<<"Available Books"<<endl;
cout<<"***************"<<endl;

string disp= "SELECT * FROM lib";
 if (mysql_query(conn, disp.c_str())) {
  cout << "Error: " << mysql_error(conn) << endl;
 }
 else{
 	MYSQL_RES* res;
 	res= mysql_store_result(conn);
 	if(res){
 		int num= mysql_num_fields(res);
 	MYSQL_ROW row;
 	while(row=mysql_fetch_row(res)){
 		for(int i=0; i< num; i++){
 			cout<<" "<<row[i];
		 }
		cout<<endl;
	 }
	 mysql_free_result(res);
	 }
 }
}

//book function

int book(MYSQL* conn, string Bname){
string exist = "SELECT Name, Quantity FROM lib WHERE Name = '" + Bname + "'";
if (mysql_query(conn, exist.c_str())) {
 cout << "Error: " << mysql_error(conn) << endl;
 }
else{
MYSQL_RES* res;
res = mysql_store_result(conn);
if(res){
	int num = mysql_num_fields(res);
MYSQL_ROW row;
while(row=mysql_fetch_row(res)){
	for(int i=0; i< num; i++){
	if(Bname == row[i]){
	int quantity = atoi(row[i+1]);
	return quantity;	
	}
	else{
		cout<<"Book Not Found."<<endl;
	}
	}
} 
mysql_free_result(res);
}
	
}//else if exist
return 0;
Sleep(5000);
}//function

// user function

void user(MYSQL* conn, Library l, Student s){
	system("cls");
	display(conn);
string Sid;
cout<<"Enter Your ID: ";
cin>>Sid;

string com = "SELECT * FROM student WHERE Id = '"+Sid+"'";
 if (mysql_query(conn, com.c_str())) {
   cout << "Error: " << mysql_error(conn) << endl;
 }
 else{
MYSQL_RES* res;
res=mysql_store_result(conn);
if(res){
	int num= mysql_num_rows(res);
	if(num==1){
		cout<<"Student ID Found."<<endl;
	string Bname;
	cout<<"Enter Book Name: ";
	cin>>Bname;
	if(book(conn,Bname) > 0){
		int bookQuantity = book(conn,Bname)-1;
		stringstream ss;
		ss<<bookQuantity;
		string Sq = ss.str();

string upd ="UPDATE lib SET Quantity ='"+Sq+"' WHERE Name = '"+Bname+"' ";
if(mysql_query(conn,upd.c_str())){
	cout<<"Error: "<<mysql_error(conn)<<endl;
}
else{
	cout<<"Book is available. Borrowing Book...."<<endl;
}
	}
	else{
		cout<<"Book is not Available."<<endl;
	}
	}
else if(num==0){
	cout<<"This Student is Not Registered."<<endl;
}
}
mysql_free_result(res);
 }	
}

int main() {
	Student s;
	Library l;

MYSQL* conn;
conn = mysql_init(NULL);

if(!mysql_real_connect(conn,HOST, USER, PW,DB,3306,NULL,0)){
	cout<<"Error: "<<mysql_error(conn)<<endl;
}
else{
	cout<<"Logged In!"<<endl;
}
Sleep(3000);
bool exit = false;
while(!exit){
	system("cls");
	int val;
cout << "Welcome To Library Management System" << endl;
cout << "************************************" << endl;
cout << "1. Administration." << endl;
cout << "2. User." << endl;
cout << "0. Exit." << endl;
cout<<"Enter Choice: ";
cin>>val;

if(val==1){
	system("cls");
admin(conn,l,s);	
}//if1 main

else if(val==2){
user(conn,l,s);
Sleep(5000);	
}
else if(val==0){
	exit= true;
	cout<<"Good Luck!"<<endl;
	Sleep(3000);
}

}//while
mysql_close(conn);
	return 0;
}

