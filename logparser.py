#logPath  = "./FingerSaber.log"
#savePath = "./FingerSaberParsed.log"

#logPath  = "./FingerSaber_FromButtonPress.log"
#savePath = "./FingerSaber_FromButtonPress_Parsed.log"

logPath  = "./FingerSaberMulti.log"
savePath = "./FingerSaberMulti_Parsed.log"

class SceneObject:
    def __init__(self, name, depth) -> None:
        self.name = name
        self.children = []
        self.depth = depth

    def childCount(self):
        return len(self.children)

    def hasChild(self, child):
        for ownedChild in self.children:
            if child.name == ownedChild.name:
                return True
        return False

    def getObjectByName(self, name):
        for obj in self.children:
            if name == obj.name:
                return obj
        return None

    def addChildByName(self, name):
        newChild = SceneObject(name, self.depth + 1)
        self.children.append(newChild)
        return newChild

    def __str__(self) -> str:
        #_str = self.name
        idx = self.name.rfind("(")
        _str = self.name[:idx]

        for obj in self.children:
            if obj.childCount() > 0:
                _str += "\n"
                _str += "|  " * obj.depth
            else:
                _str += "\n" + "|  " * (obj.depth-1) + "|___"
            _str += str(obj)
        return _str

class SceneRoot:
    def __init__(self) -> None:
        self.objects = []

    def addObject(self, obj):
        self.objects.append(obj)

    def getObjectByName(self, name):
        for obj in self.objects:
            if name == obj.name:
                return obj
        return None

    def __str__(self) -> str:
        _str = ""
        for obj in self.objects:
            _str += "" + str(obj) + "\n\n"
        return _str

scene = SceneRoot()
with open(logPath) as file:
    line_i = 0

    for row in file:
        row = row.strip()
        if row == "": continue

        row = row.split(" -> ")
        row.reverse() # Ensures that "row[0]" is ALWAYS ROOT OBJECT


        # Checking if root object exists. Adding if does not.
        rootObj = scene.getObjectByName(row[0])
        if rootObj == None:
            rootObj = SceneObject(row[0], 0)
            scene.addObject(rootObj)

        nextObj: SceneObject = rootObj
        for i in range(1, len(row)):
            childObj = nextObj.getObjectByName(row[i])

            if childObj == None:
                nextObj = nextObj.addChildByName(row[i])
            else:
                nextObj = childObj

        #line_i += 1
        #if line_i == 100: break

f = open(savePath, "w")
f.write(str(scene))
f.close()
#print(str(scene))