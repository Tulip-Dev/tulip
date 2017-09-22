from tulip import tlp
import tulipplugins

class Test(tlp.Algorithm):
	def init(self, context):
		tlp.Algorithm.init(self, context)
		self.addStringParameter("property", "the property to set", "", True, True, True)
	def check(self):
		return (True, "")
	def run(self):
		return True

tulipplugins.registerPlugin("Test", "A simple Python plugin test", "", "19/07/2016", "", "1.0")
