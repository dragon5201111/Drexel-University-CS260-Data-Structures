import random

NODES=10
RAND_MIN=0
RAND_MAX=50

class Node:
    def __init__(self, key : int):
        self.key = key
        self.right = None
        self.left = None

def insert(root: Node, key: int) -> Node:
    if root is None:
        return Node(key)
    if key < root.key:
        root.left = insert(root.left, key)
    else:
        root.right = insert(root.right, key) 
    return root

def max(root : Node) -> Node:
    node = root
    while node.right:
        node = node.right
    return node

def min(root : Node) -> Node:
    node = root
    while node.left:
        node = node.left
    return node

def predecessor(root : Node, key : int) -> Node:
    if root is None:
        return None
    
    node = search(root, key)

    if node is None:
        return None

    if node.left:
        return max(node.left)

    ancestor = root
    predecessor = None

    while ancestor is not node:
        if node.key < ancestor.key:
            ancestor = ancestor.left
        else:
            predecessor = ancestor
            ancestor = ancestor.right

    return predecessor

def successor(root : Node, key : int) -> Node:
    if root is None:
        return None
    
    node = search(root, key)

    if node is None:
        return None
    
    if node.right:
        return min(node.right)

    ancestor = root
    successor = None

    while ancestor is not node:
        if node.key > ancestor.key:
            ancestor = ancestor.right
        else:
            successor = ancestor
            ancestor = ancestor.left
    
    return successor
    
def delete(root : Node, key : int) -> None:
    if root is None:
        return root
    
    if key < root.key:
        root.left = delete(root.left, key)
    elif key > root.key:
        root.right = delete(root.right, key)
    else:
        if root.left is None:
            return root.right

        if root.right is None:
            return root.left
        
        pred = predecessor(root, key)
        root.key = pred.key
        root.left = delete(root.left, pred.key)
    return root

def search(root : Node, key : int) -> Node:
    if root is None or root.key == key:
        return root
    if key > root.key:
        return search(root.right, key)
    else:
        return search(root.left, key)

def print_node(root: Node, level = 0, prefix="Root: ") -> None:
    if root is None:
        return
    print(" " * (level * 4) + prefix + str(root.key))
    if root.left:
        print_node(root.left, level + 1, "L---> ")
    if root.right:
        print_node(root.right, level + 1, "R---> ")

if __name__ == "__main__":

    """
    # Populate BST randomly and delete one node 

    root = Node(random.randint(RAND_MIN, RAND_MAX))
    del_index = random.randint(0, NODES - 1)
    del_key = None

    for i in range(0, NODES):
        rand_key = random.randint(RAND_MIN, RAND_MAX)

        if i == del_index:
            del_key = rand_key

        root = insert(root, rand_key)

    print(f"Tree before deletion, key {rand_key}:")
    print_node(root)

    print()

    print(f"Tree after deletion, key {rand_key}:")
    delete(root, rand_key)
    print_node(root)
    """

    root = Node(13)
    insert(root, 15)
    insert(root, 14)
    insert(root, 16)
    print_node(root)
    delete(root, 15)
    print_node(root)

