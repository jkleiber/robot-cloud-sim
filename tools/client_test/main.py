
import grpc
from protos.boat_msg_pb2 import BoatControl
from protos.boat_rpc_pb2 import BoatRequest
from protos.boat_rpc_pb2_grpc import BoatServiceStub


def run():
    print("Attempting to communicate with Sim")
    with grpc.insecure_channel('localhost:9002') as channel:

        stub = BoatServiceStub(channel)
        response = stub.SayHello(BoatRequest(name="boaty_mcboatface"))
        print("Greeter client received: " + response.message)

        state = stub.ControlBoat(BoatControl(power=1.0, rudder=0.0))


if __name__ == "__main__":
    run()
