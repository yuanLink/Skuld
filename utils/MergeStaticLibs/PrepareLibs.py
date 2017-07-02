import os
import sys
import shutil

def prepare_libs(ar, _objdir, lib, output, libname):
    print("-- Merge " + libname)
    objdir = os.path.abspath(_objdir)
    if (os.path.exists(objdir)):
        shutil.rmtree(objdir)
    os.mkdir(objdir)
    os.chdir(objdir)
    os.system(ar + " -x " + lib)
    files = os.listdir(objdir)
    for name in files:
        os.rename(os.path.join(objdir, name), os.path.join(objdir, libname + "_" + name))
    with open(output, "w") as f:
        for name in files:
            f.write(libname + "_" + name + "\n")
    os.chdir("..")

if __name__ == "__main__":
    ar = sys.argv[1]
    dest = sys.argv[2]
    for i in range(3, len(sys.argv)):
        prepare_libs(ar, sys.argv[i] + ".objdir", sys.argv[i], sys.argv[i] + ".objlist", os.path.basename(sys.argv[i]).split(".")[0])
        os.chdir(sys.argv[i] + ".objdir")
        os.system(ar + " r " + dest + " @" + sys.argv[i] + ".objlist")
        os.chdir("..")
        shutil.rmtree(sys.argv[i] + ".objdir")