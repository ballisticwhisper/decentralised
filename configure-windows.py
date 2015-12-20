import subprocess

print "Executing CMake ..."

subprocess.call("cmake .. -G \"Visual Studio 12\"", shell=True)  

print "Setting leveldb platform toolset to 'CTP_Nov2013' ..."

f = open('dependencies/leveldb/leveldb.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('dependencies/leveldb/leveldb.vcxproj','w')
f.write(newdata)
f.close()


print "Setting decentralised platform toolset to 'CTP_Nov2013' ..."

f = open('src/decentralised/decentralised.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('src/decentralised/decentralised.vcxproj','w')
f.write(newdata)
f.close()

print "Setting decentralised-core platform toolset to 'CTP_Nov2013' ..."

f = open('src/decentralised-core/decentralised-core.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('src/decentralised-core/decentralised-core.vcxproj','w')
f.write(newdata)
f.close()

print "Setting decentralised-console platform toolset to 'CTP_Nov2013' ..."

f = open('src/decentralised-console/decentralised-console.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('src/decentralised-console/decentralised-console.vcxproj','w')
f.write(newdata)
f.close()
