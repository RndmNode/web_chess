import grpc
import api.external.chessEngine_pb2 as pb2
import api.external.chessEngine_pb2_grpc as grpy

PORT = 8080

class ChessEngineClient(grpy.ChessEngineServicer):
    def __init__(self):
        # setup the port information
        self.host = 'localhost'
        self.server_port = PORT

        # instantiate the channel
        self.channel = grpc.insecure_channel(
            '{}:{}'.format(self.host, self.server_port))
        
        # bind the client and server
        self.stub = grpy.ChessEngineStub(self.channel)
    
    def getFEN(self, position):
        request = pb2.fenRequest(fen=position)
        return self.stub.getFEN(request)

if __name__ == '__main__':
    client = ChessEngineClient()
    result = client.getFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq")
    print(f'{result}')