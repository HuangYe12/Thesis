#ifndef __linux__
//#include "stdafx.h"
#else
#include <string.h>
#endif
 
#include "cimg/CImg.h"
#include "common/utils.h"
#include "common/basic.hpp"
#include "common/ocl_macros.h"

#include <CL/cl.h>
#include <CL/cl_gl.h>


#define PAD_LINES 2
#define USE_OLD_API 1
#define USE_CL_IMAGE 1
#define GET_DEVICE_INFO 1



// 3x3 sobel filter kernel g = sqrt(gx*gx + gy*gy)
// use of 8 bit gray value input

#if USE_CL_IMAGE

// Scalar version of kernel for Images
const char *binomial_kernel =
"__kernel void BinomialFilter(__read_only image2d_t image_in, __write_only image2d_t image_out)\n"
"{\n"
"   const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n"
"	const int x = get_global_id(0);\n"
"	const int y = get_global_id(1);\n"
"\n"
"	uint4 l_g = (uint4)(0,0,0,0);\n"
"   const int reach = 1;\n"
"	l_g = 4*read_imageui(image_in, sampler, (int2)(x,       y)) \n "
"			+ 1*read_imageui(image_in, sampler, (int2)(x-reach, y-reach)) \n "
"			+ 2*read_imageui(image_in, sampler, (int2)(x,       y-reach)) \n "
"			+ 1*read_imageui(image_in, sampler, (int2)(x+reach, y-reach)) \n "
"			+ 2*read_imageui(image_in, sampler, (int2)(x-reach, y)) \n "
"			+ 2*read_imageui(image_in, sampler, (int2)(x+reach, y)) \n "
"			+ 1*read_imageui(image_in, sampler, (int2)(x-reach, y+reach)) \n "
"			+ 2*read_imageui(image_in, sampler, (int2)(x,       y+reach)) \n "
"			+ 1*read_imageui(image_in, sampler, (int2)(x+reach, y+reach));\n"
"	uint output = (uint)(l_g.x >> 4);\n"
"   write_imageui(image_out, (int2)(x, y), (uint4)(output, output, output, output));\n"
"}\n";

#else

// Scalar version of kernel
const char *binomial_kernel =
"__kernel void BinomialFilter(const __global uchar* pSrc, __global uchar* pDst)\n"
"{\n"
"	const int x = get_global_id(0);\n"
"	const int y = get_global_id(1);\n"
"\n"
"   const int reach = 1;"
"	const int width = get_global_size(0);\n"
"	const int iOffset = y * width + x;\n"
"	const int iPrev = iOffset - reach * width;\n"
"	const int iNext = iOffset + reach * width;\n"
"\n"
"	short l_g;\n"
"	l_g = pSrc[iPrev - reach] + 2 * pSrc[iPrev] + pSrc[iPrev + reach];\n"
"	l_g = l_g + 2 * pSrc[iOffset - reach] + 4 * pSrc[iOffset] + 2 * pSrc[iOffset + reach];\n"
"	l_g = l_g + pSrc[iNext - reach] + 2 * pSrc[iNext] + pSrc[iNext + reach];\n"
"\n"
"	// store result into memory\n"
"	pDst[iOffset] = (uchar)(l_g / 16);\n"
"}\n";

#endif

//function to print out device info
void PrintDeviceInfo(cl_device_id device)
{
    char queryBuffer[1024];
    cl_int queryInt;
	cl_uint queryUint;
	cl_ulong queryULong;
	size_t num_workItems;
	size_t workitems[3];
    cl_int clError;
	cl_bool querystatus = false;
	clError = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS,sizeof(cl_uint), &queryUint,NULL);
	printf("CL_DEVICE_ADDRESS_BITS: %d\n", queryUint);
    clError = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(queryBuffer), &queryBuffer, NULL);
    printf("CL_DEVICE_NAME: %s\n", queryBuffer);
    queryBuffer[0] = '\0';
    clError = clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(queryBuffer), &queryBuffer, NULL);
    printf("CL_DEVICE_VENDOR: %s\n", queryBuffer);
    queryBuffer[0] = '\0';
    clError = clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeof(queryBuffer), &queryBuffer, NULL);
    printf("CL_DRIVER_VERSION: %s\n", queryBuffer);
    queryBuffer[0] = '\0';
    clError = clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(queryBuffer), &queryBuffer, NULL);
    printf("CL_DEVICE_VERSION: %s\n", queryBuffer);
    queryBuffer[0] = '\0';
	clError = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(int), &queryInt, NULL);
	printf("CL_DEVICE_MAX_COMPUTE_UNITS: %d\n", queryInt);
	clError = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &queryULong, NULL);
	printf("CL_DEVICE_LOCAL_MEM_SIZE: %u\n", queryULong);
	clError = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &num_workItems, NULL);
	printf("CL_DEVICE_MAX_WORK_GROUP_SIZE: %d\n", num_workItems);
	clError = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t[3]), &workitems, NULL);
	std::cout << "CL_DEVICE_MAX_WORK_ITEM_SIZE: [" << workitems[0] << ',' << workitems[1] << ',' <<workitems[2] << ']' << std::endl;
	clError = clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT,sizeof(cl_bool), &querystatus,NULL);
	printf("CL_DEVICE_IMAGE_SUPPORT: %d\n", querystatus);
	
}


int main(int argc, const char** argv)
{
	//return code
	int ret = EXIT_SUCCESS;


	// pointer to the HOST buffers
	cl_uchar* p_input = nullptr;
	cl_uchar* p_output = nullptr;

	try
	{
		cimg_library::CImg<unsigned char> l_img("seq05_enlarged_00001.pgm");

		int width = l_img.width();
		int height = l_img.height();

		//printf("width = %d, height = %d \n", width, height);

		//Host buffers
		p_input = (cl_uchar*)malloc(sizeof(cl_uchar) * width * (height + 2 * PAD_LINES));
		p_output = (cl_uchar*)malloc(sizeof(cl_uchar) * width * (height + 2 * PAD_LINES));

		// data to input
		memcpy(p_input, l_img.data(), width*height);
		//p_input = l_img.data(0,0,0,0);
		//printf("value = %d \n", p_input[512*512]);

		///////////////////////////////////////////////////

		cl_platform_id * platforms = NULL;
		cl_uint num_platforms;

		cl_int clStatus = clGetPlatformIDs(0, NULL, &num_platforms);
		platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id)*num_platforms);
		clStatus = clGetPlatformIDs(num_platforms, platforms, NULL);

		// device list
		cl_device_id * device_list = nullptr;
		cl_uint num_devices;

		clStatus = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
		device_list = (cl_device_id *)malloc(sizeof(cl_device_id)*num_devices);

		clStatus = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, device_list, NULL);
		LOG_OCL_ERROR(clStatus, "clGetDeviceIDs Failed." );

#if GET_DEVICE_INFO
        for (cl_uint dIndex = 0; dIndex < num_devices; dIndex++){
			printf("==================Device No %d======================\n",dIndex);
            PrintDeviceInfo(device_list[dIndex]);
            printf("====================================================\n\n");
            }
#endif

		// create context
		cl_context context;
		context = clCreateContext(NULL, num_devices, device_list, NULL, NULL, &clStatus);

		// command queue
		cl_command_queue command_queue = clCreateCommandQueue(context, device_list[0], CL_QUEUE_PROFILING_ENABLE, &clStatus);
		


#if USE_CL_IMAGE
		// allocate the buffer with some padding (to avoid boundaries checking)
		cl_image_format image_format;
		image_format.image_channel_data_type = CL_UNSIGNED_INT8;
		image_format.image_channel_order = CL_INTENSITY;

#if USE_OLD_API
		cl_mem clImageIn = clCreateImage2D(context, CL_MEM_READ_ONLY, &image_format, width, (height + 2 * PAD_LINES), 0, NULL, &clStatus);
#else
		cl_image_desc image_desc;
		image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
		image_desc.image_width = width;
		image_desc.image_height = (height + 2 * PAD_LINES);
		image_desc.image_depth = 1;
		image_desc.image_array_size = 1;
		image_desc.image_row_pitch = 0;
		image_desc.image_slice_pitch = 0;
		image_desc.num_mip_levels = 0;
		image_desc.num_samples = 0;
		image_desc.buffer = NULL;

		cl_mem clImageIn = clCreateImage(context, CL_MEM_READ_ONLY, &image_format, &image_desc, NULL, &clStatus);
#endif




		// Copy the buffer to the device
		size_t origin[] = { 0, 0, 0 };
		size_t region[] = { width, (height + 2 * PAD_LINES), 1 };
		cl_event imageToDevice;
		clStatus = clEnqueueWriteImage(command_queue, clImageIn, CL_TRUE, origin, region,
			width * sizeof(cl_uchar), /*row pitch*/
			0, //width * (height + 2 * PAD_LINES) * sizeof(cl_uchar), /* slice pitch */
			p_input, 0, NULL, &imageToDevice);

		if (clImageIn == (cl_mem)0)
		{
			printf("Failed to create Input Image Buffer!:%d", clStatus);
			return EXIT_FAILURE;
		}


		// allocate the buffer with some padding (to avoid boundaries checking)
#if USE_OLD_API
		cl_mem clImageOut = clCreateImage2D(context, CL_MEM_WRITE_ONLY, &image_format, width, (height + 2 * PAD_LINES), 0, NULL, &clStatus);
#else
		cl_mem clImageOut = clCreateImage(context, /*CL_MEM_READ_WRITE*/ CL_MEM_WRITE_ONLY, &image_format, &image_desc, NULL, &clStatus);
#endif

		if (clStatus != CL_SUCCESS)
		{
			printf("Failed to create Output Image Buffer!:%d", clStatus);
			return EXIT_FAILURE;
		}


#else 

		// allocate the buffer with some padding (to avoid boundaries checking)
		cl_mem cl_input_buffer = clCreateBuffer(
			context,
			CL_MEM_READ_ONLY,
			sizeof(cl_uchar) * width * (height + 2 * PAD_LINES),
			NULL,
			&clStatus);

		if (cl_input_buffer == (cl_mem)0)
		{
			printf("Failed to create Input Buffer!");
			return -1;
		}

		// Copy the buffer to the device
		cl_event bufferToDevice;
		clStatus = clEnqueueWriteBuffer(command_queue, cl_input_buffer, CL_TRUE, 0, sizeof(cl_uchar) * width * (height + 2 * PAD_LINES), p_input, 0, NULL, &bufferToDevice);


		// allocate the output buffer with some padding (to avoid boundaries checking)
		cl_mem cl_output_buffer = clCreateBuffer(
			context,
			CL_MEM_WRITE_ONLY,
			sizeof(cl_uchar) * width * (height + 2 * PAD_LINES),
			NULL,
			&clStatus);

		if (cl_output_buffer == (cl_mem)0)
		{
			printf("Failed to create Output Buffer!");
			return EXIT_FAILURE;
		}
#endif // USE_CL_IMAGE



		///////////////////////////////////


		// Create program
		cl_program program = clCreateProgramWithSource(context, 1, (const char **)&binomial_kernel, NULL, &clStatus);

		// Build program
#if 0
		const char options[] = "-Werror -cl-std=CL1.1"; 
#else
		const char options[] = "-Werror -cl-std=CL1.1 -cl-mad-enable -cl-unsafe-math-optimizations -cl-fast-relaxed-math";
#endif


		clStatus = clBuildProgram(program, 1, device_list, options, NULL, NULL);

		if (clStatus != CL_SUCCESS)
		{
		
			cl_build_status status;

			// check build error and build status first
			clGetProgramBuildInfo(program, device_list[0], CL_PROGRAM_BUILD_STATUS,
				sizeof(cl_build_status), &status, NULL);

			size_t logSize;
			// check build log
			clGetProgramBuildInfo(program, device_list[0],
				CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);

			char *programLog = (char*)calloc(logSize + 1, sizeof(char));

			clGetProgramBuildInfo(program, device_list[0],
				CL_PROGRAM_BUILD_LOG, logSize + 1, programLog, NULL);
			printf("Build failed; error=%d, status=%d, programLog:nn%s", clStatus, status, programLog);
			free(programLog);
		}

		SAMPLE_CHECK_ERRORS(clStatus);

		// Create OpenCL kernel
		cl_kernel kernel = clCreateKernel(program, "BinomialFilter", &clStatus);

#if USE_CL_IMAGE
		// Set arguments to the kernel
		clStatus = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&clImageIn);
		clStatus = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&clImageOut);
#else // USE_CL_IMAGE
		// Set arguments to the kernel
		clStatus = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_input_buffer);
		clStatus = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&cl_output_buffer);
#endif // USE_CL_IMAGE


		size_t global_work_size[2] = { (size_t)width, (size_t)height };
		size_t local_work_size[2] = {32 , 4};
		size_t offset[2] = { 0, PAD_LINES };

		double perf_start;
		double perf_stop;


		cl_event runKernel;
		
		perf_start = time_stamp();

		// execute kernel	
		unsigned int ncalls = 10;
		for (int l_i = 0; l_i < ncalls; l_i++)
		{
			clStatus = clEnqueueNDRangeKernel(command_queue, kernel, 2, offset, global_work_size, local_work_size, 0, NULL, &runKernel);
			SAMPLE_CHECK_ERRORS(clStatus);
		}

		clStatus = clFlush(command_queue);
		clStatus = clFinish(command_queue);

		perf_stop = time_stamp();
		printf("       NDRange perf. counter time (%d Filter Calls) %f ms.\n", ncalls, (perf_stop - perf_start) * 1000);

		cl_ulong start;
		cl_ulong end;

		clWaitForEvents(1, &runKernel);
		clGetEventProfilingInfo(runKernel, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, 0);
		clGetEventProfilingInfo(runKernel, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, 0);
		printf("Executing time for last kernel: %f ms. \n", ((double)(end-start))/1000000);


#if USE_CL_IMAGE
		// nothing yet
		clStatus = clEnqueueReadImage(command_queue, clImageOut, CL_TRUE, origin, region,
			width * sizeof(cl_uchar), /*row pitch*/
			0, //width * (height + 2 * PAD_LINES) * sizeof(cl_uchar), /* slice pitch */
			p_output, 0, NULL, NULL);
		SAMPLE_CHECK_ERRORS(clStatus);

#else // USE_CL_IMAGE
		// dump out image
		clStatus = clEnqueueReadBuffer(command_queue, cl_output_buffer, CL_TRUE, 0, sizeof(cl_uchar) * width * (height + 2 * PAD_LINES), p_output, 0, NULL, NULL);
#endif // USE_CL_IMAGE


		// Save image
		SaveImageAsBMP_Y8_as_RGBA(p_output + PAD_LINES*width, width, height, "BinomialFilterOutput.bmp");


		// Rellease all CL objects and host buffers
		clStatus = clReleaseKernel(kernel);
		clStatus = clReleaseProgram(program);
#if USE_CL_IMAGE
		clStatus = clReleaseMemObject(clImageIn);
		clStatus = clReleaseMemObject(clImageOut);
#else
		clStatus = clReleaseMemObject(cl_input_buffer);
		clStatus = clReleaseMemObject(cl_output_buffer);
#endif
		clStatus = clReleaseCommandQueue(command_queue);
		clStatus = clReleaseContext(context);

		free(p_input);
		free(p_output);

		free(platforms);
		free(device_list);

	}
	catch (const Error& error)
	{
		cerr << "[ ERROR ] Sample application specific error: " << error.what() << "\n";
		ret = EXIT_FAILURE;
	}
	catch (const std::exception& error)
	{
		cerr << "[ ERROR ] " << error.what() << "\n";
		ret = EXIT_FAILURE;
	}
	catch (...)
	{
		printf("[ ERROR ] Unknown/internal error happened.\n");
		ret = EXIT_FAILURE;
	}
	
	return ret;
}
