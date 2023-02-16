
import time
import numpy as np

import grpc
from protos.boat_msg_pb2 import BoatControl, BoatState, BoatRequest
from protos.boat_msg_pb2_grpc import BoatServiceStub


def run():
    print("Attempting to communicate with Sim")
    with grpc.insecure_channel('localhost:9002') as channel:

        stub = BoatServiceStub(channel)
        response = stub.SayHello(BoatRequest(name="boaty_mcboatface"))
        print("Greeter client received: " + response.message)

        # Determine how fast gRPC can go
        start_time = -1
        times = []
        for i in range(100):
            state = stub.ControlBoat(BoatControl(power=0.5, rudder=-1.0))
            if start_time < 0:
                start_time = state.t
            else:
                elapsed_time = state.t - start_time
                times.append(elapsed_time)
                start_time = state.t
            print(state.t)

        # Report average time
        avg_time = np.average(times)
        min_time = np.min(times)
        max_time = np.max(times)
        print(f"Average time: {avg_time}\nMin: {min_time}\nMax: {max_time}")


if __name__ == "__main__":
    run()
