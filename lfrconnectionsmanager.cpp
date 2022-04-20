#include "lfrconnectionsmanager.h"

LFRConnectionsManager::LFRConnectionsManager(boost::asio::ip::tcp::endpoint ep)
	: acceptor(context, ep)
{
}

void LFRConnectionsManager::accepting()
{
	connections.push_back(std::shared_ptr<LFRConnection>(new LFRConnection(context)));
	acceptor.async_accept(connections.back()->socket, boost::bind<void>([&](std::shared_ptr<LFRConnection> connection, const boost::system::error_code& error)
	{
							  acceptNewConnection(connection, error);
							  return;
						  }, connections.back(), _1));
}

void LFRConnectionsManager::run()
{
	context.run();
}

void LFRConnectionsManager::acceptNewConnection(std::shared_ptr<LFRConnection> connection, const boost::system::error_code &error)
{
	if (error)
	{
		//BIG ERROR
		return;
	}
	//New connection accepted
	connection->sayHello();
	connections.push_back(std::shared_ptr<LFRConnection>(new LFRConnection(context)));
	acceptor.async_accept(connections.back()->socket, boost::bind<void>([&](std::shared_ptr<LFRConnection> connection, const boost::system::error_code& error)
	{
							  acceptNewConnection(connection, error);
							  return;
						  }, connections.back(), _1));
}
