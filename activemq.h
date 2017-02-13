#ifndef ACTIVEMQ_H
#define ACTIVEMQ_H

#include <activemq/transport/DefaultTransportListener.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
//#include <activemq/core/ActiveMQConnection.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Integer.h>
#include <activemq/util/Config.h>
#include <decaf/util/Date.h>



using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;
using namespace transport;

class ActiveMQProduce
{
public:
    ActiveMQProduce();
    virtual ~ActiveMQProduce();
    void start( const std::string& brokerURI, unsigned int numMessages, const std::string& destURI, bool useTopic, bool clientAck);
    void send(const char* bytesMessage,int nSize);

private:
    void cleanup();
    virtual void initialize();

private:
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;
    bool useTopic;
    bool clientAck;
    unsigned int numMessages;
    std::string brokerURI;
    std::string destURI;
    BytesMessage *bytesMessage;
};

class ActiveMQConsumer:public ExceptionListener,public MessageListener,public DefaultTransportListener
{

public:
    ActiveMQConsumer();
    virtual ~ActiveMQConsumer();

    void start( const std::string& brokerURI, const std::string& destURI, bool useTopic, bool clientAck );
    void close();
    void runConsumer();
    virtual void onMessage( const Message* message );
    virtual void onException( const CMSException& ex AMQCPP_UNUSED );
    virtual void transportInterrupted();
    virtual void transportResumed();
private:
    void cleanup();
private:
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    MessageProducer* producer;
    bool useTopic;
    bool clientAck;
    std::string brokerURI;
    std::string destURI;
};
#endif // ACTIVEMQ_H
