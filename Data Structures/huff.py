import heapq

class Node:
    def __init__(self, freq = 0, char = None):
        self.freq = freq
        self.char = char
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

    def __repr__(self):
        return f"Node(freq={self.freq}, char={self.char}, left={repr(self.left)}, right={repr(self.right)})"



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
    def __init__(self, nodeHeap = NodeHeap(), characters = None):
        self.codes = {}
        self.nodeHeap = nodeHeap
        self.characters = characters

    def buildHeap(self):
        if self.characters is None:
            return
            
        self.nodeHeap.clearNodes()

        wordFreq = {c : self.characters.count(c) for c in self.characters}

        for key, value in wordFreq.items():
            self.nodeHeap.insert(Node(value, key))
        
    
    def buildEncodingTree(self):
        self.buildHeap()

        while len(self.nodeHeap) > 1:
            minNodeLeft = self.nodeHeap.extractMin()
            minNodeRight = self.nodeHeap.extractMin()

            newRoot = Node(minNodeLeft.freq + minNodeRight.freq, char=None)
            newRoot.right = minNodeLeft
            newRoot.left = minNodeRight


            self.nodeHeap.insert(newRoot)
        
        return self.nodeHeap.extractMin()

    def buildEncodingDictionary(self, root : Node, bitString=""):
        if root is None:
            return
        
        if root.char is not None:
            self.codes[root.char] = bitString
        
        self.buildEncodingDictionary(root.left, bitString + '0')
        self.buildEncodingDictionary(root.right, bitString + '1')

    
    def setCodes(self):
        root = self.buildEncodingTree()
        self.buildEncodingDictionary(root)
    
    def getEncodedString(self, text):
        return "".join([self.codes[char] for char in text])
    
    def decodeString(self, root, encodedString):
        decodedString = ""
        currentNode = root
        
        for bit in encodedString:
            if bit == '0':
                currentNode = currentNode.left
            else:
                currentNode = currentNode.right

            if currentNode.char is not None:
                decodedString += currentNode.char
                currentNode = root

        return decodedString

def writeHuffmanToFile(huffmanTree : HuffmanTree, text="", outFile="compressed.bin"):
    encodedString = huffmanTree.getEncodedString(text)

    padding = 8 - len(encodedString) % 8
    encodedString = '0' * padding + encodedString

    with open(outFile, 'wb') as f:
        for i in range(0, len(encodedString), 8):
            byte = encodedString[i:i+8]
            f.write(bytes([int(byte, 2)]))

if __name__ == "__main__":
    txt = "this is an example of a huffman tree foo bar"
    hf = HuffmanTree(characters=txt)
    root = hf.buildEncodingTree()
    hf.buildEncodingDictionary(root)
    encodedString = hf.getEncodedString(txt)
    print(hf.decodeString(root, encodedString))
    

    


