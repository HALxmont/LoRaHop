import os

data = os.listdir() #read directory
data.sort()
list_files = []
for i in range(len(data)):
    if data[i][0] == 'G': #just gps files
        list_files.append(data[i])

print("Ingresa nombre del archivo total (GPS)")
input_name = input()    
#create total file
total_file = open(input_name+".txt",'a+')
for i in list_files:
    file_to_append = open(i,'r') #open file for read lines
    lines_to_append = file_to_append.readlines() #read lines
    for j in range(len(lines_to_append)):
        total_file.write(lines_to_append[j]) #write lines
    file_to_append.close()
total_file.close() #save total file
