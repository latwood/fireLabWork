#import some necessary modules
import pwd
import grp
import os

uid = pwd.getpwnam("latwood").pw_uid
gid = grp.getgrnam("latwood").gr_gid

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
#paraview.simple._DisableFirstRenderCameraReset()


### create needed changeable variables
mainDir = "/home/latwood/Documents/ParaViewVisualization/"	#warning, changing group owner for this dir and below
imgDir = mainDir+"/Pics/energyEqn/vshapedvalley-flatbot/buoyantBoussinesqPimpleFoam/1mph0deg-InnerField-zeroGradientWalls/glyphsFull-SouthView-Rescaling/"

originalViewSize = [906, 780]	# this is the original view size, need to get better at getting this. The problem is that if I call getViewSize, I get a proxy which changes
desiredPictureSize = [1500,1500] #[width, height]
UlegendPosition = [0.3,0.12]	# this will need to be adjusted a bunch
TlegendPosition = [0.3,0.12]	# this will need to be adjusted a bunch
viewCameraX = 500	# where in the x direction from the center do you want to position the object?
viewCameraY = 500	# where in the y direction from the center do you want to position the object?
viewCameraZ = 350	# where in the z direction from the center do you want to position the object?
cameraElev = -90	# the tilt to give the view. Need to do -90 for certain side views, mixed with viewUp
cameraViewUp = [0,0,1]	# this is for changing from which side to view
glyphFullScaleFactor = 450		# this is the size of the wind vectors
glyphFullStride = 5			# this is the every Nth number of points to use for showing vectors
glyphYsliceScaleFactor = 450		# this is the size of the wind vectors
glyphYsliceStride = 5		# this is the every Nth number of points to use for showing vectors
glyphXsliceScaleFactor = 450		# this is the size of the wind vectors
glyphXsliceStride = 5		# this is the every Nth number of points to use for showing vectors

#check to see if directory exists, and if not, create it
if not os.access(imgDir, os.F_OK):
	os.makedirs(imgDir,0777)
	#recursive unsudo the stuff. Doesn't do top directory so do that separate
	os.chown(mainDir,uid,gid)
	for root,dirs,files in os.walk(mainDir):
		for momo in dirs:
			os.chown(os.path.join(root,momo),uid,gid)
		for momo in files:
			os.chown(os.path.join(root,momo),uid,gid)


# get active view
view = GetActiveViewOrCreate('RenderView')

# current camera placement for renderView1
#view.CameraPosition = [0.0, 3000.0, 19186.51464195246]
#view.CameraFocalPoint = [0.0, 3000.0, 1571.7249755859375]
#view.CameraParallelScale = 4559.043041130733

camera = GetActiveCamera()
layout = GetLayout(GetActiveView())
location = layout.GetViewLocation(view)

# going to need to update my paraview alias to simply open paraview while making the foam files for each of the argument files, or create the foam files from in here somehow
# how to have it read in a file picking a reader based on the extension!!!
#reader = OpenDataFile("/home/latwood/Documents/energyEqn/forMeeting/buoyantBoussinesqPimpleFoam/1mph0deg-InnerField-zeroGradientWalls/1mph0deg-InnerField-zeroGradientWalls.foam")	# looks like it gives it a funny name in paraview, not sure how to fix this but it is probably all right

# get active source.
reader = GetActiveSource()

# get display properties
dpReader = GetDisplayProperties(reader,view=view)
ColorBy(dpReader, ('CELLS','T'))
dpReader.RescaleTransferFunctionToDataRange()
dpReader.SetScalarBarVisibility(view, True)
ctlReader = GetColorTransferFunction('T')
scalarbarReader = GetScalarBar(ctlReader,view)
#scalarbarReader.Title = 'T (K)'	# for some odd reason adding a title throws in the word magnitude
scalarbarReader.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarReader.Position = TlegendPosition
scalarbarReader.Orientation = 'Horizontal'
Show(reader)		# shows case (selects the eye), in whatever location the view is at
Render()			# centers the view on the case somehow

#now make the glyphs of the full case
glyphFull=Glyph(reader, GlyphType='Arrow')	#maybe try doing a glyph straight up of the reader
glyphFull.Scalars = ['CELLS', 'p']	# sets the glyph scalars to be cells of p
glyphFull.Vectors = ['CELLS', 'U']	# sets the glyph vectors to be cells of U
glyphFull.ScaleMode = 'off'	# enable scaling by scalars
glyphFull.ScaleFactor = glyphFullScaleFactor
glyphFull.GlyphMode = 'Every Nth Point'
glyphFull.Stride = glyphFullStride
dpGlyphFull = GetDisplayProperties(glyphFull,view=view)
ColorBy(dpGlyphFull, ('POINTS','GlyphVector'))
dpGlyphFull.RescaleTransferFunctionToDataRange()
dpGlyphFull.SetScalarBarVisibility(view, True)
ctlGlyphFull = GetColorTransferFunction('GlyphVector')
scalarbarGlyphFull = GetScalarBar(ctlGlyphFull,view)
#scalarbarGlyphFull.Title = 'Velocity (m/s)'	# for some odd reason adding a title throws in the word magnitude
scalarbarGlyphFull.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarGlyphFull.Position = UlegendPosition
scalarbarGlyphFull.Orientation = 'Horizontal'
Show(glyphFull)
Render()

#now make the y slice of the full case
ySlice = Slice(reader)
ySlice.SliceType.Normal = [0,1,0]
#ySlice.SliceOffsetValues = [30,60,90,120]	# creates more slices in the single slice at differing offsets of the slice !!! Nice! Except how to toggle on and off? just reset the values
dpYslice = GetDisplayProperties(ySlice,view=view)	# get display properties of the reader
ColorBy(dpYslice, ('CELLS','T'))
dpYslice.RescaleTransferFunctionToDataRange()
dpYslice.SetScalarBarVisibility(view, True)
ctlYslice = GetColorTransferFunction('T')
scalarbarYslice = GetScalarBar(ctlYslice,view)
#scalarbarYslice.Title = 'T (K)'	# for some odd reason adding a title throws in the word magnitude
scalarbarYslice.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarYslice.Position = TlegendPosition
scalarbarYslice.Orientation = 'Horizontal'
Show(ySlice)
Render()

#now create glyphs of the y slice
glyphYslice=Glyph(ySlice, GlyphType='Arrow')	#maybe try doing a glyph straight up of the reader
glyphYslice.Scalars = ['CELLS', 'p']	# sets the glyph scalars to be cells of p
glyphYslice.Vectors = ['CELLS', 'U']	# sets the glyph vectors to be cells of U
glyphYslice.ScaleMode = 'off'	# enable scaling by scalars
glyphYslice.ScaleFactor = glyphYsliceScaleFactor
glyphYslice.GlyphMode = 'Every Nth Point'
glyphYslice.Stride = glyphYsliceStride
dpGlyphYslice = GetDisplayProperties(glyphYslice,view=view)
ColorBy(dpGlyphYslice, ('POINTS','GlyphVector'))
dpGlyphYslice.RescaleTransferFunctionToDataRange()
dpGlyphYslice.SetScalarBarVisibility(view, True)
ctlGlyphYslice = GetColorTransferFunction('GlyphVector')
scalarbarGlyphYslice = GetScalarBar(ctlGlyphYslice,view)
#scalarbarGlyphYslice.Title = 'Velocity (m/s)'	# for some odd reason adding a title throws in the word magnitude
scalarbarGlyphYslice.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarGlyphYslice.Position = UlegendPosition
scalarbarGlyphYslice.Orientation = 'Horizontal'
Show(glyphYslice)
Render()

#now make the x slice of the full case
xSlice = Slice(reader)
xSlice.SliceType.Normal = [1,0,0]
#xSlice.SliceOffsetValues = [30,60,90,120]	# creates more slices in the single slice at differing offsets of the slice !!! Nice! Except how to toggle on and off? just reset the values
dpXslice = GetDisplayProperties(xSlice,view=view)	# get display properties of the reader
ColorBy(dpXslice, ('CELLS','T'))
dpXslice.RescaleTransferFunctionToDataRange()
dpXslice.SetScalarBarVisibility(view, True)
ctlXslice = GetColorTransferFunction('T')
scalarbarXslice = GetScalarBar(ctlXslice,view)
#scalarbarXslice.Title = 'T (K)'	# for some odd reason adding a title throws in the word magnitude
scalarbarXslice.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarXslice.Position = TlegendPosition
scalarbarXslice.Orientation = 'Horizontal'
Show(xSlice)
Render()

#now create glyphs of the x slice
glyphXslice=Glyph(xSlice, GlyphType='Arrow')	#maybe try doing a glyph straight up of the reader
glyphXslice.Scalars = ['CELLS', 'p']	# sets the glyph scalars to be cells of p
glyphXslice.Vectors = ['CELLS', 'U']	# sets the glyph vectors to be cells of U
glyphXslice.ScaleMode = 'off'	# enable scaling by scalars
glyphXslice.ScaleFactor = glyphXsliceScaleFactor
glyphXslice.GlyphMode = 'Every Nth Point'
glyphXslice.Stride = glyphXsliceStride
dpGlyphXslice = GetDisplayProperties(glyphXslice,view=view)
ColorBy(dpGlyphXslice, ('POINTS','GlyphVector'))
dpGlyphXslice.RescaleTransferFunctionToDataRange()
dpGlyphXslice.SetScalarBarVisibility(view, True)
ctlGlyphXslice = GetColorTransferFunction('GlyphVector')
scalarbarGlyphXslice = GetScalarBar(ctlGlyphXslice,view)
#scalarbarGlyphXslice.Title = 'Velocity (m/s)'	# for some odd reason adding a title throws in the word magnitude
scalarbarGlyphXslice.RangeLabelFormat = '%.3f' #'%6.3g'
scalarbarGlyphXslice.Position = UlegendPosition
scalarbarGlyphXslice.Orientation = 'Horizontal'
Show(glyphXslice)
Render()

# now that everything is made, show only the full glyphs from a top view, then step through each time saving pictures
camera.Elevation(cameraElev)
view.CameraViewUp = cameraViewUp
ResetCamera()		#reset the camera to the full view, if now camera view has changed, this should be looking from above
Render()
Hide(reader)	# deselect eye thing on the case so only vectors are shown
dpReader.SetScalarBarVisibility(view, False)
Hide(ySlice)
dpYslice.SetScalarBarVisibility(view, False)
Hide(glyphYslice)
dpGlyphYslice.SetScalarBarVisibility(view, False)
Hide(xSlice)
dpXslice.SetScalarBarVisibility(view, False)
Hide(glyphXslice)
dpGlyphXslice.SetScalarBarVisibility(view, False)
dpGlyphFull.SetScalarBarVisibility(view, True)
scalarbarGlyphFull.Position = UlegendPosition	# have to reset this every time you make the scalar bar visible again, SetScalarBarVisibility resets the legend position
Render()
SetActiveSource(glyphFull)

# current camera placement for renderView1 #must change the position and focal point equally if moving the view up, down, left, or right. if on the right looking spot, only changing camera position will zoom in or out.
#starting position is directly above the stuff, so camera position 2 is the zoom in or out. position 0 is left or right, position 1 is up or down.
view.CameraPosition[0] = view.CameraPosition[0]+viewCameraX
view.CameraPosition[1] = view.CameraPosition[1]+viewCameraY
view.CameraPosition[2] = view.CameraPosition[2]+viewCameraZ
view.CameraFocalPoint[0] = view.CameraFocalPoint[0]+viewCameraX
view.CameraFocalPoint[1] = view.CameraFocalPoint[1]+viewCameraY
view.CameraFocalPoint[2] = view.CameraFocalPoint[2]+viewCameraZ
#view.CameraParallelScale = 4559.043041130733
Render() 	#don't call reset camera or all this is put back. Make sure that the legend position is set again here

# get the timesteps for the for loop and for changing times
timeSteps = reader.TimestepValues
anim = GetAnimationScene()
anim.PlayMode = 'Snap To TimeSteps'

# set the view size for the pictures
view.ViewSize = desiredPictureSize
Render()

for i in range(0,len(timeSteps)):
	anim.AnimationTime = timeSteps[i]
	#view.ViewTime = timeSteps[i]
	#ResetCamera()
	Render()
	ColorBy(dpGlyphFull, ('POINTS','GlyphVector'))
	dpGlyphFull.RescaleTransferFunctionToDataRange()	#looks like if you throw a True into this final parenthesis, it only rescales if the value is greater
	dpGlyphFull.SetScalarBarVisibility(view, True)
	scalarbarGlyphFull.Position = UlegendPosition	# have to reset this every time you make the scalar bar visible again, SetScalarBarVisibility resets the legend position
	Render()
	WriteImage(imgDir+str(int(timeSteps[i]))+".png")
	#get rid of root ownership
	os.chown(imgDir+str(int(timeSteps[i]))+".png",uid,gid)

view.ViewSize = originalViewSize
Render()

#

#ctlGlyphFull.UpdateScalarBarsComponentTitle(dpGlyphFull.GetArrayInformationForColorArray())

#dpReader.RescaleTransferFunctionToDataRange(True)
#ctlT = GetColorTransferFunction('T')
#dpReader.SetScalarBarVisibility(view, True)
#ctlGlyphs = GetColorTransferFunction('GlyphVector')



#Change time
#timeSteps = reader.TimestepValues	# makes an array of each of the timesteps used, where timeSteps[0] is the first timeStep
#anim = GetAnimationScene()
#anim.PlayMode = 'Snap To TimeSteps'
#anim.AnimationTime = timeSteps[1]	# takes you to the first timestep
#view.ViewTime = timeSteps[1]	# looks like it is doing this already, but just do it to be safe. I've just noticed that AnimationTime and ViewTime are separate even though they are together, and there are ways to set one without setting the other. It will do stuff but it will not update the gui correctly in the display saying which timestep you are at.
#Render()

#view.CameraViewUp = [0 ,0, 1]	# this made the camera adjust to snap nice, but still kept the 45 deg angle. Use Render after these camera commands
#view.CameraPosition = [-15, 10, 0]	# zooms in crazy, does not do nice snapping
#view.CameraFocalPoint = [0, 10, 0]	#didn't see any noticable change, but I bet it changed the zooming in zooming out stuff
#maybe there is a different property that would be more useful
#found more information
# The FocalPoint - this controls the point the camera "looks" at
# The Position - this controls _where_ the camera is in space
# The ViewUp - this controls what the "up" direction is (i.e., the direction that goes bottom-to-top in the view).
#	The constraint is that ViewUp should be orthogonal to the direction from Position to FocalPoint (this is accessible through GetDirectionOfProjection)
# Normally what I do is
# a) Move the FocalPoint to the center of my object, or wherever on the object I want to center my view
# b) Move the Position to a point in the corresponding axis. For looking from positive X, move the camera to something like (1,0,0).
# c) Set the ViewUp to whatever should be the appropriate up vector for my scene - (0,0,1) for Z axis
# d) Call ResetCamera on the renderer to give me the full view. There's also an overload to ResetCamera that takes bounds, so you can reset to view only a specific region.
#camera.Dolly(int) makes the camera move further or closer

#dp = GetDisplayProperties(reader)	# get display properties of the reader
#dp.Representation = 'Surface With Edges'
#dp.LookupTable = MakeBlueToRedLT(0, 0.5)
				# apply button to the camera stuff?

#SetActiveSource(c)	# selects (doesn't make visible) the c object. !!! Nice !!!
#Delete(c)		# deletes the source c (not the variable)
#del c			# deletes the variable

#SetProperties(cone, Radius=0.2, Center=[2, 0.5, 0])	# or you can set the properties to a cone that doesn't have them already

#for prop in glyph:
#	print type(prop), prop.GetXMLLabel()	# prints out each of the available properties for glyphs
#dir(glyph)	# a better alternative

#set the background color
#view.Background = [1,1,1]  #white
#set image size
#view.ViewSize = [1500, 1500] #[width, height]
#save screenshot
#WriteImage("test.png")

#legendReader = GetScalarBar(reader) #using reader doesn't work because it isn't a color transfer function
#dpReader.SetScalarBarVisibility(view, True)
#dpReader.RescaleTransferFunctionToDataRange(True)

#ctl = GetColorTransferFunction('GlyphVector')
#bar = CreateScalarBar(LookupTable=ctl, Title="Velocity")
#dpGlyphFull.RescaleTransferFunctionToDataRange()

#dpReader.RescaleTransferFunctionToDataRange()	# if I put the UpdateScalarBars here, or before the hide(reader), the legend comes out to be that used by the full case, not the glyphs.
# if I don't UpdateScalarBars till this guy is hidden, I get the glyph vectors
#dpGlyphFull.RescaleTransferFunctionToDataRange() # this is the rescale function, forces the freakin legend to actually show up too. Not sure how to force legend settings to always have the nice format yet. Hm, still some issues and isn't the solution to toggle on or off different legends
#UpdateScalarBars()
#Render()


#looks like this is how you actually set the legend
#ColorBy(dpReader, ('POINTS','GlyphVector'))
#dpReader.RescaleTransferFunctionToDataRange(True)
#dpReader.SetScalarBarVisibility(view, True)
	
