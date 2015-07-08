import urllib3
http = urllib3.PoolManager()
r = http.request('GET', 'http://192.168.1.3/?V=11')
print(r.status)
print(" ")
print(r.data)
