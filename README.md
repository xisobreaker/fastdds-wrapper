# fastdds-wrapper

## 安装说明

操作系统：ubuntu 16.04

`Fast DDS` 库以及所需依赖版本

- foonathan_memory_vendor **(v1.0.0)** 下载地址：[https://github.com/eProsima/foonathan_memory_vendor](https://github.com/eProsima/foonathan_memory_vendor)
- Fast-CDR **(v1.0.19)** 下载地址：[https://github.com/eProsima/Fast-CDR](https://github.com/eProsima/Fast-CDR)
- Fast-DDS **(v2.2.1)** 下载地址：[https://github.com/eProsima/Fast-DDS](https://github.com/eProsima/Fast-DDS)

#### 安装准备

安装 fastdds 库之前需要首先安装 asio 及 tinyxml2 库。fastdds 从 2.2.0 版本开始， asio 库需要 1.10.8 及以上版本，由于 ubuntu 16.04 系统的 apt 管理软件只支持到 1.10.6 版本，因此需要手动编译进行安装。

asio 下载地址：[https://github.com/chriskohlhoff/asio](https://github.com/chriskohlhoff/asio)

asio 的编译需要 `autoconf` 工具，执行以下命令安装：

```bash
sudo apt install -y autoconf
```

安装完成后进行 asio 的编译安装：

```bash
# 生成 configure
./autogen.sh
# 检查依赖，生成 makefile
./configure
# 代码编译及安装
make && make install
```

tinyxml2 可通过如下命令进行安装：

```bash
sudo apt install -y libtinyxml2-dev
```

#### Foonathan Memory

在构建 **Fast DDS** 之前，需要先安装 **Foonathan Memory** 依赖

```bash
git clone https://github.com/eProsima/foonathan_memory_vendor.git -b v1.0.0 foonathan_memory_vendor
cd foonathan_memory_vendor
mkdir build && cd build
cmake ..
cmake --build . --target install
```

Foonathan Memory 在编译时，会从 github 克隆 foonathan/memory 源码，由于公司网络问题无法下载成功，可在 CMakeLists.txt 中找到如下代码进行注释

```cmake
  externalproject_add(foo_mem-ext
  GIT_REPOSITORY https://github.com/foonathan/memory.git
  GIT_TAG v0.7-1
  TIMEOUT 600
  # Avoid the update (git pull) and so the recompilation of foonathan_memory library each time.
  UPDATE_COMMAND ""
  CMAKE_ARGS
    -DFOONATHAN_MEMORY_BUILD_EXAMPLES=${BUILD_MEMORY_EXAMPLES}
    -DFOONATHAN_MEMORY_BUILD_TESTS=${BUILD_MEMORY_TESTS}
    -DFOONATHAN_MEMORY_BUILD_TOOLS=${BUILD_MEMORY_TOOLS}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/foo_mem_ext_prj_install
    ${extra_cmake_args}
    -Wno-dev
    ${PATCH_COMMAND_STR}
  )
```

在已编译好的源码包中，清空 build 文件夹下，除 foo_mem_ext_prj_install 目录外的所有文件，之后执行如下命令进行安装：

```bash
cmake ..
sudo make install
```

#### Fast CDR

`Fast CDR` 安装命令如下：

```bash
git clone --recursive https://github.com/eProsima/Fast-CDR.git -b v1.0.19 Fast-CDR-1.0.19
cd Fast-CDR-1.0.19
mkdir build && cd build

cmake ..
make && make install
```

#### Fast RTPS(DDS)

Fast DDS 安装命令：

```bash
git clone --recursive https://github.com/eProsima/Fast-DDS.git -b v2.2.1 Fast-DDS-2.2.1
cd Fast-DDS-2.2.1
mkdir build && cd build

cmake -DTHIRDPARTY=ON -DSECURITY=ON ..
make && make install
```

#### Fast DDS Gen

Fast DDS Gen 编译需要 java 环境，通过如下命令安装 jdk8

```bash
sudo apt install openjdk-8-jdk
```

Fast DDS Gen 依赖于 IDL-Parser **(v1.0.0)**，编译时会从 github 拉取对应代码，通过如下方式跳过此步骤
将下载的 IDL-Parser 解压，之后将其拷贝到 Fast DDS Gen 的 thirdparty 目录下，并重命名为 idl-parser
修改 Fast DDS Gen 目录下的 build.gradle，找到如下代码

```java
buildIDLParser.dependsOn submodulesUpdate
```

删去后面的 submodulesUpdate，将其修改为如下所示：

```java
buildIDLParser.dependsOn
```

之后执行如下命令进行编译及安装

```bash
# 执行编译
./gradlew assemble

# 执行安装
./gradlew install
```

## Fast DDS 使用

eprosima API 地址：[https://fast-dds.docs.eprosima.com/](https://fast-dds.docs.eprosima.com/)

#### dds pubsub demo 示例

使用 cmake 工具来构建项目，新建 CMakeLists.txt 文件，复制如下内容

```cmake
cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(DDSHelloWorld)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()
```

创建 HelloWorld.idl 文件，定义一个 HelloWorld 类型

```idl
struct HelloWorld
{
    unsigned long index;
    string message;
};
```

执行 `fastddsgen` 命令生成代码

```bash
fastddsgen HelloWorld.idl
```

执行 `fastddsgen` 命令后会生成如下文件：

- HelloWorld.cxx: HelloWorld 类型定义.
- HelloWorld.h: HelloWorld 头文件.
- HelloWorldPubSubTypes.cxx: HelloWorld 类型的序列化及反序列化代码.
- HelloWorldPubSubTypes.h: HelloWorldPubSubTypes 头文件.

在 CMakeList 文件的末尾包含以下代码段：

```c++
message(STATUS "Configuring HelloWorld publisher/subscriber example...")
file(GLOB DDS_HELLOWORLD_SOURCES_CXX "src/*.cxx")
```

publisher 代码示例

```c++
#include "HelloWorldPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

using namespace eprosima::fastdds::dds;

class HelloWorldPublisher
{
private:
    HelloWorld hello_;
    DomainParticipant *participant_;
    Publisher *publisher_;
    Topic *topic_;
    DataWriter *writer_;
    TypeSupport type_;

    class PubListener : public DataWriterListener
    {
    public:
        PubListener() : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(DataWriter *, const PublicationMatchedStatus &info) override
        {
            if (info.current_count_change == 1) {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            } else if (info.current_count_change == -1) {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change << " is not a valid value for PublicationMatchedStatus current count change."
                          << std::endl;
            }
        }

        std::atomic_int matched_;

    } listener_;

public:
    HelloWorldPublisher() : participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new HelloWorldPubSubType())
    {
    }

    virtual ~HelloWorldPublisher()
    {
        if (writer_ != nullptr) {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr) {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //! Initialize the publisher
    bool init()
    {
        hello_.index(0);
        hello_.message("HelloWorld");

        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
        if (participant_ == nullptr) {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the publications Topic
        topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) {
            return false;
        }

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr) {
            return false;
        }

        // Create the DataWriter
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
        if (writer_ == nullptr) {
            return false;
        }
        return true;
    }

    //! Send a publication
    bool publish()
    {
        if (listener_.matched_ > 0) {
            hello_.index(hello_.index() + 1);
            writer_->write(&hello_);
            return true;
        }
        return false;
    }

    //! Run the Publisher
    void run(uint32_t samples)
    {
        uint32_t samples_sent = 0;
        while (samples_sent < samples) {
            if (publish()) {
                samples_sent++;
                std::cout << "Message: " << hello_.message() << " with index: " << hello_.index() << " SENT" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main(int argc, char **argv)
{
    std::cout << "Starting publisher." << std::endl;
    int samples = 10;

    HelloWorldPublisher *mypub = new HelloWorldPublisher();
    if (mypub->init()) {
        mypub->run(static_cast<uint32_t>(samples));
    }

    delete mypub;
    return 0;
}
```

subscriber 代码示例

```c++
#include "HelloWorldPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

using namespace eprosima::fastdds::dds;

class HelloWorldSubscriber
{
private:
    DomainParticipant *participant_;
    Subscriber *subscriber_;
    DataReader *reader_;
    Topic *topic_;
    TypeSupport type_;

    class SubListener : public DataReaderListener
    {
    public:
        SubListener() : samples_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(DataReader *, const SubscriptionMatchedStatus &info) override
        {
            if (info.current_count_change == 1) {
                std::cout << "Subscriber matched." << std::endl;
            } else if (info.current_count_change == -1) {
                std::cout << "Subscriber unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change << " is not a valid value for SubscriptionMatchedStatus current count change"
                          << std::endl;
            }
        }

        void on_data_available(DataReader *reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&hello_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
                    samples_++;
                    std::cout << "Message: " << hello_.message() << " with index: " << hello_.index() << " RECEIVED." << std::endl;
                }
            }
        }

        HelloWorld hello_;
        std::atomic_int samples_;
    } listener_;

public:
    HelloWorldSubscriber()
        : participant_(nullptr), subscriber_(nullptr), topic_(nullptr), reader_(nullptr), type_(new HelloWorldPubSubType())
    {
    }

    virtual ~HelloWorldSubscriber()
    {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //! Initialize the subscriber
    bool init()
    {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr) {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);
        // Create the subscriptions Topic
        topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        if (subscriber_ == nullptr) {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);
        if (reader_ == nullptr) {
            return false;
        }
        return true;
    }

    //! Run the Subscriber
    void run(uint32_t samples)
    {
        while (listener_.samples_ < samples) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

int main(int argc, char **argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    HelloWorldSubscriber *mysub = new HelloWorldSubscriber();
    if (mysub->init()) {
        mysub->run(static_cast<uint32_t>(samples));
    }

    delete mysub;
    return 0;
}
```

未指定 transport 时，框架将默认使用 udp transport，数据包过大时容易发生数据丢包的情况，需指定为 tcp 模式
其他代码示例可参见 Fast DDS 源码的 examples 目录

#### shared memory 模式使用注意事项

使用 fastdds 的共享内存模式时，当强行退出程序，没有调用 `Domain::stopAll()` 函数时，会导致共享内存未释放，影响程序正常运行。程序申请的共享内存文件会放置在 /dev/shm 文件夹内，此文件夹的文件都是保存在内存中的，不会持久化到磁盘，当服务器重启后会自动清理掉，也可通过手动删除的方式进行清理。

在程序中，可通过捕获信号的方式来进行释放，可参见如下代码：

```c++
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signum) {
    Domain::stopAll();
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    int signals[] = {SIGHUP, SIGINT, SIGSEGV};
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    for (int sig : signals) {
        if (sigaction(sig, &act, nullptr)) {
            printf("信号注册失败：%d\n", sig);
            exit(EXIT_FAILURE);
        }
    }

    Domain::stopAll();
    return 0;
}
```

fastddsgen 版本不同生成的代码文件有差异，新版生成器对数据大小有校验，需要注意
