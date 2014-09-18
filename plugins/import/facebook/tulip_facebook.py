from tulip import *
import facebook
import tempfile
import sys
if sys.version_info[0] == 3:
  from urllib.request import urlopen
  import queue as Queue
else:
  from urllib2 import urlopen
  import Queue
import os
import threading

maxThreadsDl = 8
nbThreadsDl = 0
threadJobs = Queue.Queue()
threadsPool = []
runThread = False

def downloadAvatar():
  global runThread
  while runThread:
    job = None
    try:
      job = threadJobs.get(block = 0)
    except Queue.Empty:
      pass

    if not job:
      continue

    url = job[0]
    fileName = job[1]
    if sys.version_info[0] < 3:
      fileNameEnc = fileName.decode(sys.getdefaultencoding()).encode(sys.getfilesystemencoding())
    else:
      fileNameEnc = fileName.encode(sys.getfilesystemencoding())

    avatarFile = urlopen(url)
    output = open(fileNameEnc,'wb')
    output.write(avatarFile.read())
    output.close()
    threadJobs.task_done()

def launchAvatarDlThread(url, directory, name):
    global threadsPool
    global threadJobs
    global nbThreadsDl
    global maxThreadsDl
    global runThread
    runThread = True
    ohPos = url.find("?oh")
    if ohPos != -1:
      urlImg = url[:ohPos]
      ext = urlImg[len(urlImg)-4:]
    else:
      ext = url[len(url)-4:]
    fileName = directory+"/"+name+ext
    threadJobs.put((url, fileName))
    if (nbThreadsDl < maxThreadsDl):
      nbThreadsDl = nbThreadsDl + 1
      t = threading.Thread(target=downloadAvatar)
      threadsPool.append(t)
      t.start()
    return fileName

def waitAvatarDlThreads():
    global threadsPool
    global threadJobs
    global runThread
    threadJobs.join()
    runThread = False
    for t in threadsPool:
        t.join()
    threadsPool = []

def getTempDir():
    return tempfile.gettempdir()

def importFacebookGraph(graph, accessToken, pluginProgress, avatarsDlPath):
  # Creation of needed properties, don't remove those lines !!!
  viewLabel = graph.getStringProperty("viewLabel")
  viewTexture = graph.getStringProperty("viewTexture")
  viewColor = graph.getColorProperty("viewColor")
  viewBorderColor = graph.getColorProperty("viewBorderColor")
  viewShape = graph.getIntegerProperty("viewShape")
  viewBorderWidth = graph.getDoubleProperty("viewBorderWidth")
  viewLayout = graph.getLayoutProperty("viewLayout")
  viewSize = graph.getSizeProperty("viewSize")
  viewMetric = graph.getDoubleProperty("viewMetric")
  nameProp = graph.getStringProperty("name")
  
  if pluginProgress:
    pluginProgress.setComment("Connecting to Facebook Graph API")
  fbGraph = facebook.GraphAPI(accessToken)
  
  if pluginProgress:
    pluginProgress.setComment("Retrieving your profile")
  profile = fbGraph.get_object("me")
  
  name = str(profile["name"])
  graph.setName("Facebook social network of " + name)
  meNode = graph.addNode()
  viewLabel[meNode] = name
  nameProp[meNode] = name

  if len(avatarsDlPath) > 0:
    picture = fbGraph.get_object("me/picture")
    fileName = launchAvatarDlThread(picture["url"], avatarsDlPath, name)
    viewTexture[meNode] = fileName
  
  if pluginProgress:
    pluginProgress.setComment("Retrieving your friends")
  friends = fbGraph.get_connections("me", "friends")
  
  friendsMap = {}
  
  for friend in friends["data"]:
    friendNode = graph.addNode()
    name = str(friend["name"])
    friendsMap[name] = friendNode
    viewLabel[friendNode] = name
    viewLabel[friendNode] = name
    graph.addEdge(meNode, friendNode)
  
  nbFriends = len(friends["data"])
  i = 0

  if pluginProgress:
    pluginProgress.progress(i, nbFriends)

  for friend in friends["data"]:
    name = str(friend["name"])
    friendNode = friendsMap[name]
    
    if len(avatarsDlPath) > 0:
      picture = fbGraph.get_object(friend["id"] + "/picture")
      fileName = launchAvatarDlThread(picture["url"], avatarsDlPath, name)
      viewTexture[friendNode] = fileName
    
    if pluginProgress:
      pluginProgress.setComment("Checking your mutual friends with " + name)
    mutualFriends = fbGraph.get_object("me/mutualfriends/"+friend["id"])
    
    for mfriend in mutualFriends["data"]:
      mfriendNode = friendsMap[str(mfriend["name"])]
      if not graph.existEdge(friendNode, mfriendNode, False).isValid():
        graph.addEdge(friendNode, mfriendNode)

    i = i + 1
    if pluginProgress:
      pluginProgress.progress(i, nbFriends)
  
  if len(avatarsDlPath) > 0:
    viewShape.setAllNodeValue(tlp.NodeShape.Square)
    viewColor.setAllNodeValue(tlp.Color(255, 255, 255))
  else:
    viewShape.setAllNodeValue(tlp.NodeShape.Circle)
    viewColor.setAllNodeValue(tlp.Color(255, 0, 0))
  viewBorderColor.setAllNodeValue(tlp.Color(0, 0, 0))
  viewColor.setAllEdgeValue(tlp.Color(0,0,0))
  viewBorderColor.setAllEdgeValue(tlp.Color(0, 0, 0))
  viewBorderWidth.setAllNodeValue(1)
  viewBorderWidth.setAllEdgeValue(1)
  
  dataSet = tlp.getDefaultPluginParameters("FM^3 (OGDF)", graph)
  graph.applyLayoutAlgorithm("FM^3 (OGDF)", viewLayout, dataSet)

  if pluginProgress:
    pluginProgress.setComment("Finishing to download avatars")

  waitAvatarDlThreads()
  
  
