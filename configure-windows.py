import subprocess

print "Executing CMake ..."

subprocess.call("cmake .. -G \"Visual Studio 12\"", shell=True)  

print "Setting libbitcoin platform toolset to 'CTP_Nov2013' ..."

f = open('dependencies/libbitcoin/libbitcoin.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('dependencies/libbitcoin/libbitcoin.vcxproj','w')
f.write(newdata)
f.close()

print "Setting libbitcoin-blockchain platform toolset to 'CTP_Nov2013' ..."

f = open('dependencies/libbitcoin-blockchain/libbitcoin-blockchain.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('dependencies/libbitcoin-blockchain/libbitcoin-blockchain.vcxproj','w')
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

print "Setting decentralised_core platform toolset to 'CTP_Nov2013' ..."

f = open('src/decentralised_core/decentralised_core.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('src/decentralised_core/decentralised_core.vcxproj','w')
f.write(newdata)
f.close()

print "Setting secp256k1 platform toolset to 'CTP_Nov2013' ..."

f = open('dependencies/secp256k1/secp256k1.vcxproj','r')
filedata = f.read()
f.close()

newdata = filedata.replace("<PlatformToolset>v120</PlatformToolset>","<PlatformToolset>CTP_Nov2013</PlatformToolset>")

f = open('dependencies/secp256k1/secp256k1.vcxproj','w')
f.write(newdata)
f.close()