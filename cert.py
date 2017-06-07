#!/usr/local/bin/python

import re

def main():
    with open('cert.txt') as f:
        lines = f.readlines()
        lines = [x.strip() for x in lines]
        counter = 0
        text_block_list = []
        begin = counter
        end = None
        for line in lines:
            if re.match('-----BEGIN CERTIFICATE-----', line):
                begin = counter
            elif re.match('-----END CERTIFICATE-----', line):
                end = counter + 1 # add one to actually count end line
            if end is not None:
                text_block_list.append({begin, end})
                end = None
            counter += 1

        for block in text_block_list:
            mylist = list(block)
            for cert_line in range(mylist[0], mylist[1]):
                print '"' + lines[cert_line] + '\\n"'
if __name__ == "__main__":
    main()
