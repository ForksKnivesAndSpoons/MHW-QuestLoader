import os
from zipfile import ZipFile
import shutil
import glob

os.chdir(os.path.dirname(os.path.abspath(__file__)) + '/..')

with ZipFile('x64/Loader-Release.zip', 'w') as zip:
    zip.write("x64/Release/loader.dll", "loader.dll")
    zip.write("x64/Release/injector.dll", "dtdata.dll")
    zip.write("x64/Release/loader-config.json", "loader-config.json")
    zip.write("x64/Release/QuestLoader.dll", "nativePC/plugins/QuestLoader.dll")
    zip.write("x64/Release/MonsterLoader.dll", "nativePC/plugins/MonsterLoader.dll")

try:
    with ZipFile('x64/Clutch-Release.zip', 'w') as zip:
        zip.write('x64/Release/ClutchRework.dll', 'nativePC/plugins/ClutchRework.dll')
        for path in glob.iglob(r'Plugins/ClutchRework/nativePC/**/*.col', recursive=True):
            zip.write(path, os.path.relpath(path, r'Plugins/ClutchRework/'))
except FileNotFoundError as e:
    print("ClutchRework was not built")
    print(e)
    os.remove("x64/Clutch-Release.zip")

shutil.copyfile("x64/Release/loader.lib", "Plugins/dependencies/loader.lib")
shutil.copyfile("MHWLoader/loader.h", "Plugins/dependencies/loader.h")





