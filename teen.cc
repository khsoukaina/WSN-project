import random
import math
import time

DEBUG = True

class TEENHeader:
    offset_ = None

class TEEN:
    def __init__(self, id):
        self.index = id
        self.rlink_ = -1
        self.flink_ = -1
        self.flag_ = 0
        self.ultimated_ = 0
        self.result_ = 0
        self.seqno = 1

        if DEBUG:
            print(f"Node {self.index} is set, starting broadcasting.")

    def command(self, argc, argv):
        if argc == 4:
            if argv[1].lower().startswith("start"):
                source = int(argv[2])
                dest = int(argv[3])
                print(f"Source = {source}, Dest = {dest}")
                if dest != 0:
                    self.send_req(source, dest)
                return "TCL_OK"
        if argc == 2:
            if argv[1].lower() == "start":
                source = int(argv[2])
                print(f"source={source}")
                self.call_creset(source)
                return "TCL_OK"
        if argc == 3:
            if argv[1].lower() == "index":
                self.index = int(argv[2])
                return "TCL_OK"
            elif argv[1].lower().startswith("start"):
                source = int(argv[2])
                print(f"source={source}")
                self.call_creset(source)
                return "TCL_OK"
            elif argv[1].lower() in ["log-target", "tracetarget"]:
                logtarget = argv[2]
                if logtarget is None:
                    return "TCL_ERROR"
                return "TCL_OK"
            elif argv[1].lower() == "drop-target":
                return "TCL_OK"
            elif argv[1].lower() == "if-queue":
                ifqueue = argv[2]
                if ifqueue is None:
                    return "TCL_ERROR"
                return "TCL_OK"
            elif argv[1].lower() == "port-dmux":
                dmux_ = argv[2]
                if dmux_ is None:
                    print(f"Error: {argv[1]} lookup of {argv[2]} failed")
                    return "TCL_ERROR"
                return "TCL_OK"
        return "TCL_ERROR"

    def send_req(self, src, dest):
        print(f"\n\nSource={src}")
        if self.rlink_ == -1:
            if self.flag_ == 0:
                self.flag_ = 1

        self.ultimated_ = dest
        if self.index == src:
            # Build and send request packet (simplified)
            packet = {"src_nodeid": self.index, "pkt_type": "TEEN_REQ", "pkt_id": self.seqno}
            self.seqno += 1
            self.schedule(packet)

    def recv_req(self, packet):
        if packet["src_nodeid"] == self.index:
            return

        if self.flag_ == 0:
            self.flag_ = 1
            self.rlink_ = packet["src_nodeid"]
            print(f"Rlink of {self.index} is {self.rlink_}")
            self.send_resp(self.rlink_, self.index)
        else:
            print(f"\nNode {self.index} already has a leader {self.rlink_}")
            return

    def send_resp(self, tnodeid, pnodeid):
        # Build and send response packet (simplified)
        packet = {"src_nodeid": self.index, "desti_nodeid": self.rlink_, "pkt_type": "TEEN_RESP", "pkt_id": self.seqno}
        self.seqno += 1
        self.schedule(packet)

    def recv_resp(self, packet):
        if packet["src_nodeid"] == self.index:
            return

        if packet["src_nodeid"] == self.ultimated_:
            if self.rlink_ == -1:
                self.flink_ = packet["src_nodeid"]
                print("Route connected")
            else:
                self.result_ = 1
                self.flink_ = packet["src_nodeid"]
                print(f"Flink of {self.index} is {self.flink_}")
                self.send_result(self.rlink_, self.result_)

        else:
            self.send_cont(packet["src_nodeid"], self.ultimated_)

    def send_cont(self, desti, ulti):
        # Build and send continue packet (simplified)
        packet = {"src_nodeid": self.index, "udesti_nodeid": ulti, "pkt_type": "TEEN_CONT", "pkt_id": self.seqno}
        self.seqno += 1
        self.schedule(packet)

    def recv_cont(self, packet):
        self.ultimated_ = packet["udesti_nodeid"]
        if packet["src_nodeid"] == self.rlink_:
            print(f"Received cont packet from {packet['src_nodeid']}")
            self.send_req(self.index, self.ultimated_)
        else:
            return

    def send_result(self, leader, res):
        # Build and send result packet (simplified)
        packet = {"src_nodeid": self.index, "desti_nodeid": leader, "pkt_type": "TEEN_RESULT", "pkt_id": self.seqno, "route_info": res}
        self.seqno += 1
        self.schedule(packet)

    def recv_result(self, packet):
        if packet["route_info"] == 1 and self.rlink_ != -1:
            self.flink_ = packet["src_nodeid"]
            print(f"Flink of {self.index} is {self.flink_}")

    def schedule(self, packet, delay=0.0):
        # Simulate scheduling the packet (this would normally involve network stack interaction)
        time.sleep(delay)
        print(f"Scheduled packet {packet['pkt_type']} with ID {packet['pkt_id']}")

    def call_creset(self, source):
        print(f"Resetting source {source}")

# Example of creating a TEEN object and using its methods
teen_node = TEEN(id=1)
teen_node.command(4, ["start", "1", "0", "5"])
