.. install.rst:

########
Install 
########

Build Environments
==================

The |release| version of |project| supports Linux\*-based systems  
with the following packages and prerequisites: 

.. csv-table::
   :header: "Operating System", "Compiler", "Build System", "Status", "Additional Packages"
   :widths: 25, 15, 25, 20, 25
   :escape: ~

   CentOS 7.4 64-bit, GCC 4.8, CMake 3.4.3, supported, ``wget zlib-devel ncurses-libs ncurses-devel patch diffutils gcc-c++ make git perl-Data-Dumper`` 
   Ubuntu 16.04 (LTS) 64-bit, Clang 3.9, CMake 3.5.1 + GNU Make, supported, ``build-essential cmake clang-3.9 git curl zlib1g zlib1g-dev libtinfo-dev``
   Clear Linux\* OS for Intel Architecture, Clang 5.0.1, CMake 3.10.2, experimental, bundles ``machine-learning-basic dev-utils python3-basic python-basic-dev``

Other configurations may work, but should be considered experimental with
limited support. On Ubuntu 16.04 with ``gcc-5.4.0`` or ``clang-3.9``, for 
example, we recommend adding ``-DNGRAPH_USE_PREBUILT_LLVM=TRUE`` to the 
:command:`cmake` command in step 4 below. This fetches a pre-built tarball 
of LLVM+Clang from `llvm.org`_, and will substantially reduce build time.

If using ``gcc`` version 4.8, it may be necessary to add symlinks from ``gcc`` 
to ``gcc-4.8``, and from ``g++`` to ``g++-4.8``, in your :envvar:`PATH`, even 
if you explicitly specify the ``CMAKE_C_COMPILER`` and ``CMAKE_CXX_COMPILER`` 
flags when building. (**Do NOT** supply the ``-DNGRAPH_USE_PREBUILT_LLVM`` 
flag in this case, because the prebuilt tarball supplied on llvm.org is not 
compatible with a gcc 4.8-based build.)


Installation Steps
==================

The CMake procedure installs ``ngraph_dist`` to the installing user's ``$HOME`` 
directory as the default location. See the :file:`CMakeLists.txt` file for 
details about how to change or customize the install location.

The instructions below also presume cloning the nGraph source via an SSH-enabled 
Github account. If you don't have SSH keys set up on your GitHub account, you can 
still follow the instructions below and clone via HTTPS.


Ubuntu
------

The process documented here will work on Ubuntu\* 16.04 (LTS)

#. (Optional) Create something like ``/opt/libraries`` and (with sudo), 
   give ownership of that directory to your user. Creating such a placeholder 
   can be useful if you'd like to have a local reference for APIs and 
   documentation, or if you are a developer who wants to experiment with 
   how to :doc:`../howto/execute` using resources available through the 
   code base.

   .. code-block:: console

      $ sudo mkdir -p /opt/libraries
      $ sudo chown -R username:username /opt/libraries
      $ cd /opt/libraries

#. Clone the `NervanaSystems` ``ngraph`` repo:

   .. code-block:: console

      $ git clone https://github.com/NervanaSystems/ngraph.git
      $ cd ngraph

#. Create a build directory outside of the ``ngraph/src`` directory 
   tree; somewhere like ``ngraph/build``, for example:

   .. code-block:: console

      $ mkdir build && cd build

#. Generate the GNUMakefiles in the customary manner (from within the 
   ``build`` directory). If running ``gcc-5.4.0`` or ``clang-3.9``, remember 
   that you can also append ``cmake`` with the prebuilt LLVM option to 
   speed-up the build. Another option if your deployment system has Intel®
   Advanced Vector Extensions (Intel® AVX) is to target the accelerations 
   available directly by compiling the build as follows during the cmake 
   step: ``-DNGRAPH_TARGET_ARCH=skylake-avx512``.

   .. code-block:: console

      $ cmake ../ [-DNGRAPH_USE_PREBUILT_LLVM=TRUE]

#. Run ``$ make`` and ``make install`` to install ``libngraph.so`` and the 
   header files to ``$HOME/ngraph_dist``:

   .. code-block:: console
      
      $ make   # note: make -j <N> may work, but sometimes results in out-of-memory errors if too many compilation processes are used
      $ make install          

#. (Optional, requires `doxygen`_, `Sphinx`_, and `breathe`_). Run ``make html`` 
   inside the ``doc/sphinx`` directory of the cloned source to build a copy of 
   the `website docs`_ locally. The low-level API docs with inheritance and 
   collaboration diagrams can be found inside the ``/docs/doxygen/`` directory.    


CentOS
------

The process documented here will work on CentOS 7.4.

#. (Optional) Create something like ``/opt/libraries`` and (with sudo), 
   give ownership of that directory to your user. Creating such a placeholder 
   can be useful if you'd like to have a local reference for APIs and 
   documentation, or if you are a developer who wants to experiment with 
   how to :doc:`../howto/execute` using resources available through the 
   code base.

   .. code-block:: console

      $ sudo mkdir -p /opt/libraries
      $ sudo chown -R username:username /opt/libraries

#. Update the system with :command:`yum` and issue the following commands: 
   
   .. code-block:: console

      $ sudo yum update
      $ sudo yum install zlib-devel install ncurses-libs ncurses-devel patch diffutils wget gcc-c++ make git perl-Data-Dumper


#. Install Cmake 3.4:

   .. code-block:: console
    
      $ wget https://cmake.org/files/v3.4/cmake-3.4.3.tar.gz      
      $ tar -xzvf cmake-3.4.3.tar.gz
      $ cd cmake-3.4.3
      $ ./bootstrap
      $ make && sudo make install  

#. Clone the `NervanaSystems` ``ngraph`` repo and use Cmake 3.4.3 to 
   install the nGraph libraries to ``$HOME/ngraph_dist``.

   .. code-block:: console

      $ cd /opt/libraries 
      $ git clone https://github.com/NervanaSystems/ngraph.git
      $ cd ngraph && mkdir build && cd build
      $ cmake ../
      $ make && sudo make install 


macOS\* development
--------------------

.. note:: Although we do not offer support for the macOS platform; some 
   configurations and features may work.

The repository includes two scripts (``maint/check-code-format.sh`` and 
``maint/apply-code-format.sh``) that are used respectively to check adherence 
to ``libngraph`` code formatting conventions, and to automatically reformat code 
according to those conventions. These scripts require the command 
``clang-format-3.9`` to be in your ``PATH``. Run the following commands 
(you will need to adjust them if you are not using bash):

.. code-block:: bash

   $ brew install llvm@3.9
   $ mkdir -p $HOME/bin
   $ ln -s /usr/local/opt/llvm@3.9/bin/clang-format $HOME/bin/clang-format-3.9
   $ echo 'export PATH=$HOME/bin:$PATH' >> $HOME/.bash_profile


Test 
====

The |InG| library code base uses GoogleTest's\* `googletest framework`_ 
for unit tests. The ``cmake`` command from the :doc:`install` guide 
automatically downloaded a copy of the needed ``gtest`` files when 
it configured the build directory.

To perform unit tests on the install:

#. Create and configure the build directory as described in our 
   :doc:`install` guide.

#. Enter the build directory and run ``make check``:
   
   .. code-block:: console

      $ cd build/
      $ make check


Compile a framework with ``libngraph``
======================================

After building and installing nGraph on your system, there are two likely 
paths for what you'll want to do next: either compile a framework to run a DL 
training model, or load an import of an "already-trained" model for inference 
on an Intel nGraph-enabled backend.

For the former case, this early |version|, :doc:`framework-integration-guides`, 
can help you get started with a training a model on a supported framework. 

* :doc:`neon<framework-integration-guides>` framework,  
* :doc:`MXNet<framework-integration-guides>` framework,  
* :doc:`TensorFlow<framework-integration-guides>` framework, and

For the latter case, if you've followed a tutorial from `ONNX`_, and you have an 
exported, serialized model, you can skip the section on frameworks and go directly
to our :doc:`../howto/import` documentation. 

Please keep in mind that both of these are under continuous development, and will 
be updated frequently in the coming months. Stay tuned!  


.. _doxygen: https://www.stack.nl/~dimitri/doxygen/
.. _Sphinx:  http://www.sphinx-doc.org/en/stable/
.. _breathe: https://breathe.readthedocs.io/en/latest/
.. _llvm.org: https://www.llvm.org 
.. _NervanaSystems: https://github.com/NervanaSystems/ngraph/blob/master/README.md
.. _googletest framework: https://github.com/google/googletest.git
.. _ONNX: http://onnx.ai
.. _website docs: http://ngraph.nervanasys.com/docs/latest/
