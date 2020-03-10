def h1(node, dims):
    ''' Underestimating Heuristic '''
    indices = node.indices
    if len(indices) <= 0:
        return 0
    val = dims[indices[0]]
    for i in indices:
        val = val * dims[i+1]
    return val/len(indices)


def h2(node, dims):
    ''' Overestimating Heuristic '''
    indices = node.indices
    if len(indices) <= 0:
        return 0
    val = dims[indices[0]]
    for i in indices:
        val = val * dims[i+1]
    return val * len(indices)