SRC = ./src
IDIR = ./include
BIN = ./bin
ODIR = ./obj
EXECUTABLE = server

LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares`

CXX = g++ -Ofast
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17 -ggdb

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: server cleanup

$(EXECUTABLE): chessEngine.pb.o chessEngine.grpc.pb.o server.o Board.o ChessGame.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
	python3 -m grpc_tools.protoc --proto_path=. --python_out=. --grpc_python_out=. $<

%.pb.cc: %.proto
	protoc --cpp_out=. --python_out=. $<

cleanup:
	rm -f *.o

clean:
	rm -f *.o *.pb.cc *.pb.h *_pb2.py *grpc.py server