#include"ConsoleChat.h"
#include<iostream>
#include "sha1.h" // Подключаем заголовочный файл для хеширования SHA-1
#include <cstring>
#include <string>
#ifdef _WIN64
#include <windows.h>
#endif
using namespace std;

sql::Driver* ConsoleChat::driver = 0;
sql::Connection* ConsoleChat::con = 0;

void ConsoleChat::initdb() {
	/* Create a connection */
	driver = get_driver_instance();
	try {
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		/* Connect to the MySQL test database */
		con->setSchema("chatdb");
	}
	catch (sql::SQLException ex) {
		cout << ex.getErrorCode();
	}
}

void ConsoleChat::freedbres() {
	if (con != 0) delete con;
	con = 0;
}

void ConsoleChat::start()
{
	_сhatStarted = true;
}

void ConsoleChat::startMenu() // Стартовое меню, отображается при запуске
{
#ifdef _WIN64
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
#endif
	ns << "\tWelcome to chat!\n\n"
		<< "Choose an action to continue: \n\n"
		<< "1-Register\n"
		<< "2-Enter chat\n"
		<< "3-Leave the chat\n";
	string str;
	char action = '0';

	ns >> str;

	if (!str.empty())
		action = '0';

	action = str[0];

	switch (action)
	{
	case '1':
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 10);
#endif
		signUp(); // Зарегистрироваться
		break;
	case '2':

		logIn(); // Войти
		break;
	case '3':
		_сhatStarted = false; // Выход из чата
		ns << "CDATA:CMD:EXIT\n";
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 15);
#endif
		break;
	default:
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 12);
#endif
		ns << "Please check that the characters you are entering are correct.\n";
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 11);
#endif
		break;
	}

};

shared_ptr<User> ConsoleChat::getUserByLogin(const string login) const
{
	shared_ptr<User> result = nullptr;

	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res = 0;
	res = stmt->executeQuery("SELECT id,create_time,name,login,password FROM table_users");
	while (res->next()) {
		string clogin = res->getString("login");
		if (clogin == login) {
			string cpwd = res->getString("password");
			string cname = res->getString("name");
			int cid = res->getInt("id");
			result = shared_ptr<User>(new User(cid, clogin, cpwd, cname));
			break;
		}
	}
	delete res;
	delete stmt;

	return result;
}

shared_ptr<User>  ConsoleChat::getOnlineUser() const
{
	return _onlineUser;
}

void ConsoleChat::logIn() // Вход в чат
{
	string login;
	string password;

	ns << "Please enter a login: ";
	ns >> login;
	ns << "Enter password: ";
	ns >> password;

	_onlineUser = getUserByLogin(login); // Указатель на онлайн пользователя получает указатель на пользователя с указанным логином

	if (_onlineUser == nullptr || !verifyPassword(password, _onlineUser->getPassword())) // Если нет онлайн пользователя или указанный пароль не соответствует хэшу пароля онлайн пользователя
	{
		_onlineUser = nullptr; // Сообщаем, что онлайн пользователь отсутствует
#ifdef _WIN64
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
#endif
		ns << "Invalid login or password, please try again\n";
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 11);
#endif
	}
}

void ConsoleChat::signUp() // Регистрация
{
#ifdef _WIN64
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
#endif
	string login, password, name;

	ns << "Please enter your login: ";
	ns >> login;
	ns << "Choose security password: ";
	ns >> password;
	ns << "Enter your name: ";
	ns >> name;

	if (login == "" || name == "") {
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 12);
#endif
		ns << "\n\n (" << "fields login and name cannot be empty!" << ") \n\n";
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 15);
#endif
		return;
	}

	try
	{
		if (getUserByLogin(login) || login == "all") // Если пользователь с таким логином существует или логин = all
		{
			throw LoginException("error: "); // Уникальность логина
		};
	}
	catch (const LoginException& ex) // Ловим выброшенное исключение
	{
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 12);
#endif
		ns << "\n\n (" << ex.what() << ") \n\n"; // Вызываем метод what()
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 15);
#endif
		return;
	}

	string hashedPassword = hashPassword(password); // Хешируем пароль

	User user = User(-1, login, hashedPassword, name); // Создаем нового пользователя

	shared_ptr<User> result = nullptr;
	sql::Statement* stmt = con->createStatement();
	stmt->execute("insert into table_users (name, login, password) values('" +
		name + "','" +
		login + "','" +
		hashedPassword + "')");

	delete stmt;

	_onlineUser = getUserByLogin(login); // Новый пользователь становится онлайн пользователем
	ns << "\nCongratulations! You have successfully registered!\n";
}

string ConsoleChat::hashPassword(const string& password) // Функция для хеширования пароля
{
	uint* hashedPassword = sha1(password.c_str(), password.length());
	string hashStr;

	for (int i = 0; i < SHA1HASHLENGTHUINTS; ++i) {
		// Преобразуем каждое 32-битное слово хеша в строку шестнадцатеричного представления
		hashStr += to_string(hashedPassword[i]);
	}

	// Удаляем массив с хешем
	delete[] hashedPassword;

	return hashStr;
}

bool ConsoleChat::verifyPassword(const string& password, const string& hashedPassword) // Функция для проверки соответствия хэша пароля
{
	string hashedInput = hashPassword(password); // Хешируем введенный пароль
	return (hashedInput == hashedPassword); // Сравниваем хэши
}


void ConsoleChat::openChat() // Просмотр чата и списка пользователей
{
#ifdef _WIN64
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
#endif
	string sender, recipient;

	ns << "-------users chatting online-----\n";
	ns << "login  \t\t  name  \n";
	ns << "\n---------------------------------\n";

	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res = 0;
	res = stmt->executeQuery("SELECT id,create_time,name,login,password FROM table_users");
	while (res->next()) {
		string clogin = res->getString("login");
		string cpwd = res->getString("password");
		string cname = res->getString("name");

		ns << clogin << "\t---\t" << cname << "\n"; // Выводим логин и имя пользователя
	}
	delete res;

	ns << "\n--------messages-----------------\n\n";
	ns << " -----------chat-------  \n";

	res = stmt->executeQuery("SELECT id,create_time,letter,sender_id,(select login from table_users where table_users.id=table_messages.sender_id limit 1) as sender_login, recepient_id, (select login from table_users where table_users.id=table_messages.recepient_id limit 1) as recepient_login FROM table_messages");
	while (res->next()) {
		string cletter = res->getString("letter");
		string csender = res->getString("sender_login");
		string crecepient = res->getString("recepient_login");

		const Message message(csender, crecepient.empty() ? "all" : crecepient, cletter);
		if (_onlineUser->getLogin() == message.getSender() || _onlineUser->getLogin() == message.getRecipient() || message.getRecipient() == "all") // Если логин онлайн пользователя равен логину отправителя или логину получателя или равен "all"
		{
			sender = (_onlineUser->getLogin() == message.getSender()) ? _onlineUser->getLogin() : getUserByLogin(message.getSender())->getName(); // Устанавливаем отправителя

			if (message.getRecipient() == "all") // Если сообщение предназначено для всех
			{
				recipient = "all";
			}
			else // Если сообщение для конкретного пользователя
			{
				recipient = (_onlineUser->getLogin() == message.getRecipient()) ? _onlineUser->getLogin() : getUserByLogin(message.getRecipient())->getName(); // Устанавливаем получателя
			}
			ns << "message from " << sender << ": to " << recipient << "\n";
			ns << " - " << message.getLetter() << " - \n";
		}
	}
	delete res;
	ns << "---------------------------\n";

	delete stmt;
}

void ConsoleChat::sendMessage() // Написать сообщение
{
#ifdef _WIN64
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	string recipient, text;
	ns << "to (login or all) - ";
	ns >> recipient; // Указать логин получателя или all
	ns << ">> ";
	ns >> text;

	if (!(recipient == "all" || getUserByLogin(recipient))) {  // Если получатель не равен all или не найден логин пользователя

#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 12);
#endif
		ns << "error no such user  -- " << recipient << "\n";
#ifdef _WIN64
		SetConsoleTextAttribute(hConsole, 11);
#endif
		return;
	}

	sql::Statement* stmt = con->createStatement();
	if (recipient == "all") { // Если сообщение для всех
		stmt->execute("insert into table_messages(letter,sender_id) values('" + text + "'," + std::to_string(_onlineUser->getID()) + ")");
	}
	else {
		stmt->execute("insert into table_messages(letter,sender_id,recepient_id) values('" + text + "'," +
			std::to_string(_onlineUser->getID()) + ", " +
			std::to_string(getUserByLogin(recipient)->getID()) + ")");
	}
	delete stmt;
	ns << "message send -  " << recipient << ":\n ";
}

void ConsoleChat::chatMenu() // Меню чата
{
#ifdef _WIN64
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
#endif
	ns << "Welcome " << _onlineUser->getName() << "\n";
	while (_onlineUser) // Пока есть онлайн пользователь
	{
		ns << "Choose an action: \n"
			<< "1-Group chat \n"
			<< "2-Write a message \n"
			<< "3-Exit \n";
		string str;
		char action;

		ns >> str;

		if (!str.empty())
			action = '0';

		action = str[0];

		switch (action)
		{
		case '1':
			openChat(); // Открыть чат
			break;
		case '2':
			sendMessage(); // Написать сообщение
			break;
		case '3':
			_onlineUser = nullptr; // Выход
			break;
		default:
			ns << "Please try again\n";
			break;
		}
	}
}