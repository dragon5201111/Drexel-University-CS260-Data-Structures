def createMatrix(k):
    matrix = [[float('inf')] * k for _ in range(k)]
    for i in range(k):
        matrix[i][i] = 0
    return matrix


def createMatrixString(matrix):
    strRep = ""
    for i in range(len(matrix)):
        strRep += f"{i}: {matrix[i]}\n"
    return strRep


class AdjacencyMatrix:
    def __init__(self, k, weighted=False, directed=False):
        self.k = k
        self.matrix = createMatrix(k)
        self.weighted = weighted
        self.directed = directed

    def addEdge(self, i, j, weight=1):
        if not self.directed:
            self.matrix[j][i] = weight
        self.matrix[i][j] = weight

    def computeASP(self):
        memoTable = [row[:] for row in self.matrix]

        for k in range(self.k):
            for i in range(self.k):
                for j in range(self.k):
                    if memoTable[i][j] > memoTable[i][k] + memoTable[k][j]:
                        memoTable[i][j] = memoTable[i][k] + memoTable[k][j]

        return memoTable

    def printASP(self):
        matrix = self.computeASP()
        print(createMatrixString(matrix))

    def __str__(self):
        return createMatrixString(self.matrix)


if __name__ == "__main__":
    am = AdjacencyMatrix(2)
    am.printASP()
