import os, sys
from datetime import datetime

path = sys.argv[1]
print(path)
file = open("temp.txt", "w")

segundos = os.stat(path).st_mtime
data = datetime.fromtimestamp(segundos).strftime("%A, %B %d, %Y %I:%M:%S")

file.write(str(segundos)+"\n"+data)
file.close()
