# Copyright (c) 2011 Chris Lentini
# http://divergentcoder.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of 
# this software and associated documentation files (the "Software"), to deal in 
# the Software without restriction, including without limitation the rights to use, 
# copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
# Software, and to permit persons to whom the Software is furnished to do so, 
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all 
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from subprocess import Popen, PIPE
from sys import stdout
import struct
from optparse import OptionParser
from datetime import datetime

################################################################################

cmdline = OptionParser()
cmdline.add_option("--output", help="Specifies bitmap destination file",
                   action="store", dest="filename", default="output.bmp")
cmdline.add_option("--width", help="Specifies output image width",
                   action="store", dest="width", default=64)
cmdline.add_option("--height", help="Specifies output image height",
                   action="store", dest="height", default=64)
cmdline.add_option("--tilew", help="Specifies the tiling width",
                   action="store", dest="tilew", default=4)

(options, args) = cmdline.parse_args()

width = int(options.width)
height = int(options.height)
tilew = int(options.tilew)

################################################################################

def write_header(file):
    global width, height
    l0 = struct.pack('<L', 0)
    h0 = struct.pack('<H', 0)
    
    file.write(struct.pack('<B',66))
    file.write(struct.pack('<B',77))
    file.write(l0)
    file.write(h0)
    file.write(h0)
    file.write(struct.pack('<L',54))
    file.write(struct.pack('<L',40))
    file.write(struct.pack('<L',width))
    file.write(struct.pack('<L',height))
    file.write(struct.pack('<H',1))
    file.write(struct.pack('<H',24))
    file.write(l0)
    file.write(l0)
    file.write(l0)
    file.write(l0)
    file.write(l0)
    file.write(l0)

output = open(options.filename, "wb")
write_header(output)

################################################################################
count = 0
total = (width / tilew) * height
start = datetime.now()
for y in range(0,height,1):
    for x in range(0,width,tilew*2):
        inst0 = Popen(["gcc","-o","tmp0.exe","-DOUTW=%d" % width,
                       "-DOUTH=%d" % height, "-DUX=%d" % x,
                       "-DUY=%d" % (height-y-1), "-DTILEW=%d" % tilew,
                       "main.cpp"])
        
        inst1 = Popen(["gcc","-o","tmp1.exe","-DOUTW=%d" % width,
                       "-DOUTH=%d" % height, "-DUX=%d" % (x+tilew),
                       "-DUY=%d" % (height-y-1), "-DTILEW=%d" % tilew,
                       "main.cpp"])
        
        inst0.wait()
        inst1.wait()
        
        inst0 = Popen(["tmp0.exe"], stdout=PIPE)
        out0 = inst0.communicate()[0]
        output.write(out0)
        
        inst1 = Popen(["tmp1.exe"], stdout=PIPE)
        out1 = inst1.communicate()[0]
        output.write(out1)
        
        count += 2
        stdout.write("\r%d/%d" % (count,total))
        stdout.flush()
end = datetime.now()
delta = end - start
numseconds = delta.seconds + (delta.days * 24 * 3600) + (delta.microseconds / 10**6)
print("\nCompleted %d seconds" % numseconds)
################################################################################

output.close()