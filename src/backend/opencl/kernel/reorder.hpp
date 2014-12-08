/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once
#include <kernel_headers/reorder.hpp>
#include <program.hpp>
#include <traits.hpp>
#include <string>
#include <mutex>
#include <map>
#include <dispatch.hpp>
#include <Param.hpp>
#include <debug_opencl.hpp>

using cl::Buffer;
using cl::Program;
using cl::Kernel;
using cl::make_kernel;
using cl::EnqueueArgs;
using cl::NDRange;
using std::string;

namespace opencl
{
    namespace kernel
    {
        // Kernel Launch Config Values
        static const dim_type TX = 32;
        static const dim_type TY = 8;
        static const dim_type TILEX = 512;
        static const dim_type TILEY = 32;

        template<typename T>
        void reorder(Param out, const Param in, const dim_type *rdims)
        {
            try {
                static std::once_flag compileFlags[DeviceManager::MAX_DEVICES];
                static std::map<int, Program*>   reorderProgs;
                static std::map<int, Kernel *> reorderKernels;

                int device = getActiveDeviceId();

                std::call_once( compileFlags[device], [device] () {
                    std::ostringstream options;
                    options << " -D T=" << dtype_traits<T>::getName();
                    if (std::is_same<T, double>::value ||
                        std::is_same<T, cdouble>::value) {
                        options << " -D USE_DOUBLE";
                    }
                    Program prog;
                    buildProgram(prog, reorder_cl, reorder_cl_len, options.str());
                    reorderProgs[device] = new Program(prog);
                    reorderKernels[device] = new Kernel(*reorderProgs[device], "reorder_kernel");
                });

                auto reorderOp = make_kernel<Buffer, const Buffer, const KParam, const KParam,
                                          const dim_type, const dim_type, const dim_type, const dim_type,
                                          const dim_type, const dim_type> (*reorderKernels[device]);

                NDRange local(TX, TY, 1);

                dim_type blocksPerMatX = divup(out.info.dims[0], TILEX);
                dim_type blocksPerMatY = divup(out.info.dims[1], TILEY);
                NDRange global(local[0] * blocksPerMatX * out.info.dims[2],
                               local[1] * blocksPerMatY * out.info.dims[3],
                               1);

                reorderOp(EnqueueArgs(getQueue(), global, local),
                          *out.data, *in.data, out.info, in.info,
                          rdims[0], rdims[1], rdims[2], rdims[3],
                          blocksPerMatX, blocksPerMatY);

                CL_DEBUG_FINISH(getQueue());
            } catch (cl::Error err) {
                CL_TO_AF_ERROR(err);
                throw;
            }
        }
    }
}
