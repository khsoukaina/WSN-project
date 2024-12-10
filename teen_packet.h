import struct

# Define constants for packet types
TEEN_REQ = 0x01
TEEN_RESP = 0x02
TEEN_CONT = 0x03
TEEN_RESULT = 0x04
TEEN_CRESET = 0x05

# Packet header classes

class hdr_teen:
    offset_ = None  # Placeholder for offset

    @staticmethod
    def access(packet):
        # Simulate packet access for Python (returning packet as an object)
        return packet

class hdr_teen_req:
    def __init__(self, pkt_id, src_nodeid):
        self.pkt_type = TEEN_REQ
        self.pkt_id = pkt_id
        self.src_nodeid = src_nodeid

    def size(self):
        # Calculate the size of the structure
        return struct.calcsize("!B B Q")  # Format: unsigned char, unsigned char, unsigned long long

class hdr_teen_resp:
    def __init__(self, pkt_id, src_nodeid, desti_nodeid):
        self.pkt_type = TEEN_RESP
        self.pkt_id = pkt_id
        self.src_nodeid = src_nodeid
        self.desti_nodeid = desti_nodeid

    def size(self):
        # Calculate the size of the structure
        return struct.calcsize("!B B Q Q")  # Format: unsigned char, unsigned char, unsigned long long, unsigned long long

class hdr_teen_cont:
    def __init__(self, pkt_id, udesti_nodeid, desti_nodeid, src_nodeid):
        self.pkt_type = TEEN_CONT
        self.pkt_id = pkt_id
        self.udesti_nodeid = udesti_nodeid
        self.desti_nodeid = desti_nodeid
        self.src_nodeid = src_nodeid

    def size(self):
        # Calculate the size of the structure
        return struct.calcsize("!B B Q Q Q")  # Format: unsigned char, unsigned char, unsigned long long, unsigned long long, unsigned long long

class hdr_teen_result:
    def __init__(self, src_nodeid, pkt_id, route_info):
        self.pkt_type = TEEN_RESULT
        self.src_nodeid = src_nodeid
        self.pkt_id = pkt_id
        self.route_info = route_info

    def size(self):
        # Calculate the size of the structure
        return struct.calcsize("!B Q B B")  # Format: unsigned char, unsigned long long, unsigned char, unsigned char

class hdr_teen_creset:
    def __init__(self, pkt_id, src_nodeid):
        self.pkt_type = TEEN_CRESET
        self.pkt_id = pkt_id
        self.src_nodeid = src_nodeid

    def size(self):
        # Calculate the size of the structure
        return struct.calcsize("!B Q B")  # Format: unsigned char, unsigned long long, unsigned char

# Example usage
packet_req = hdr_teen_req(pkt_id=1, src_nodeid=12345)
packet_resp = hdr_teen_resp(pkt_id=1, src_nodeid=12345, desti_nodeid=54321)

print(f"TEEN_REQ packet size: {packet_req.size()} bytes")
print(f"TEEN_RESP packet size: {packet_resp.size()} bytes")
