#ifndef LFRCONNECTION_H
#define LFRCONNECTION_H
#include <boost/asio.hpp>
#include <QUuid>
#include <QDateTime>
#include <queue>

struct PassingEvent
{
	PassingEvent(QUuid id, bool enterance, bool passed, QDateTime time);
	QUuid id;
	bool enterance;
	bool passed;
	QDateTime time;
};

struct PersonalCard
{
	QUuid id;
	QString imagePath;
	QString name, surname, lastname;
	QString post, subdivision;
};

struct Query
{
	Query(int type, const PersonalCard &card);
	int type;	//0 - add, 1 - edit, 2 - delete
	PersonalCard card;
};

class LFRConnectionsManager;

class LFRConnection
{
public:
	LFRConnection(boost::asio::io_context &context, LFRConnectionsManager *manager);

	bool isConnected() const;

	void sayHello();

	void start();

	bool isRunning() const;

	void stop();

	void personalCardAdded(const PersonalCard &card);
	void personalCardEdited(const PersonalCard &card);
	void personalCardDeleted(const PersonalCard &card);

	void restartQuery();

	friend class LFRConnectionsManager;

private:

	void checkConnection();

	bool recvPassingEvent(PassingEvent &event);
	bool recvPersonalCard(PersonalCard &card);
	bool sendPersonalCard(const PersonalCard &card);

	bool sendRestartQuery();

	std::string userName;	//user information

	boost::asio::ip::tcp::socket socket;
	bool connected;
	bool running;

	std::queue<Query> queries;
	std::mutex queriesMutex;

	std::shared_ptr<LFRConnectionsManager> manager;

	QDateTime lastSyncTime;
};

#endif // LFRCONNECTION_H
