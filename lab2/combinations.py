def combos(L, D):
    res = []
    n = len(L)
    for i in range(n-1):
        for j in range(i+1, n):
            X = L.copy()
            X[i], X[j] = X[j], X[i]
            if hash(X) not in D:
                D.add(hash(X))
                res.append(X)
    return res

def hash(L):
    return ",".join( map(str,L) )

L = [1,2,3,4]
res = [L]
j = 0
D = set([hash(L)])
while True:
    n = len(res)

    for i in range(j, n):
        x = combos(res[i], D)
        res = res + x

    # print(j, n, res)
    if(j>=n):
        break
    j = n

for L in res:
    print(L)
print(len(res))