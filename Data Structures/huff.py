import heapq


class Node:
    """Represents a node in the Huffman Tree."""
    def __init__(self, freq=0, char=None):
        self.freq = freq 
        self.char = char
        self.left = None
        self.right = None

    def __lt__(self, other):
        """Comparator for priority queue (heap), based on frequency."""
        return self.freq < other.freq

    def __repr__(self):
        """String representation for debugging."""
        return f"Node(freq={self.freq}, char={repr(self.char)}, left={repr(self.left)}, right={repr(self.right)})"


class NodeHeap:
    """A Min-Heap data structure for managing nodes."""
    def __init__(self):
        self.nodes = []

    def insert(self, node: Node):
        """Inserts a node into the heap."""
        heapq.heappush(self.nodes, node)

    def extract_min(self):
        """Extracts the node with the smallest frequency."""
        return heapq.heappop(self.nodes)

    def clear(self):
        """Clears all nodes from the heap."""
        self.nodes.clear()

    def __repr__(self):
        """String representation of the heap."""
        return repr(self.nodes)

    def __len__(self):
        """Returns the number of nodes in the heap."""
        return len(self.nodes)


class HuffmanTree:
    """Builds and manipulates a Huffman Tree for encoding and decoding text."""
    def __init__(self, text: str = "", node_heap: NodeHeap = None):
        self.codes = {}
        self.node_heap = node_heap or NodeHeap()
        self.text = text

    def build_heap(self):
        """Builds the heap from character frequencies in the text."""
        if not self.text:
            return
        
        self.node_heap.clear()
        freq_map = {char: self.text.count(char) for char in set(self.text)}

        for char, freq in freq_map.items():
            self.node_heap.insert(Node(freq, char))

    def build_tree(self):
        """Builds the Huffman tree by repeatedly combining the two least frequent nodes."""
        self.build_heap()

        while len(self.node_heap) > 1:
            left = self.node_heap.extract_min()
            right = self.node_heap.extract_min()

            parent = Node(left.freq + right.freq, None)
            parent.left = left
            parent.right = right

            self.node_heap.insert(parent)

        return self.node_heap.extract_min()

    def build_encoding_dict(self, node: Node, prefix=""):
        """Recursively builds the encoding dictionary."""
        if node is None:
            return
        
        if node.char is not None:
            self.codes[node.char] = prefix

        self.build_encoding_dict(node.left, prefix + '0')
        self.build_encoding_dict(node.right, prefix + '1')

    def generate_codes(self):
        """Generates the Huffman codes based on the tree."""
        root = self.build_tree()
        self.build_encoding_dict(root)

    def encode(self, text: str) -> str:
        """Encodes the input text into a Huffman encoded string."""
        return ''.join(self.codes[char] for char in text)

    def decode(self, root: Node, encoded_string: str) -> str:
        """Decodes a Huffman encoded string back into the original text."""
        decoded_string = []
        current_node = root
        
        for bit in encoded_string:
            current_node = current_node.left if bit == '0' else current_node.right

            if current_node.char is not None:
                decoded_string.append(current_node.char)
                current_node = root

        return ''.join(decoded_string)


def write_huffman_to_file(huffman_tree: HuffmanTree, text: str, out_file="compressed.bin"):
    """Writes the Huffman encoded text to a binary file."""
    encoded_string = huffman_tree.encode(text)
    padding = 8 - len(encoded_string) % 8
    encoded_string = '0' * padding + encoded_string

    with open(out_file, 'wb') as file:
        for i in range(0, len(encoded_string), 8):
            byte = encoded_string[i:i + 8]
            file.write(bytes([int(byte, 2)]))


if __name__ == "__main__":
    text = "this is an example"
    huffman_tree = HuffmanTree(text=text)
    huffman_tree.generate_codes()

    root = huffman_tree.build_tree()
    encoded_string = huffman_tree.encode(text)

    print(f"Encoded: {encoded_string}")
    decoded_string = huffman_tree.decode(root, encoded_string)
    print(f"Decoded: {decoded_string}")
