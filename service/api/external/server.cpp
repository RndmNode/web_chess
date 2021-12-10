#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chessEngine.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chessEngine::ChessEngine;
using chessEngine::fenRequest;
using chessEngine::fenResponse;

class ChessEngineImplementation final : public ChessEngine::Service {
    Status getFEN(
        ServerContext* context, 
        const fenRequest* request, 
        fenResponse* response
    ) override {
        std::string position = request->fen();
        std::string reply = "Hello from the server! You gave me this position: " + position;
        response->set_fen(reply);

        return Status::OK;
    } 
};

void Run(){
    std::string address("0.0.0.0:8080");
    ChessEngineImplementation service;

    ServerBuilder builder;

    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << address << std::endl;

    server->Wait();
}

int main(int argc, char** argv){
    Run();

    return 0;
}