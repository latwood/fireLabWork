require 'sketchup.rb'
require 'extensions.rb'

module WindNinjaSmoke
  module ParaViewSlices

    unless file_loaded?(__FILE__)
      ex = SketchupExtension.new('ImportParaViewSlices', 'ex_import_paraview_slices/main')
      ex.description = 'SketchUp Ruby API program that imports paraView slice images and exports them to a kmz.'
      ex.version     = '1.0.0'
      ex.copyright   = 'No Copyright: Trial'
      ex.creator     = 'Loren Atwood'
      Sketchup.register_extension(ex, true)
      file_loaded(__FILE__)
    end

  end
end
