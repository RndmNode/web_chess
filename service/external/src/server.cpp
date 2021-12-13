#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include "chessEngine.grpc.pb.h"
#include "engine.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chessEngine::ChessEngine;
using chessEngine::fenRequest;
using chessEngine::fenResponse;

class ChessEngineImplementation final : public ChessEngine::Service {
    Status Get_AI_Move(
        ServerContext* context, 
        const fenRequest* request, 
        fenResponse* response
    ) override {
        std::string position = request->fen();
        std::string reply = getAI_move(position);
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