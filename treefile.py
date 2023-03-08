import os

def list_files(startpath):
    for root, dirs, files in os.walk(startpath):
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * (level)
        print('{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            print('{}{}'.format(subindent, f))

def list_files_better(startpath):
    for root, dirs, files in os.walk(startpath):
        for name in files:
            print(os.path.join(root,name).replace("onboard code", "${CMAKE_CURRENT_LIST_DIR}"))


list_files_better("onboard code/src/")
