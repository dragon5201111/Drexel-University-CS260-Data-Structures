class UnionFind:
    def __init__(self, v):
        self.nodes = [v for v in range(v+1)]
        self.size = [1] * (v+1)
    
    def find(self, v):
        while self.nodes[v] != v:
            v = self.nodes[v]
        return v
    
    def union(self, v, w):
        rootV = self.find(v)
        rootW = self.find(w)
        
        if rootV != rootW:
            if self.size[rootV] >= self.size[rootW]:
                self.nodes[rootW] = rootV
                self.size[rootV] += self.size[rootW]
            else:
                self.nodes[rootV] = rootW
                self.size[rootW] += self.size[rootV]