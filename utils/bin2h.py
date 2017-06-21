import sys
import os

def bin2h(input, output, variable):
    des_dir = os.path.dirname(os.path.abspath(output))
    if (not os.path.exists(des_dir)):
        os.makedirs(des_dir)
    with open(input, "rb") as f1:
        content = f1.read()
    with open(output, "w") as f2:
        f2.write("static const unsigned char " + variable + "[] = {")
        for value in content:
            f2.write(hex(ord(value)))
            f2.write(",")
        f2.write("};")

if __name__ == "__main__":
    if (len(sys.argv) < 4):
        print("usage: python bin2h.py input output variable")
        sys.exit()
    bin2h(sys.argv[1], sys.argv[2], sys.argv[3])