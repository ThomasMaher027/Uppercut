def arduinoMap(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def clamp(n, minn, maxn):
    return max(min(maxn, n), minn)

