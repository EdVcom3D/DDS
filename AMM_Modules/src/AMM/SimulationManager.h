

#include <mutex>
#include <thread>


#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/subscriber/SampleInfo.h>

#include "AMMPubSubTypes.h"

#include <fastrtps/Domain.h>

using namespace eprosima;
using namespace eprosima::fastrtps;

using namespace std;
using namespace std::chrono;

class SimulationManager {

public:
	SimulationManager();
	virtual ~SimulationManager();

    bool Init();
	void StartSimulation();
	void StopSimulation();
	void Shutdown();

	void SetSampleRate(int rate);
	int GetSampleRate();

	bool isRunning();

	int GetTickCount();

	void SendCommand(const std::string &command);

	void Cleanup();
	void TickLoop();

protected:

	std::thread m_thread;
	std::mutex m_mutex;
	bool m_runThread;

    Participant *mp_participant;

    Publisher *tick_publisher;
    Publisher *command_publisher;
    Publisher *node_publisher;

    Subscriber *tick_subscriber;
    Subscriber *command_subscriber;
    Subscriber *node_subscriber;

	int tickCount = 0;
	int sampleRate = 50;

    class PubListener : public PublisherListener
    {
    public:
        PubListener() : n_matched(0){};
        ~PubListener(){};
        void onPublicationMatched(Publisher* pub,MatchingInfo& info);
        int n_matched;
    } pub_listener;

    class NodeSubListener : public SubscriberListener
    {
    public:
        NodeSubListener() : n_matched(0),n_msg(0){};
        ~NodeSubListener(){};
        void onSubscriptionMatched(Subscriber* sub,MatchingInfo& info);
        void onNewDataMessage(Subscriber* sub);
        SampleInfo_t m_info;
        int n_matched;
        int n_msg;
    } node_sub_listener;

    AMM::Simulation::TickPubSubType tickType;
    AMM::Physiology::NodePubSubType nodeType;
    AMM::PatientAction::BioGears::CommandPubSubType commandType;

};

