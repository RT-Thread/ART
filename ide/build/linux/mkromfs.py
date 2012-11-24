#!/usr/bin/python

import sys
import os
import string
import struct

basename = ''
baseaddr = 0x0
output = ''

LITTLE_ENDIAN = 0x00
BIG_ENDIAN    = 0x01
endian = LITTLE_ENDIAN

def mkromfs_output(out):
    # print '%s' % out,
    output.write(out)

def mkromfs_file(filename, arrayname):
    f = file(filename, "rb")
    arrayname = arrayname.replace('.', '_')
    arrayname = arrayname.replace('-', '_')
    mkromfs_output('const static unsigned char %s[] = {\n' % arrayname)

    count = 0
    while True:
        byte = f.read(1)

        if len(byte) != 1:
            break

        mkromfs_output('0x%02x,' % ord(byte))

        count = count + 1
        if count == 16:
            count = 0
            mkromfs_output('\n')

    if count == 0:
        mkromfs_output('};\n\n')
    else:
        mkromfs_output('\n};\n\n')

    f.close()

def mkromfs_dir(dirname, is_root = False):
    list = os.listdir(dirname)
    path = os.path.abspath(dirname)

    # make for directory
    for item in list:
        fullpath = os.path.join(path, item)
        if os.path.isdir(fullpath):
            # if it is an empty directory, ignore it
            l = os.listdir(fullpath)
            if len(l):
                mkromfs_dir(fullpath)

    # make for files
    for item in list:
        fullpath = os.path.join(path, item)
        if os.path.isfile(fullpath):
            subpath = fullpath[len(basename):]
            array = subpath.split('\\')
            arrayname = string.join(array, '_')
            mkromfs_file(fullpath, arrayname)

    subpath = path[len(basename):]
    dir = subpath.split('\\')
    direntname = string.join(dir, '_')
    if is_root:
        mkromfs_output('const struct romfs_dirent _root_dirent[] = {\n')
    else:
        mkromfs_output(('const static struct romfs_dirent %s[] = {\n' % direntname))

    for item in list:
        fullpath = os.path.join(path, item)
        fn = fullpath[len(dirname):]
        if fn[0] == '\\':
            fn = fn[1:]
        fn = fn.replace('\\', '/')

        subpath = fullpath[len(basename):]
        items = subpath.split('\\')
        item_name = string.join(items, '_')
        item_name = item_name.replace('.', '_')
        item_name = item_name.replace('-', '_')
        subpath = subpath.replace('\\', '/')
        if subpath[0] == '/':
            subpath = subpath[1:]

        if not os.path.isfile(fullpath):
            l = os.listdir(fullpath)
            if len(l):
                mkromfs_output(('\t{ROMFS_DIRENT_DIR, "%s", (rt_uint8_t*) %s, sizeof(%s)/sizeof(%s[0])},\n' % (fn, item_name, item_name, item_name)))
            else:
                mkromfs_output(('\t{ROMFS_DIRENT_DIR, "%s", RT_NULL, 0},\n' % fn))

    for item in list:
        fullpath = os.path.join(path, item)
        fn = fullpath[len(dirname):]
        if fn[0] == '\\':
            fn = fn[1:]
        fn = fn.replace('\\', '/')

        subpath = fullpath[len(basename):]
        items = subpath.split('\\')
        item_name = string.join(items, '_')
        item_name = item_name.replace('.', '_')
        item_name = item_name.replace('-', '_')
        subpath = subpath.replace('\\', '/')
        if subpath[0] == '/':
            subpath = subpath[1:]

        if os.path.isfile(fullpath):
            mkromfs_output(('\t{ROMFS_DIRENT_FILE, "%s", %s, sizeof(%s)},\n' % (fn, item_name, item_name)))

    mkromfs_output('};\n\n')

#############################
# make romfs as a binary file
#############################
ROMFS_DIRENT_FILE = 0
ROMFS_DIRENT_DIR = 1

class romfs_dirent():
    def __init__(self):
        self.type = ROMFS_DIRENT_FILE
        self.name = 0x0
        self.data = 0x0
        self.size = 0

    def pack(self):
        name = baseaddr + self.name
        data = baseaddr + self.data

        # print 'name = %d' % name
        # print 'data = %d' % data

        return struct.pack('LLLL', self.type, name,
            data, self.size)

def mkromfs_write_string(str):
    str = str + '\0'
    padding = (len(str) + 3) & ~0x03 # align to 4 bytes
    padding = padding - len(str)

    output.write(str)
    if padding:
        for i in range(padding):
            output.write('\0')

def mkromfs_binary_dir(dirname, is_root = False):
    # get files and sub-directory in this directory
    list = os.listdir(dirname)
    path = os.path.abspath(dirname)

    # remove '.' and '..' item
    if '.' in list:
        list.remove('.')
    if '..' in list:
        list.remove('..')

    # get number of list
    number = len(list)
    dirent_length = number * 16

    # generate root direntry
    if is_root:
        dirent_root = romfs_dirent()
        dirent_root.name = 16
        dirent_root.data = 20
        dirent_root.type = ROMFS_DIRENT_DIR
        dirent_root.size = number

        output.write(dirent_root.pack())
        mkromfs_write_string('/')

    # save current direntry offset, this direntry will be updated later
    dirent_offset = output.tell()

    # create direntry list and write empty direntry
    dirent_list = []
    for index in range(number):
        item = romfs_dirent()
        dirent_list.append(item)
        output.write(item.pack())

    # write name
    for index in range(number):
        dirent_list[index].name = output.tell()

        # write name of files or sub-directory
        name = list[index]
        mkromfs_write_string(name)

    index = 0
    for item in list:
        fullpath = os.path.join(path, item)
        dirent_list[index].data = output.tell()

        if os.path.isfile(fullpath):
            # read file data
            input = file(fullpath, "rb")
            data = input.read()
            input.close()

            dirent_list[index].type = ROMFS_DIRENT_FILE
            dirent_list[index].size = len(data)
            # align to 4 bytes
            padding = (dirent_list[index].size + 3) & ~0x03
            padding = padding - dirent_list[index].size

            # write data
            output.write(data)
            # write padding
            if padding:
                for i in range(padding):
                    output.write('\0')
        else:
            # it's a directory
            dirent_list[index].type = ROMFS_DIRENT_DIR
            dirent_list[index].size = mkromfs_binary_dir(fullpath)

        index = index + 1

    # write dirent entries
    current = output.tell()
    output.seek(dirent_offset, os.SEEK_SET)
    for index in range(number):
        output.write(dirent_list[index].pack())
    output.seek(current, os.SEEK_SET)

    return number

if __name__ == "__main__":
    binary_mode = False
    try:
        if sys.argv[1] == '--binary':
            binary_mode = True
            if sys.argv[2] == '--addr':
                baseaddr = int(sys.argv[3], 16)
                basename = os.path.abspath(sys.argv[4])
            else:
                base_addr = 0x00
                basename = os.path.abspath(sys.argv[2])
            filename = basename + '.bin'
        else:
            basename = os.path.abspath(sys.argv[1])
            filename = os.path.abspath(sys.argv[2])
    except IndexError:
        print "Usage: mkromfs <dirname> <filename>"
        print "       mkromfs --binary --addr <address> <dirname>"
        raise SystemExit

    if binary_mode:
        output = file(filename, 'wb')
        mkromfs_binary_dir(basename, is_root=True)
        output.close()
    else:
        output = file(filename, 'wt')
        mkromfs_output("#include <dfs_romfs.h>\n\n")
        mkromfs_dir(basename, is_root = True)

        mkromfs_output("const struct romfs_dirent romfs_root = {ROMFS_DIRENT_DIR, \"/\", (rt_uint8_t*) _root_dirent, sizeof(_root_dirent)/sizeof(_root_dirent[0])};\n\n")
        output.close()
