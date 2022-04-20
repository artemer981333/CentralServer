#include "lfrconnection.h"


LFRConnection::LFRConnection(boost::asio::io_context &context, LFRConnectionsManager *manager)
	: socket(context), manager(manager)
{
}

bool LFRConnection::isConnected() const
{
	return connected;
}

void LFRConnection::sayHello()
{
	std::string msg = "Hello!";
	socket.async_write_some(boost::asio::buffer(msg.data(), msg.size()), [](const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (error)
		{
			//log debug << BIG ERROR
			return;
		}
		//log info << bytes_transferred send
	});
}

bool LFRConnection::isRunning() const
{
	return running;
}

void LFRConnection::stop()
{
	running = false;
}

void LFRConnection::personalCardAdded(const PersonalCard &card)
{
	queriesMutex.lock();
	queries.push(Query(0, card));
	queriesMutex.unlock();
}

void LFRConnection::personalCardEdited(const PersonalCard &card)
{
	queriesMutex.lock();
	queries.push(Query(1, card));
	queriesMutex.unlock();
}

void LFRConnection::personalCardDeleted(const PersonalCard &card)
{
	queriesMutex.lock();
	queries.push(Query(2, card));
	queriesMutex.unlock();
}

PassingEvent::PassingEvent(QUuid id, bool enterance, bool passed, QDateTime time)
	: id(id), enterance(enterance), passed(passed), time(time)
{}

Query::Query(int type, const PersonalCard &card)
	: type(type), card(card)
{}
