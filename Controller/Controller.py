import urllib.request
import sys
from PIL import Image




#get the rest server. ex: 192.168.0.13:8080
restServer=sys.argv[1]

#get the name of image file to load
imageFileName=sys.argv[2]


LEDPANELSIZE=(8, 8)
LEDRGBPIXELS=[]

print("Open "+imageFileName)
im = Image.open(imageFileName)

print("Original size is")
print ( im.size)
if (im.size!=LEDPANELSIZE):
    print ("Resize image to")
    im= im.resize(LEDPANELSIZE)
    print (im.size)

print("Build list of rgb pixels from image")
imRGB=im.convert('RGB')

for x in range(0,8):
  for y in range(0,8):

    if (x % 2 ==0 ):
      LEDRGBPIXELS.append('%02X%02X%02X' % imRGB.getpixel((x,y)))
    else:
      LEDRGBPIXELS.append('%02X%02X%02X' % imRGB.getpixel((x,7-y)))

imageAsString = ','.join (LEDRGBPIXELS)

restRequest="http://"+restServer+"/do?colors="+imageAsString

print ("Execute HTTP GET "+restRequest)
urllib.request.urlopen(restRequest).read()
