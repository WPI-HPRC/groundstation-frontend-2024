import os

root = "/Users/will/Documents/GitHub/HPRC/Ground Station/main/QT-GroundStation2024/Resources/Maps"
write_file = "/Users/will/Documents/GitHub/HPRC/Ground Station/main/QT-GroundStation2024/map.qrc"
prefix="/"

file = open(write_file, "w")

file.write("<RCC>\n")
file.write(f"\t<qresource prefix=\"{prefix}\">\n")

for path, subdirs, files in os.walk(root):
    for name in files:
        file.write(f"\t\t<file>{os.path.join(path, name).split('QT-GroundStation2024/')[1]}</file>\n")

file.write("\t</qresource>\n")
file.write("</RCC>\n")