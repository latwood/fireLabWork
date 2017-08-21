require 'sketchup.rb'

module WindNinjaSmoke
  module ParaViewSlices

  #function for hiding all layers except for the chosen one
	def self.show_single_layer (xlayers,ylayers,single_layer_type,single_layer_index)
		# first check to make sure the single_layer_type is valid. If not, do nothing except warn
		if single_layer_type == "xlayer" || single_layer_type == "ylayer"
			#hide all layers, then make visible the one of interest
			for i in 0..xlayers.size-1
				xlayers[i].visible = false
			end	
			for i in 0..ylayers.size-1
				ylayers[i].visible = false
			end
			if single_layer_type == "xlayer"
				xlayers[single_layer_index].visible = true
			elsif single_layer_type == "ylayer"
				ylayers[single_layer_index].visible = true
			end	
		else
			puts "single_layer_type: #{single_layer_type} not valid!"
		end
	end
	  
    def self.ImportParaViewSlices
      model = Sketchup.active_model
	  model.start_operation('ImportParaViewSlices', true)	#wrap the rest with this function
	  entities = model.active_entities
	  view = model.active_view
	  
	  #first start by clearing all the entities
#	  entities.clear!
	  puts "starting number of entities: #{ entities.count}"
	  
	  inch_to_meter_multiplier = 0.0254	#in * 0.0254 m/in

	  #these variables are to be changed by the user. The idea is to get each of the corners of the overall mesh, 
	  # and the same mesh but one outer cell less, in UTM coordinates. 
	  # this lets calculations of cell centers and number of cells/pictures be possible
	 #use as many decimal places as possible when obtaining these points. Improves precision
	  mesh1stCell_botLeftCorner = Geom::UTM.new 11, "T", 723516.672, 5202238.293
	  mesh1stCell_topRightCorner = Geom::UTM.new 11, "T", 728649.9915, 5209814.578
	  mesh2ndCell_botLeftCorner = Geom::UTM.new 11, "T", 723648.2956, 5202371.211
	  mesh2ndCell_topRightCorner = Geom::UTM.new 11, "T", 728518.3679, 5209681.661
	  z_min = 1057.506677.m
	  z_max = 2520.7.m
	  tiltAngle = 2.1.degrees
	  timeStep = "34551"
	  importingPath = "../../../../../FSwork/SmokeTransportVisualization/pastGoogleEarth/GrantCreek1stOriginalPaper_momSolv_33.07mRes/pics/Transparent-" + timeStep + "-Pics/"
	  kmzPath = "C:\\Users\\latwood\\FSwork\\smokeTransportVisualization\\pastGoogleEarth\\GrantCreek1stOriginalPaper_momSolv_33.07mRes\\pics\\Transparent-" + timeStep + "-Pics\\"
	  show_every_blank_slice = 8	# so will hide each five slices between
      
	  
	  #these are calculated variables
	  cell_dx = (mesh2ndCell_botLeftCorner.x - mesh1stCell_botLeftCorner.x).m
	  puts "cell_dx = #{cell_dx*inch_to_meter_multiplier}"
	  cell_dy = (mesh2ndCell_botLeftCorner.y - mesh1stCell_botLeftCorner.y).m
	  puts "cell_dy = #{cell_dy*inch_to_meter_multiplier}"
	  mesh_dx = (mesh1stCell_topRightCorner.x-mesh1stCell_botLeftCorner.x).m
	  puts "mesh_dx = #{mesh_dx*inch_to_meter_multiplier}"
	  mesh_dy = (mesh1stCell_topRightCorner.y-mesh1stCell_botLeftCorner.y).m
	  puts "mesh_dy = #{mesh_dy*inch_to_meter_multiplier}"
	  mesh_dz = z_max-z_min
	  puts "mesh_dz = #{mesh_dz*inch_to_meter_multiplier}"
	  xslices_count = ((mesh_dx)/cell_dx).round
	  puts "xslices_count = #{xslices_count}"
	  yslices_count = ((mesh_dy)/cell_dy).round
	  puts "yslices_count = #{yslices_count}"
	  allSlices_count = xslices_count+yslices_count
	  puts "allSlices_count = #{allSlices_count}"
	  allSlices = Array.new(xslices_count+yslices_count)
	  
	  
	  # This is the bit that actually sets the geo-location, but hasn't yet added it to the model.
	  botLeftCornerLatLong = mesh1stCell_botLeftCorner.to_latlong
	  puts "botLeftCornerLatLong = #{botLeftCornerLatLong}"
	  shadowinfo = model.shadow_info
      shadowinfo["City"] = "Unknown"
      shadowinfo["Country"] = "Unknown"
	  shadowinfo["Latitude"] = botLeftCornerLatLong.latitude
	  shadowinfo["Longitude"] = botLeftCornerLatLong.longitude
	  
	# This bit makes SketchUp's 'Window->Model Info' menu option work correctly and 'model.georeferenced?' starts working too.
	# The person that showed how to do this online found SketchUp will crash if you don't set all of these. 
	# Looks like "GeoReferenceNorthAngle", "ModelTranslation", and "ModelHere" stuff doesn't care about what the value is
	# The real value is what angle and zoom the camera has on the model
	  key = "GeoReference"
	  model.set_attribute key, "GeoReferenceNorthAngle", 358.646700673226
	  model.set_attribute key, "UsesGeoReferencing", true
	  model.set_attribute key, "Longitude" , botLeftCornerLatLong.longitude
	  model.set_attribute key, "Latitude" , botLeftCornerLatLong.latitude
	  model.set_attribute key, "ModelTranslationX" ,  -12901579.2256146
	  model.set_attribute key, "ModelTranslationY" , -214086056.635273
	  model.set_attribute key, "ModelTranslationZ" , 0.0
	  model.set_attribute key, "LocationSource" , "Google Earth"
	  model.set_attribute key, "ZValueCentered" ,  0.0
	  model.set_attribute key, "TimeStamp", Time.now.to_i
	  model.set_attribute key, "ModelHereState", ""
	  model.set_attribute key, "ModelHereZoom", 19

	# Check to see if the model was actually referenced
	  georef = model.georeferenced?
	  puts "Is the model georeferenced? #{georef}"
	  
	  #since this next section just repeats for each type of slice, set up the allSlices_counter
	  allSlices_counter = 0
	  
	#y slices section
	  # add layer types
	  ylayers = Array.new(show_every_blank_slice)
	  for i in 0..show_every_blank_slice-1
		ylayers[i] = model.layers.add "ylayer#{i}"
	  end
 
	  yslices = Array.new(yslices_count)
	  layerCount = 0
	  for i in 0..yslices_count-1
		file_name = timeStep + "-yslice#{i+1}.png"
		image_file = Sketchup.find_support_file(file_name, importingPath)
		point = Geom::Point3d.new(0, cell_dy*i+cell_dy/2, 0)	# changes each slice
		image = entities.add_image(image_file, point, mesh_dx)	# looks like it will always be flat
		rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(1, 0, 0), 90.degrees)
		entities.transform_entities(rotation,image)
		#adjust height
		if i == 0
			firstImage_height = image.height	# notice this should assign this variable so that all the rest of the loops use this value
		else
			adjust_height = firstImage_height - image.height	# so technically this height is as if the first slice were directly on the origin. It is not, so there is a slight distortion in height. Seems to be all right, since the only way to fix it is to know some kind of height difference between the first slice and the origin actually found in google earth. We don't really have anything for this yet
			z_adjust_point = Geom::Point3d.new(0,cell_dy*i+cell_dy/2,adjust_height)
			translation = Geom::Transformation.translation(point.vector_to(z_adjust_point))
			image.transform! translation
		end
		#now add the appropriate layer
		image.layer = ylayers[layerCount]
		if layerCount == show_every_blank_slice-1
			layerCount = 0	#reset counter
		else
			layerCount = layerCount + 1
		end
		yslices[i] = image
		allSlices[allSlices_counter] = image
		allSlices_counter = allSlices_counter + 1
	  end
	  
	  #now add in the T scale for the y axis and add it to all slices, making a layer for each axis type. Put it over the first slice
	  file_name = timeStep + "-Tscale.png"
	  image_file = Sketchup.find_support_file(file_name, importingPath)
	  point = Geom::Point3d.new(0, cell_dy/2, 0)	# changes each slice
	  yTscale = entities.add_image(image_file, point, mesh_dx)	# looks like it will always be flat
	  rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(1, 0, 0), 90.degrees)
	  entities.transform_entities(rotation,yTscale)
	  adjust_height = firstImage_height	# so technically this height is as if the first slice were directly on the origin. It is not, so there is a slight distortion in height. Seems to be all right, since the only way to fix it is to know some kind of height difference between the first slice and the origin actually found in google earth. We don't really have anything for this yet
	  z_adjust_point = Geom::Point3d.new(0,cell_dy/2,adjust_height+yTscale.height/8)
  	  translation = Geom::Transformation.translation(point.vector_to(z_adjust_point))
	  yTscale.transform! translation
	  yTscale.layer = model.layers.add "yTscaleLayer"
	  allSlices[allSlices_counter] = yTscale
	  allSlices_counter = allSlices_counter + 1
 
	#x slices section
	  # add layer types
	  xlayers = Array.new(show_every_blank_slice)
	  for i in 0..show_every_blank_slice-1
		xlayers[i] = model.layers.add "xlayer#{i}"
	  end
 
	  xslices = Array.new(xslices_count)
	  layerCount = 0
	  for i in 0..xslices_count-1
		file_name = timeStep + "-xslice#{i+1}.png"
		image_file = Sketchup.find_support_file(file_name, importingPath)
		point = Geom::Point3d.new(0, 0, 0)	# changes each slice
		image = entities.add_image(image_file, point, mesh_dy)	# looks like it will always be flat
		rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(1, 0, 0), 90.degrees)
		entities.transform_entities(rotation,image)
		rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(0, 0, 1), -90.degrees)
		entities.transform_entities(rotation,image)
		translation = Geom::Transformation.translation(point.vector_to(Geom::Point3d.new(cell_dx*i+cell_dx/2,mesh_dy,0)))
		point = Geom::Point3d.new(cell_dx*i+cell_dx/2,0,0)
		image.transform! translation
		#adjust height
		if i == 0
			firstImage_height = image.height	# notice this should assign this variable so that all the rest of the loops use this value
		else
			adjust_height = firstImage_height - image.height	# so technically this height is as if the first slice were directly on the origin. It is not, so there is a slight distortion in height. Seems to be all right, since the only way to fix it is to know some kind of height difference between the first slice and the origin actually found in google earth. We don't really have anything for this yet
			z_adjust_point = Geom::Point3d.new(cell_dx*i+cell_dx/2,0,adjust_height)
			translation = Geom::Transformation.translation(point.vector_to(z_adjust_point))
			image.transform! translation
		end
		#now add the appropriate layer
		image.layer = xlayers[layerCount]
		if layerCount == show_every_blank_slice-1
			layerCount = 0	#reset counter
		else
			layerCount = layerCount + 1
		end
		xslices[i] = image
		allSlices[allSlices_counter] = image
		allSlices_counter = allSlices_counter + 1
	  end
	  
	  #now add in the T scale for the x axis and add it to all slices, making a layer for each axis type. Put it over the first slice
	  file_name = timeStep + "-Tscale.png"
	  image_file = Sketchup.find_support_file(file_name, importingPath)
	  point = Geom::Point3d.new(0, 0, 0)
	  xTscale = entities.add_image(image_file, point, mesh_dy)
	  rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(1, 0, 0), 90.degrees)
	  entities.transform_entities(rotation,xTscale)
	  rotation = Geom::Transformation.rotation(point, Geom::Vector3d.new(0, 0, 1), -90.degrees)
	  entities.transform_entities(rotation,xTscale)
	  translation = Geom::Transformation.translation(point.vector_to(Geom::Point3d.new(cell_dx/2,mesh_dy,firstImage_height+xTscale.height/8)))
	  xTscale.transform! translation
	  xTscale.layer = model.layers.add "xTscaleLayer"
	  allSlices[allSlices_counter] = xTscale
	  allSlices_counter = allSlices_counter + 1
	  
	  #rotate to needed dem location
	  rotation = Geom::Transformation.rotation([0,0,0],[0,0,1],-tiltAngle)
	  entities.transform_entities(rotation,allSlices)	#warns it expects an entity, not entities, if you get the array reference wrong and one of the spaces is nil
	  
	  view.zoom_extents	#this way if stuff after fails, it at least does this step

	  # Export model to kmz file for Google Earth
		xTscale.layer.visible = false	#hide all T scales but the first that will come, just need one
		  model.export kmzPath + timeStep + "-allTogether.kmz", true
	  
###### now the entire box of stuff is finished. hide layers and show in different view angles to output the kmz files	  

	  #almost easiest to find this by running it once, zooming around till you find a view you like, 
	  #then using the ruby console to retrieve this info
	  #this is especially true since it changes so much with so little and is hard to see what works
	  #recommend zooming to extents right before retrieving the view.camera info
	  eye = [2420.724156.m, -7479.064646.m, 4526.217733.m]
	  target = [2650.453531.m, 3300.442503.m, -39.720778.m]
	  up = [0.008308698033197089, 0.3898659891864517, 0.9208341197048807]
	  my_camera = Sketchup::Camera.new eye, target, up
	  view.camera = my_camera
	  view.invalidate
	  
	  for i in 0..show_every_blank_slice-1
		show_single_layer(xlayers,ylayers,"ylayer",i)
		file_name = timeStep + "-yslices#{i+1}.kmz"
		# Export model to kmz file for Google Earth
		  model.export kmzPath + file_name, true
	  end

	  #almost easiest to find this by running it once, zooming around till you find a view you like, 
	  #then using the ruby console to retrieve this info
	  #this is especially true since it changes so much with so little and is hard to see what works
	  #recommend zooming to extents right before retrieving the view.camera info
	  eye = [-11096.605859.m, 4627.433737.m, 5049.580642.m]
	  target = [2504.542023.m, 3660.820305.m, 168.104545.m]
	  up = [0.3362030595880566, -0.023893453396387983, 0.9414863809999721]
	  my_camera = Sketchup::Camera.new eye, target, up
	  view.camera = my_camera
	  view.invalidate
	  
	  yTscale.layer.visible = false	#hide all T scales but the first that will come, just need one
	  xTscale.layer.visible = true	#hide all T scales but the first that will come, just need one
	  for i in 0..show_every_blank_slice-1
		show_single_layer(xlayers,ylayers,"xlayer",i)
		file_name = timeStep + "-xslices#{i+1}.kmz"
		# Export model to kmz file for Google Earth
		  model.export kmzPath + file_name, true
	  end
	  
######finished. Count number of entities (in case needed)
	  puts "ending number of entities: #{ entities.count}"  
	  
	  model.commit_operation	# end wrapper function
    end

    unless file_loaded?(__FILE__)
      menu = UI.menu('Plugins')
      menu.add_item('ImportParaViewSlices') {
        self.ImportParaViewSlices
      }
      file_loaded(__FILE__)
    end

  end
end
