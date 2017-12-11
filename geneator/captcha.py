from claptcha import Claptcha

font = "/Library/Fonts/AppleGothic.ttf"
path = "./images/"

for j in range(0, 10):
    for i in range(0, 100):
        c = Claptcha(str(j), font);
        c.size = (100, 100)
        c.margin = (5,5);
        text, image = c.image
        text, bytes = c.bytes
        fileName = str(j) + "_"
        if i < 10:
            fileName = fileName + "00" + str(i)
        else:
            fileName = fileName + "0" + str(i)
        fileName = fileName + ".bmp"

        print(fileName)
        c.write(path + fileName)
