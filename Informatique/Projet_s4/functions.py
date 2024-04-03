def arduinoMap(x, in_min, in_max, out_min, out_max):
    """

    Parameters
    ----------
    x : TYPE
        DESCRIPTION.
    in_min : TYPE
        DESCRIPTION.
    in_max : TYPE
        DESCRIPTION.
    out_min : TYPE
        DESCRIPTION.
    out_max : TYPE
        DESCRIPTION.

    Returns
    -------
    TYPE
        DESCRIPTION.

    """
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def clamp(n, minn, maxn):
    return max(min(maxn, n), minn)

