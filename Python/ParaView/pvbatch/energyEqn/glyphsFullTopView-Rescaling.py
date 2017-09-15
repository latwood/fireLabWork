#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
#paraview.simple._DisableFirstRenderCameraReset()

# get active view
view = GetActiveViewOrCreate('RenderView')
originalViewSize = [906, 780]	# this is the original view size, need to get better at getting this

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
scalarbarReader.Position = [0.45,0.1]
scalarbarReader.Orientation = 'Horizontal'
Show(reader)		# shows case (selects the eye), in whatever location the view is at
Render()			# centers the view on the case somehow

#now make the glyphs of the full case
glyphFull=Glyph(reader, GlyphType='Arrow')	#maybe try doing a glyph straight up of the reader
glyphFull.Scalars = ['CELLS', 'p']	# sets the glyph scalars to be cells of p
glyphFull.Vectors = ['CELLS', 'U']	# sets the glyph vectors to be cells of U
glyphFull.ScaleMode = 'off'	# enable scaling by scalars
glyphFull.ScaleFactor = 450
glyphFull.GlyphMode = 'Every Nth Point'
glyphFull.Stride = 5
dpGlyphFull = GetDisplayProperties(glyphFull,view=view)
ColorBy(dpGlyphFull, ('POINTS','GlyphVector'))
dpGlyphFull.RescaleTransferFunctionToDataRange()
dpGlyphFull.SetScalarBarVisibility(view, True)
ctlGlyphFull = GetColorTransferFunction('GlyphVector')
scalarbarGlyphFull = GetScalarBar(ctlGlyphFull,view)
scalarbarGlyphFull.Position = [0.45,0.1]
scalarbarGlyphFull.Orientation = 'Horizontal'
Show(glyphFull)
Render()

#now make the slice of the full case
slice = Slice(reader)
slice.SliceType.Normal = [0,1,0]
#slice.SliceOffsetValues = [30,60,90,120]	# creates more slices in the single slice at differing offsets of the slice !!! Nice! Except how to toggle on and off? just reset the values
dpSlice = GetDisplayProperties(slice,view=view)	# get display properties of the reader
ColorBy(dpSlice, ('CELLS','T'))
dpSlice.RescaleTransferFunctionToDataRange()
dpSlice.SetScalarBarVisibility(view, True)
ctlSlice = GetColorTransferFunction('T')
scalarbarSlice = GetScalarBar(ctlSlice,view)
scalarbarSlice.Position = [0.45,0.1]
scalarbarSlice.Orientation = 'Horizontal'
Show(slice)
Render()

#now create glyphs of the slice
glyphSlice=Glyph(slice, GlyphType='Arrow')	#maybe try doing a glyph straight up of the reader
glyphSlice.Scalars = ['CELLS', 'p']	# sets the glyph scalars to be cells of p
glyphSlice.Vectors = ['CELLS', 'U']	# sets the glyph vectors to be cells of U
glyphSlice.ScaleMode = 'off'	# enable scaling by scalars
glyphSlice.ScaleFactor = 450
glyphSlice.GlyphMode = 'Every Nth Point'
glyphSlice.Stride = 5
dpGlyphSlice = GetDisplayProperties(glyphSlice,view=view)
ColorBy(dpGlyphSlice, ('POINTS','GlyphVector'))
dpGlyphSlice.RescaleTransferFunctionToDataRange()
dpGlyphSlice.SetScalarBarVisibility(view, True)
ctlGlyphSlice = GetColorTransferFunction('GlyphVector')
scalarbarGlyphSlice = GetScalarBar(ctlGlyphSlice,view)
scalarbarGlyphSlice.Position = [0.45,0.1]
scalarbarGlyphSlice.Orientation = 'Horizontal'
Show(glyphSlice)
Render()

# now that everything is made, show only the full glyphs from a top view, then step through each time saving pictures
ResetCamera()		#reset the camera to the full view, if now camera view has changed, this should be looking from above
Render()
Hide(reader)	# deselect eye thing on the case so only vectors are shown
Hide(slice)
Hide(glyphSlice)
Render()
SetActiveSource(glyphFull)

# current camera placement for renderView1 #must change the position and focal point equally if moving the view up, down, left, or right. if on the right looking spot, only changing camera position will zoom in or out.
#starting position is directly above the stuff, so camera position 2 is the zoom in or out. position 0 is left or right, position 1 is up or down.
#view.CameraPosition[0] = view.CameraPosition[0]+1000
view.CameraPosition[1] = view.CameraPosition[1]-300
#view.CameraPosition[2] = view.CameraPosition[2]+1000
#view.CameraFocalPoint[0] = view.CameraFocalPoint[0]+1000
view.CameraFocalPoint[1] = view.CameraFocalPoint[1]-300
#view.CameraFocalPoint[2] = view.CameraFocalPoint[2]+1000
#view.CameraParallelScale = 4559.043041130733
Render() 	#don't call reset camera or all this is put back

# get the timesteps for the for loop and for changing times
timeSteps = reader.TimestepValues
anim = GetAnimationScene()
anim.PlayMode = 'Snap To TimeSteps'

for i in range(0,len(timeSteps)):
	anim.AnimationTime = timeSteps[i]
	#view.ViewTime = timeSteps[i]
	#ResetCamera()
	Render()
	ColorBy(dpGlyphFull, ('POINTS','GlyphVector'))
	dpGlyphFull.RescaleTransferFunctionToDataRange()	#looks like if you throw a True into this final parenthesis, it only rescales if the value is greater
	dpGlyphFull.SetScalarBarVisibility(view, True)
	view.ViewSize = [1500,1500] #[width, height]
	Render()
	WriteImage(str(int(timeSteps[i]))+".png")

view.ViewSize = originalViewSize
Render()

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

