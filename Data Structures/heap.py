class MaxHeap:
    def __init__(self, items=None):
        if items is None:
            self.items = []
        else:
            self.items = items
            self.max_heapify()
            
    def max_heapify(self):
        last_parent_i = (len(self.items)//2) - 1

        for i in range(last_parent_i, -1, -1):
            self.down_heap(i)

    def is_empty(self):
        return len(self.items) == 0
    
    def get_parent(self, i):
        return (i-1)//2

    def get_left_child(self, i):
        return 2 * i + 1
    
    def get_right_child(self, i):
        return 2 * i + 2

    def up_heap(self, i):
        parent_i = self.get_parent(i)

        if parent_i < 0:
            return

        if self.items[i] > self.items[parent_i]:
            self.items[i], self.items[parent_i] = self.items[parent_i], self.items[i]
            self.up_heap(parent_i)

    def down_heap(self, parent_i):
        left_i = self.get_left_child(parent_i)
        right_i = self.get_right_child(parent_i)
        biggest_i = parent_i

        len_items = len(self.items)

        if left_i < len_items and self.items[left_i] > self.items[biggest_i]:
            biggest_i = left_i
        if right_i < len_items and self.items[right_i] > self.items[biggest_i]:
            biggest_i = right_i
        if biggest_i != parent_i:
            self.items[parent_i], self.items[biggest_i] = self.items[biggest_i], self.items[parent_i]
            self.down_heap(biggest_i)     
            
        
    
    def extract_max(self):
        if self.is_empty():
            raise Exception("Cannot extract max on empty heap.")
        
        max_value = self.items[0]
        # Swap last and first element
        self.items[0], self.items[len(self.items)-1] = self.items[len(self.items)-1], self.items[0]

        self.items.pop()
        self.down_heap(0)
        return max_value
    
    def peek_max(self):
        if self.is_empty():
            raise Exception("Cannot peek empty heap.")
        return self.items[0]
    
    def insert(self, value):
        self.items.append(value)
        self.up_heap(len(self.items)-1)
    
    def __repr__(self):
        return f"MaxHeap({self.items})"


if __name__ == "__main__":
    max_heap = MaxHeap([1,2,40,1203,123,2,0,1,-1,34,5])
    print(max_heap)


