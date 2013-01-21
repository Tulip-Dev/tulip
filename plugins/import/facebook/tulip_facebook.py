from tulip import *
import facebook
import tempfile
import urllib2
import os

def downloadAvatar(url, directory, name):
  ext = url[len(url)-4:]
  fileName = directory+"/"+name+ext
  if not os.path.isfile(fileName):
    avatarFile = urllib2.urlopen(url)
    output = open(fileName,'wb')
    output.write(avatarFile.read())
    output.close()
  return fileName
  

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
  meNode = graph.addNode()
  viewLabel[meNode] = name
  nameProp[meNode] = name

  if len(avatarsDlPath) > 0:
    if pluginProgress:
      pluginProgress.setComment("Downloading your avatar")
    picture = fbGraph.get_object("me/picture")
    fileName = downloadAvatar(picture["url"], avatarsDlPath, name)
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
  
  for friend in friends["data"]:
    name = str(friend["name"])
    friendNode = friendsMap[name]
    
    if len(avatarsDlPath) > 0:
      if pluginProgress:
	pluginProgress.setComment("Downloading avatar of " + name)
      picture = fbGraph.get_object(friend["id"] + "/picture")
      fileName = downloadAvatar(picture["url"], avatarsDlPath, name)
      viewTexture[friendNode] = fileName
    
    if pluginProgress:
      pluginProgress.setComment("Checking your mutual friends with " + name)
    mutualFriends = fbGraph.get_object("me/mutualfriends/"+friend["id"])
    
    for mfriend in mutualFriends["data"]:
      mfriendNode = friendsMap[str(mfriend["name"])]
      if not graph.existEdge(friendNode, mfriendNode, False).isValid():
	graph.addEdge(friendNode, mfriendNode)
  
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
  
  