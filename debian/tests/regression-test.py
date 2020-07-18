#!/usr/bin/python3
from subprocess import PIPE, Popen
from PIL import Image
from PIL import ImageChops


def spicy_screenshot():
    cmd = "spicy-screenshot -h localhost -p 5912 -o output.ppm"
    p = Popen(cmd, shell=True)
    p.wait()

def verify():
    base = Image.open("base_test.ppm")
    output = Image.open("output.ppm")
    return ImageChops.difference(base, output).getbbox()

if __name__ == "__main__":
    spicy_screenshot()
    diff = verify()

    if diff is None:
        print("\033[1;32mSUCCESS: No regressions were found!\033[1;m")
    else:
        print("\033[1;31mFAIL: Regressions were found!\n\033[1;m"
              "\033[1;31m      Please, take a look in your code and go fix it!\033[1;m")
