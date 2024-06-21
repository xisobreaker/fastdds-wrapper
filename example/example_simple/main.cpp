#include <cstring>
#include <iostream>
#include <memory>

#include "DDSConstants.h"
#include "DDSTestHandler.h"
#include "HelloWorldOne.h"

using namespace std;

void run_dds_data_writer();
void run_dds_data_reader();

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " sub/pub" << std::endl;
        return -1;
    }

    if (strcmp(argv[1], "sub") == 0) {
        run_dds_data_reader();
    } else if (strcmp(argv[1], "pub") == 0) {
        run_dds_data_writer();
    } else {
        std::cerr << "unknown command: " << argv[1] << std::endl;
    }
    return 0;
}

void processHelloWorldOne(std::shared_ptr<HelloWorldOne> data)
{
    std::cout << "recv message: " << data->index() << std::endl;
}

void run_dds_data_writer()
{
    DDSTestHandler handler(170);
    handler.initDomainParticipant("test_writer");
    DDSTopicDataWriter<HelloWorldOne> *dataWriter = handler.createDataWriter<HelloWorldOne>(DDS_TOPIC_HELLO_WORLD_ONE);

    bool runFlag = true;
    int  index = 0;

    std::thread([&]() {
        while (std::cin.get() != '\n') {
        }
        runFlag = false;
    }).detach();

    while (runFlag) {
        HelloWorldOne message;
        message.index(++index);
        message.points(std::vector<uint8_t>(100));
        if (dataWriter->writeMessage(message)) {
            std::cout << "send message: " << message.index() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void run_dds_data_reader()
{
    DDSTestHandler handler(170);
    handler.initDomainParticipant("test_reader");
    DDSTopicDataReader<HelloWorldOne> *dataReader =
        handler.createDataReader<HelloWorldOne>(DDS_TOPIC_HELLO_WORLD_ONE, processHelloWorldOne);

    while (std::cin.get() != '\n') {
    }
}
