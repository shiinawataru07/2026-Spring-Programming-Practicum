exit = None
class TaggedList:  #元素带标签的列表
    def __init__(self, scores, tags) :
        self.scores = scores
        self.tags = tags
    def __len__(self) :
        return len(self.scores)
    def __contains__(self, score) :
        return score in self.scores
    def __str__(self) :
        s = ''
        for i in range(len(self.scores)) :
            s += self.tags[i] + ':' + str(self.scores[i]) + ','
        return s
    def __getitem__(self, key) :
        if type(key) == int :
            return self.scores[key]
        else :
            return self.scores[self.tags.index(key)]
    def __setitem__(self, key, value) :
        if type(key) == int :
            self.scores[key] = value
        else :
            self.scores[self.tags.index(key)] = value
a = TaggedList([70,80,90,100],["语文","数学","英语","物理"])
print(len(a),78 in a, 80 in a) #>>4 False True
print(str(a)) #>>语文:70,数学:80,英语:90,物理:100,
print(a[0],a['数学']) #>>70 80   标签也可以作为下标访问元素
a[1] = a['物理'] = 85
print(a) #>>语文:70,数学:85,英语:90,物理:85,