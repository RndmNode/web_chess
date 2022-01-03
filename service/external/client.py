import grpc
import service.external.chessEngine_pb2 as pb2
import service.external.chessEngine_pb2_grpc as grpy

PORT = 8080

class ChessEngineClient(grpy.ChessEngineServicer):
    def __init__(self):
        # setup the port information
        self.host = 'localhost'
        self.server_port = PORT

        # instantiate the channel
        self.channel = grpc.insecure_channel(
            '{}:{}'.format(self.host, self.server_port), 
            options=(('grpc.enable_http_proxy', 0),))
        
        # bind the client and server
        self.stub = grpy.ChessEngineStub(self.channel)
    
    def Get_AI_Move(self, position, depth):
        request = pb2.fenRequest(fen=position, depth=depth)
        return self.stub.Get_AI_Move(request)

def getAI_move(position, depth):
    client = ChessEngineClient()
    return client.Get_AI_Move(position, depth)

if __name__ == '__main__':
    response = getAI_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    print(response)