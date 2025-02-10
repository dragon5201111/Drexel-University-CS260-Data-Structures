import heapq

class Node:
    def __init__(self, freq = 0, char = ''):
        self.freq = freq
        self.char = char
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq <= other.freq

    def __repr__(self):
        left_repr = f"left={repr(self.left)}" if self.left else "left=None"
        right_repr = f"right={repr(self.right)}" if self.right else "right=None"
        return f"Node(freq={self.freq}, char={self.char}, {left_repr}, {right_repr})"



class NodeHeap:
    def __init__(self):
        self.nodes = []
    
    def insert(self, node : Node):
        heapq.heappush(self.nodes, node)

    def extractMin(self):
        return heapq.heappop(self.nodes)
    
    def clearNodes(self):
        self.nodes.clear()

    def __repr__(self):
        return self.nodes.__repr__()

    def __len__(self):
        return len(self.nodes)
    

class HuffmanTree:
    def __init__(self, characters = ""):
        self.characters = characters

    def buildHeap(self, nodeHeap : NodeHeap):
        wordFreq = {}

        for i in self.characters:
            wordFreq[i] = wordFreq.setdefault(i, 0) + 1
        
        for key, value in wordFreq.items():
            nodeHeap.insert(Node(value, key))
        
    
    def buildEncodingTree(self, nodeHeap: NodeHeap):
        while len(nodeHeap) > 1:
            minNode1 = nodeHeap.extractMin()
            minNode2 = nodeHeap.extractMin()

            newRoot = Node(minNode1.freq + minNode2.freq)
            newRoot.left = minNode1
            newRoot.right = minNode2

            nodeHeap.insert(newRoot)
        
        return nodeHeap.extractMin()
    
    def buildEncodingDictionary(self, node : Node):
        pass
            

if __name__ == "__main__":
    txt = "Foo!"
    nh = NodeHeap()
    hf = HuffmanTree(txt)
    hf.buildHeap(nh)
    root = hf.buildEncodingTree(nh)
    print(root)
    


