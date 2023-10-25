

transaction_management_open_cl_kernel_functions_path = ./../../transaction_management/opencl_functions/
transaction_management_path := ./../../transaction_management/
open_cl_path = ./../../opencl/
helper_path := ./../../helper/
types_path := ./../../types/
gpu_path := ./../../gpu/

# Options

output_file_path := ./output/
output_name := $(output_file_path)output.out
g++ := g++
flags := -g -lOpenCL -o $(output_name)

all: 

	# Main file
	cp *.cpp $(output_file_path)

	# Transaction Management Kernel functions
	cp $(transaction_management_open_cl_kernel_functions_path)*.spyke $(output_file_path)

	# Opencl functions
	cp $(open_cl_path)*.cpp $(open_cl_path)*.h $(output_file_path)

	# Types
	cp $(types_path)*.cpp $(types_path)*.h $(output_file_path)

	# Gpu
	cp $(gpu_path)*.cpp $(gpu_path)*.h $(output_file_path)

	# Helper
	cp $(helper_path)*.cpp $(helper_path)*.h $(output_file_path)

	# Transaction Management
	cp $(transaction_management_path)*.cpp $(transaction_management_path)*.h $(output_file_path)

	# Compilation
	$(g++) $(flags) $(output_file_path)*.cpp

	# Clean
	$(MAKE) clean

clean:

	rm $(output_file_path)*.cpp $(output_file_path)*.h
