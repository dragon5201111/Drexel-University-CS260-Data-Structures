import heapq

class Node:
    def __init__(self, freq : int, char):
        self.freq = freq
        self.char = char
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

    def __repr__(self):
        return f"Node(freq={self.freq},char={self.char})"


class NodeHeap:
    def __init__(self):
        self.nodes = []
    
    def insert(self, node : Node):
        self.nodes.append(node)
        heapq.heapify(self.nodes)

    def extractMin(self):
        return heapq.heappop(self.nodes)

    def __repr__(self):
        return self.nodes.__repr__()


if __name__ == "__main__":
    nh = NodeHeap()
    nh.insert(Node(5, 'c'))
    nh.insert(Node(5, 'b'))
    nh.insert(Node(2, 'e'))
    print(nh)
