#!/usr/bin/env python

from paraview.simple import *

cone = Cone()	# create a cone
help(cone)	# get help about the cone object
cone.Resolution	# get cone resolution
cone.Resolution = 32	# set cone resolution
cone = Cone(Resolution=32)	# alternative method to set cone resolution
cone.Center	# get cone center
cone.Center = [1, 2, 3]	# set cone center
cone.Center[0:2] = [2, 4]	# set part of cone center
cone.Center	# get cone center
shrinkFilter = Shrink(cone)	# create a shrink filter of the cone
shrinkFilter.Input		# get shrink filter input information or something
shrinkFilter.UpdatePipeline()	# supposedly updates the pipline? I don't see much
shrinkFilter.GetDataInformation().GetNumberOfCells()	# gets the number of cells of shrink filter
shrinkFilter.GetDataInformation().GetNumberOfPoints()	# gets the number of points of shrink
Show(shrinkFilter)		# shows shrink filter (selects the eye), in whatever location the view is at
Render()			# centers the view on the shrink filter somehow

sphere = Sphere()		# create a sphere
sphere.Center = [1.0, 1.0, 0.0]	#set sphere center
sphere.Radius = 3		# set sphere radius

c = Cone()	# create a cone
GetActiveSource()	# get the active source
Shrink()		# shrinks the last addressed item (the active source)
SetActiveSource(c)	# selects (doesn't make visible) the cone object. !!! Nice !!!
Delete(c)		# deletes the source c (not the variable)
del c			# deletes the variable

GetSources()	# shows all the stuff loaded (including openfoam case)
FindSource('Cone1')	# gets the Cone1 source. Unfortunately you have to know the name and it doesn't seem easy to get the name from GetSources (unless maybe put results of GetSources() into an array? Would need to sort it after since it returns stuff in alphabetical order, so how could you know which is the first? Will be better to find a function that simply opens it new to paraview)

cone = Cone(Radius=0.5, Center=[1, 0.5, 0])	# create a cone with the already shown properties
SetProperties(cone, Radius=0.2, Center=[2, 0.5, 0])	# or you can set the properties to a cone that doesn't have them already


sph = Sphere()		# create a sphere
elev=Elevation(sph)	# create an elevation of the sphere
glyph=Glyph(elev, GlyphType='Sphere') # glyph the points of the sphere with spheres (so elevation is actually the points? I tried to view it and didn't see anything)
glyph.Scalars = 'Elevation'	# scale the glyph with the Elevation array
glyph.Scalars	# shows information about the scalars used in the glyphs, including that they are associated with the points and scaled to elevation
glyph.Scalars = ['POINTS', 'Elevation']	# setting the same as above in another way I guess
glyph.ScaleMode = 'scalar'	# enable scaling by scalars
#so the above is changing the properties of the glyphs to what would it be scaled by or the attributes and scaling. Nice !!! Usually my default gets set to stupid p and u points and not cell centers

for prop in glyph:
	print type(prop), prop.GetXMLLabel()	# prints out each of the available properties for glyphs

it = iter(sph)	# iter is the property iterator object
for i in it:
	print it.GetKey(), it.GetProperty()		# get list of properties for the sphere

s = Sphere()	# create a sphere named s
Show(s)			# make sphere visible in paraview
dp = GetDisplayProperties(s)	# get display properties of the sphere
dp.Representation	# show the representation of the display properties of the sphere
#dp.GetProperty("Representation").Available	# show all available display properties of the sphere. Doesn't work because of available part
dp.Representation = 'Outline'	# change the display to be an outline (nice I can change from surface to surface with edges then)

di = glyph.GetDataInformation(0)	# get glyph meta data information for the last output
di	# display the obtained information
glyph.UpdatePipeline()	# made a change to the output doing updates on anything that has happened
di.GetDataClassName()	# get the data type
pdi = glyph.PointData   # get information about the data points
len(pdi)				# directly access the wrapper for a vtk class
ai = pdi[0]				# get information for a point array
ai.GetRange(0)			# get the range of values in the point array

# can fetch the data directly from the server which is not a good idea if the data is big
# you have to connect to a server first, then fetch the data using server manager.	

view = CreateRenderView()	# creates a new view of stuff to work with. A new render window!
# probably best to just use regular Render() unless maybe this is required for use outside of paraview
GetRenderViews()	# shows the different render views open in paraview
GetActiveView()		# gets the currently active render view
Show(elev, GetRenderViews()[1])	# choosing to have something active in a specific render view
# not sure why using 0 instead of 1 gets the right render view
Render(GetRenderViews()[1])	#forces a render on one specific render view, so a zoom in

camera = GetActiveCamera()	# get the active camera to manipulate the view angle
camera.Elevation(45)		# change the active camera elevation
Render() 					# apply button to the camera stuff?

dp = GetDisplayProperties(elev)
dp.LookupTable = MakeBlueToRedLT(0, 0.5)
dp.LookupTable = GetLookupTableForArray("Elevation", 1,
   RGBPoints = [0, 0, 0, 1, 5, 1, 0, 0],
   ColorSpace = "HSV")	# looks like this idea is to create a sortable array! Super userful for getting the file name maybe?
dp.ColorArrayName = 'Elevation'			# change stuff in the lookup table. Affects the original stuff
Render()

view = GetActiveView()	# get active view
view.ViewTime		# get the current viewtime or time step shown in the viewer
#reader = GetActiveSource()		# needed to get timesteps if the source has a type of timesteps
view.ViewTime = 1		# somehow this and render aren't enough. Not changing the view
Render()

###############################
# how to have it read in a file picking a reader based on the extension!!!
reader = OpenDataFile("/home/latwood/Documents/energyEqn/forMeeting/buoyantBoussinesqPimpleFoam/1mph0deg-InnerField-zeroGradientWalls/1mph0deg-InnerField-zeroGradientWalls.foam")	# looks like it gives it a funny name in paraview, not sure how to fix this but it is probably all right
##############################

###writer = CreateWriter(".../foo.csv", source) ## for outputting data files. Didn't work for png files, so maybe other similar command for that or 3-D files for sketchup
#writer.FieldAssociation = "Points"
#writer.UpdatePipeline()
#del writer

layout = GetLayouts() #get the layout of the active view
# layout.SplitViewVertical(view = GetActiveView())	# split the active view vertically (use horizontal for horizontal split, can add argument of fraction 0.7 for sizing)
layout = GetLayout(GetActiveView())
location = layout.GetViewLocation(view)
layout.MaximizeCell(location)	# zooms in?


# now need to figure out how to do this without opening paraview and putting it into the shell
# then need to figure out how to create an object that is the foam file or some other file so that I can start doing cool stuff with our openfoam cases

