
transaction_management_path := ./../../transaction_management/
helper_path := ./../../helper/
type_path := ./../../type/
gpu_path := ./../../gpu/

openCl_kernel_functions_path := $(transaction_management_path)openCl_kernel_functions/

# Options

output_file_path := ./output/
output_name := $(output_file_path)output.out
g++ := g++
flags := -g -lOpenCL -o $(output_name)

all: 

	# OpenCl functions
	cp $(openCl_kernel_functions_path)*.spyke $(output_file_path)

	# Transaction Management
	cp $(transaction_management_path)*.cpp $(transaction_management_path)*.h $(output_file_path)

	# Helper
	cp $(helper_path)*.cpp $(helper_path)*.h $(output_file_path)

	# Type
	cp $(type_path)*.cpp $(type_path)*.h $(output_file_path)

	# Gpu
	cp $(gpu_path)*.cpp $(gpu_path)*.h $(output_file_path)

	# Compilation
	$(g++) $(flags) $(output_file_path)*.cpp $(output_file_path)*.cp

	# Clean
	$(MAKE) clean

clean:

	rm $(output_file_path)*.cpp $(output_file_path)*.h