import os

AddOption('--gdb',
		  dest='gdb',
		  action='store_true',
		  default=False)
AddOption('--gprof',
		  dest='gprof',
		  action='store_true',
		  default=False)

boost_libs = Split('boost_filesystem boost_program_options boost_system boost_regex')

opencv_libs = Split('opencv_core opencv_highgui opencv_imgproc opencv_objdetect opencv_ml opencv_imgcodecs opencv_videoio')

cuda_libs = Split('''cudart''')

flycapture_libs = Split('flycapture flycapturegui')

includes = Split('''
	#core/include 
	#camera/include
	#demo/include
	/usr/local/include/opencv2 
	/usr/local/include/opencv 
	/usr/include/flycapture/ 
	/usr/local/include/flycapture/
	/usr/local/cuda-8.0/include/
	''')

gpu_includes = Split('''
	/home/ecestudent/research/eco_feature/core/include/ 
	/usr/local/include/opencv2 
	/usr/local/include/opencv 
	/usr/local/cuda-8.0/include/
	''')

cxx_flags = "-Wall -O3 -fopenmp -std=c++11"
link_flags = "-fopenmp"

if GetOption('gprof'):
	cxx_flags = "-pg -std=c++11"
	link_flags = "-pg"

if GetOption('gdb'):
	cxx_flags = "-g -fvar-tracking -std=c++11"
	link_flags = "-g -fvar-tracking"


env = Environment(CPPPATH=includes, CXXFLAGS=cxx_flags, LINKFLAGS=link_flags, 
		LIBS=boost_libs + opencv_libs + cuda_libs + flycapture_libs,
		LIBPATH=['/usr/local/lib'],
		LD_LIBRARY_PATH=['/usr/local/lib'])

cuda_env = Environment(CC = '/usr/bin/nvcc',CPPPATH = gpu_includes)
cuda_env.Append(LIBS = opencv_libs)
cuda_env.Tool('nvcc',toolpath = ['/home/ecestudent/nvcc/'])


objs = []
for subdir in ['core','camera','demo']:
	o = SConscript('%s/SConscript' % subdir,exports=['env','cuda_env','objs'],variant_dir='#build/%s' % subdir, duplicate=0)
	objs.append(o)
# SConscript('core/SConscript',exports='env',variant_dir='#build/core', duplicate=0)
# SConscript('camera/SConscript',exports='env',variant_dir='#build/camera', duplicate=0)
# SConscript('demo/SConscript',exports='env',variant_dir='#build/demo', duplicate=0)

