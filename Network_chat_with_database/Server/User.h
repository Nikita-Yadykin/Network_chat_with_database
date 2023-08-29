#pragma once
#include<string>

using namespace std;

class User
{
public:

	User(int id, const string& login, string& password, string& name);
	~User() = default;

	string getLogin() const; // Получить логин
	string getPassword() const; // Получить пароль
	string getName() const; // Получить имя
	int getID() const; // Получить id
private:
	int _id;
	const string _login;
	string _password;
	string _name;

};
