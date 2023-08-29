#include"User.h"

User::User(int id, const string& login, string& password, string& name) : _id(id), _login(login), _password(password), _name(name)
{
}

string User::getLogin() const
{
	return _login;
}
string  User::getPassword() const
{
	return _password;
}
string  User::getName() const
{
	return _name;
}
int  User::getID() const
{
	return _id;
}