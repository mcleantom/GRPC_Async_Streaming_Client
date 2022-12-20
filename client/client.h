#ifndef GRPC_ASYNC_STREAMING_CLIENT_H
#define GRPC_ASYNC_STREAMING_CLIENT_H

#include <iostream>
#include <ProtoObjects/helloworld.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <stdexcept>


class GreeterClient {
public:
    explicit GreeterClient(
        std::shared_ptr<grpc::Channel> channel
    ) : m_xStub(ProtoObjects::Greeter::NewStub(channel)) {};
    void SayHello(const std::string& user) {
        ProtoObjects::HelloRequest request;
        request.set_name(user);
    }

    void AsyncCompleteRPC() {
        // Infinite loop, servicing the completion queue and passing the logic onto the
        // ResponseHandler.
        void* got_tag;
        bool ok = false;

        while (mCompletionQueue.Next(&got_tag, &ok)) {
            ResponseHandler* responseHandler = static_cast<ResponseHandler*>(got_tag);
            responseHandler->HandleResponse(ok);
        }
    }
private:

    class ResponseHandler {
    public:
        virtual void HandleResponse(bool eventStatus) = 0;
    };

    class AsyncClientCall: public ResponseHandler {
    public:
        AsyncClientCall() : mCallStatus(CREATE) {};
        ProtoObjects::HelloReply mReply;
        grpc::ClientContext mContext;
        grpc::Status mStatus;
        std::unique_ptr<grpc::ClientAsyncReaderInterface<ProtoObjects::HelloReply>> m_xResponseReader;

        void HandleResponse(bool responseStatus) override {
            switch (mCallStatus) {
                case CREATE:
                    if (responseStatus) {
                        m_xResponseReader->Read(&mReply, (void*)this);
                        mCallStatus = PROCESS;
                    } else {
                        m_xResponseReader->Finish(&mStatus, (void*)this);
                        mCallStatus = FINISH;
                    }
                    break;
                case PROCESS:
                    if (responseStatus) {
                        std::cout << "Greeter recieved " << this << ": " << mReply.message() << std::endl;
                        m_xResponseReader->Read(&mReply, (void*)this);
                    } else {
                        m_xResponseReader->Finish(&mStatus, (void*)this);
                        mCallStatus = FINISH;
                    }
                    break;
                case FINISH:
                    if (mStatus.ok()) {
                        std::cout << "Server response completed." << std::endl;
                    } else {
                        std::cerr << "RPC Failed." << std::endl;
                    }
                    delete this;
            }
        };
    private:
        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus mCallStatus;
    };

    std::unique_ptr<ProtoObjects::Greeter::Stub> m_xStub;
    grpc::CompletionQueue mCompletionQueue;
};

#endif