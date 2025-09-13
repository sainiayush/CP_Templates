class UnionFind:
    def __init__(self, n, size=None):
        self.parent_or_size = [-1] * n if size is None else [-x for x in size]
 
    def find(self, a):
        a = self.parent_or_size[a] if self.parent_or_size[a] >= 0 else a
        acopy = a
        while self.parent_or_size[a] >= 0:
            a = self.parent_or_size[a]
        while acopy != a:
            self.parent_or_size[acopy], acopy = a, self.parent_or_size[acopy]
        return a
 
    def merge(self, a, b):
        pa, pb = self.find(a), self.find(b)
        if pa == pb: return False
        if self.parent_or_size[pa] > self.parent_or_size[pb]:
            pa, pb = pb, pa
        self.parent_or_size[pa] += self.parent_or_size[pb]
        self.parent_or_size[pb] = pa
        return True
 
    def getSize(self, a):
        return -self.parent_or_size[self.find(a)]
 
