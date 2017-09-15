#!/usr/bin/env python

from paraview.simple import *

view = GetActiveView()
camera = GetActiveCamera()
layout = GetLayout(GetActiveView())
location = layout.GetViewLocation(view)

# going to need to update my paraview alias to simply open paraview while making the foam files for each of the argument files, or create the foam files from in here somehow
# how to have it read in a file picking a reader based on the extension!!!
#reader = OpenDataFile("/home/latwood/Documents/energyEqn/forMeeting/buoyantBoussinesqPimpleFoam/1mph0deg-InnerField-zeroGradientWalls/1mph0deg-InnerField-zeroGradientWalls.foam")	# looks like it gives it a funny name in paraview, not sure how to fix this but it is probably all right
reader = GetActiveSource()	# this works is using the paraFoamNew alias that only loads one file and nothing else has been created
dpReader = GetDisplayProperties(reader)	# get display properties of the reader
dpReader.ColorArrayName = ['CELLS','T']
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
dpGlyphFull = GetDisplayProperties(glyphFull)
dpGlyphFull.ColorArrayName = 'GlyphVector'
#ColorBy(dpGlyphFull,('POINTS','GlyphVector','Magnitude'))
Show(glyphFull)
Render()

#ctl = GetColorTransferFunction('GlyphVector')
#bar = CreateScalarBar(LookupTable=ctl, Title="Velocity")
#dpGlyphFull.RescaleTransferFunctionToDataRange()

#now make the slice of the full case
slice = Slice(reader)
slice.SliceType.Normal = [0,1,0]
#slice.SliceOffsetValues = [30,60,90,120]	# creates more slices in the single slice at differing offsets of the slice !!! Nice! Except how to toggle on and off? just reset the values
dpSlice = GetDisplayProperties(slice)	# get display properties of the reader
dpSlice.ColorArrayName = ['CELLS','T']
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
dpGlyphSlice = GetDisplayProperties(glyphSlice)
dpGlyphSlice.ColorArrayName = 'GlyphVector'
#ColorBy(dpGlyphSlice,('POINTS','GlyphVector','Magnitude'))
Show(glyphSlice)
Render()


#now all the stuff is made, now it's just a matter of manipulating how it is seen and taking pictures
#note that I can change view from the start view, but haven't figured out how to imitate the snap to north, south, top, or bottom view. Also haven't figured out how to get the legend to toggle on and off correctly, though there might be some slight workarounds with what I've got so far, since it somehow does the rescaling to data range as is seen at the time.

camera.Elevation(-45)		# change the active camera elevation
dpReader.RescaleTransferFunctionToDataRange()	# if I put the UpdateScalarBars here, or before the hide(reader), the legend comes out to be that used by the full case, not the glyphs.
# if I don't UpdateScalarBars till this guy is hidden, I get the glyph vectors
Render() 	
Hide(reader)	# deselect eye thing on the case so only vectors are shown
Hide(slice)
Hide(glyphSlice)
Render()
dpGlyphFull.RescaleTransferFunctionToDataRange() # this is the rescale function, forces the freakin legend to actually show up too. Not sure how to force legend settings to always have the nice format yet. Hm, still some issues and isn't the solution to toggle on or off different legends
UpdateScalarBars()
Render()
SetActiveSource(glyphFull)




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

#set the background color
#view.Background = [1,1,1]  #white
#set image size
#view.ViewSize = [1500, 1500] #[width, height]
#save screenshot
#WriteImage("test.png")

#legendReader = GetScalarBar(reader) #using reader doesn't work because it isn't a color transfer function


