import urllib3
http = urllib3.PoolManager()
r = http.request('GET', 'http://192.168.1.177/?V=A1')
print(r.status)
print(" ")
print(r.data)
