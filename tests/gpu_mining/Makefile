

gpu_path := ./../../gpu/

# Options

output_file_path := ./output/
output_name := $(output_file_path)output.out
g++ := g++
flags := -g -lOpenCL -o $(output_name)

all: 

	# Gpu
	cp $(gpu_path)*.cpp $(gpu_path)*.h $(output_file_path)

	# Compilation
	$(g++) $(flags) $(output_file_path)*.cpp $(output_file_path)*.cp

	# Clean
	$(MAKE) clean

clean:

	rm $(output_file_path)*.cpp $(output_file_path)*.h
	